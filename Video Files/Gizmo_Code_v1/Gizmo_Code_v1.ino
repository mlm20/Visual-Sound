// MSGEQ7
#include "MSGEQ7.h"
#define pinReset 50
#define pinStrobe 52
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define smoothing false // Range: 0-255
#define leftPin A0
#define rightPin A1

// Setup MSGEQ7 Object
/*
 * template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin
 *           uint8_t firstAnalogPin, uint8_t ...analogPins>
 */
// Two MSGEQ7 ICs (stereo), Smoothing disabled (0%)
CMSGEQ7<smoothing, pinReset, pinStrobe, leftPin, rightPin> MSGEQ7;

void setup() {
  // This will set the IC ready for reading
  MSGEQ7.begin();
  
  // Setup serial monitor
  Serial.begin(115200);
  Serial.println(F("Startup"));
}

void loop() {
  // Make reading (set number of times per second)
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Calculate FPS
  uint16_t FPS = getFPS(newReading);

  // Save readings (all bands)
  uint8_t band0 = MSGEQ7.get(MSGEQ7_0);
  uint8_t band1 = MSGEQ7.get(MSGEQ7_1);
  uint8_t band2 = MSGEQ7.get(MSGEQ7_2);
  uint8_t band3 = MSGEQ7.get(MSGEQ7_3);
  uint8_t band4 = MSGEQ7.get(MSGEQ7_4);
  uint8_t band5 = MSGEQ7.get(MSGEQ7_5);
  uint8_t band6 = MSGEQ7.get(MSGEQ7_6);

  // Remove noise from signals
  band0 = mapNoise(band0);
  band1 = mapNoise(band1);
  band2 = mapNoise(band2);
  band3 = mapNoise(band3);
  band4 = mapNoise(band4);
  band5 = mapNoise(band5);
  band6 = mapNoise(band6);

  // Print to Serial
  Serial.println(band3);
}
