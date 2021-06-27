/* Copyright 2021 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
/* Copyright (C) 2021 Ken'ichi Kuromusha modified, Apache License 2.0
==============================================================================*/

/* 
 *  Namespaced methods for providing IMU data
 *  @author Rikard Lindstrom <rlindsrom@google.com>
*/
#include "data_provider.h"
#include <Arduino_APDS9960.h>

namespace data_provider
{

  /************************************************************************
  * Vars
  ************************************************************************/

  const int GESTURE_MIN = min(min(min(min(GESTURE_NONE, GESTURE_UP), GESTURE_DOWN), GESTURE_LEFT), GESTURE_RIGHT);
  const int GESTURE_MAX = max(max(max(max(GESTURE_NONE, GESTURE_UP), GESTURE_DOWN), GESTURE_LEFT), GESTURE_RIGHT);
  const float GESTURE_MEDIAN = (GESTURE_MAX + GESTURE_MIN) / 2.0;
  const float GESTURE_AMPLITUDE = (GESTURE_MAX - GESTURE_MIN) / 2.0;
  const float COLOR_MAX = 4097;
  int lastProximity, lastR, lastG, lastB;
  const int HISTORY_SIZE = 1000;
  float historyProximity[HISTORY_SIZE];
  float sumHistoryProximity;
  int historyIndex, historySize;

  /************************************************************************
  * "Public" functions
  ************************************************************************/

  bool setup()
  {

    if (!APDS.begin())
    {
      Serial.println("Failed to initialized APDS!");
      return false;
    }

    lastProximity = -1;
    lastR = lastG = lastB = 0;

    for(int i = 0; i < HISTORY_SIZE; i++)
    {
      historyProximity[i] = 0;
    }
    sumHistoryProximity = 0;
    historyIndex = historySize = 0;

    return true;
  }

  void update(float *buffer)
  {

    // read Proximity which has a range of -1 ? 255
    int proximity;
    if(APDS.proximityAvailable())
    {
      proximity = APDS.readProximity();
      lastProximity = proximity;
    }
    else
    {
      proximity = lastProximity;
    }
    float normProximity = (proximity - 127) / 128.0;
    // update sumHistoryProximity & historyProximity
    sumHistoryProximity += normProximity;
    sumHistoryProximity -= historyProximity[historyIndex];
    historyProximity[historyIndex] = normProximity;
    if(++historyIndex >= HISTORY_SIZE)
    {
      historyIndex = 0;
    }
    if(historySize < HISTORY_SIZE)
    {
      historySize++;
    }
    // diff from the average of the last HISTORY_SIZE Proximity
    buffer[0] = (normProximity - sumHistoryProximity / historySize) / 2;

    // read Gesture which has a values of GESTURE_NONE, GESTURE_UP, GESTURE_DOWN, GESTURE_LEFT, GESTURE_RIGHT
    buffer[1] = ((APDS.gestureAvailable() ? APDS.readGesture() : GESTURE_NONE) - GESTURE_MEDIAN) / GESTURE_AMPLITUDE;

    // read Color which has a range of 0 - COLOR_MAX
    int r, g, b;
    if(APDS.colorAvailable())
    {
      APDS.readColor(r, g, b);
      lastR = r;
      lastG = g;
      lastB = b;
    }
    else
    {
      r = lastR;
      g = lastG;
      b = lastB;
    }

    // convert from RGB to HVS which has a range of 0 - 1
    int rgb_min = min(min(r, g), b);
    int rgb_max = max(max(r, g), b);
    float rgb_min_max_diff = rgb_max - rgb_min;
    float h = rgb_min == rgb_max ? 0 :
              (r == rgb_max ? (g - b) / rgb_min_max_diff + 1 :
               g == rgb_max ? (b - r) / rgb_min_max_diff + 3 :
                              (r - g) / rgb_min_max_diff + 5) / 6;
    float s = rgb_min_max_diff / COLOR_MAX;
    float v = rgb_max / COLOR_MAX;
    buffer[2] = (h - 0.5) * 2;
    buffer[3] = (s - 0.5) * 2;
    buffer[4] = (v - 0.5) * 2;
  }
}
