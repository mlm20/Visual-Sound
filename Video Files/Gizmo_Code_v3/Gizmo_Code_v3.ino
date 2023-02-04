// Libraries
#include <arduinoFFT.h>
#include <Servo.h>

// FFT global variables
#define SAMPLES 512 // Must be a power of 2
#define SAMPLING_FREQ 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE 10 // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
#define AUDIO_IN_PIN A0 // Signal in on this pin
#define NUM_BANDS 5 // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
#define NOISE 500 // Used as a crude noise filter, values below this are ignored
#define MAX_OUTPUT 180 // maximum output of FFT (maximum angle of servo)
#define MIN_OUTPUT 0 // minimum output of FFT (minmum angle of servo)

// Servo global variables
Servo servo_0;
Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;

// Sampling and FFT stuff
unsigned int sampling_period_us;
byte peak[] = {0,0,0,0,0};              // The length of these arrays must be >= NUM_BANDS
int oldServoAngle[] = {0,0,0,0,0};
int bandValues[] = {0,0,0,0,0};
int servoAngleArray[] = {0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime;
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

// Run on start
void setup() {
  // Start logging serial monitor
  Serial.begin(115200);

  // FFT Stuff
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));

  // Assign servo PWM pins
  servo_0.attach(2);
  servo_1.attach(3);
  servo_2.attach(4);
  servo_3.attach(5);
  servo_4.attach(6);

  // Set servos to bottom position -> 0°
  servo_0.write(0);
  servo_1.write(0);
  servo_2.write(0);
  servo_3.write(0);
  servo_4.write(0);
}

void loop() {

  // Reset bandValues[]
  for (int i = 0; i < NUM_BANDS; i++){
    bandValues[i] = 0;
  }

  // Sample the audio pin
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_IN_PIN); // A conversion takes about 9.7uS on an ESP32
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }
  }

  // Compute FFT
  FFT.DCRemoval();
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  // Analyse FFT results
  for (int i = 2; i < (SAMPLES/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    if (vReal[i] > NOISE) {                    // Add a crude noise filter

  // 5 bands, 16kHz top band
      if (i <= 2) bandValues[0] += (int)vReal[i];
      if (i > 2 && i <= 9) bandValues[1] += (int)vReal[i];
      if (i > 9 && i <= 34) bandValues[2] += (int)vReal[i];
      if (i > 34 && i <= 130) bandValues[3] += (int)vReal[i];
      if (i > 130) bandValues[4] += (int)vReal[i];
    }
  }

  // Process FFT results into servo inputs (0-180)
  for (byte band = 0; band < NUM_BANDS; band++) {
  
    // Scale outputs to within range
    int servoAngle = bandValues[band] / AMPLITUDE;

    // Set values to either MAX or MIN if values fall outside of range
    if (servoAngle > MAX_OUTPUT) {
      servoAngle = MAX_OUTPUT;
    }
    if (servoAngle < MIN_OUTPUT) {
      servoAngle = MIN_OUTPUT;
    }

    // Smooth FFT Outputs w.r.t. previous values
    servoAngle = ((oldServoAngle[band] * 1) + servoAngle) / 2;

    // Save servoAngle to Array to use later
    servoAngleArray[band] = servoAngle;
    
    // Save to oldServoAngle for next round of averaging
    oldServoAngle[band] = servoAngle;
  }

  // Print FFT Results
  for (int i = 0; i < NUM_BANDS; i++) {
    Serial.print(servoAngleArray[i]);
    Serial.print(" ");
  }
  
  Serial.println();

  // Move servo motors
  servo_0.write(servoAngleArray[0]);
  servo_1.write(servoAngleArray[1]);
  servo_2.write(servoAngleArray[2]);
  servo_3.write(servoAngleArray[3]);
  servo_4.write(servoAngleArray[4]);
}
