#include <Arduino.h>
#include <SD.h>
#include <PushButton.h>
#include "WavFileWriter.h"
#include "config.h"
#include "i2s_mic_std.h"

PushButton button1(BUTTON_1_PIN);
PushButton button2(BUTTON_2_PIN);
PushButton button3(BUTTON_3_PIN);
PushButton button4(BUTTON_4_PIN);

bool isRecording = false;

// Mic
I2S_Mic mic(I2S_MIC_PORT);

// WAV



#define I2S_SAMPLE_RATE 16000
#define I2S_SAMPLE_BITS 32
#define I2S_CHANNEL_NUM 1
#define I2S_READ_LEN 1024

WavFileWriter wavFileWriter;
const char* audioFilename = "/recording.wav";

void onButton1Released(unsigned int duration) {
  Serial.printf("Button 1 released after %zu millis.\n", duration);
}

void onButton2Released(unsigned int duration) {
  Serial.printf("Button 2 released after %zu millis.\n", duration);
}

void onButton3Released(unsigned int duration) {
  Serial.printf("Button 3 released after %zu millis.\n", duration);
}

void onButton4Pressed() {
  Serial.println("Button 4 pressed.");

  // Wav file writer.
  wavFileWriter.init(I2S_SAMPLE_RATE, 16, 1);
  wavFileWriter.start(SD, audioFilename);

  Serial.println("Start recording...");
  isRecording = true;
}

void onButton4Released(unsigned int duration) {
  Serial.println("Stopping recording...");
  // Set iRecording to false as soonas possible.
  isRecording = false;

  wavFileWriter.finish();

  Serial.println("File saved and closed.");
}

void recordToWav() {
  if (!isRecording)
    return;

  Serial.println("Reading from mic...");

  //int32_t i2sBuffer[1024];
  int16_t i2sBuffer[1024];
  
  size_t bytes_read;
  mic.read16bits((int16_t *)&i2sBuffer, 1024, &bytes_read);
  Serial.printf("Bytes read %lu.\n", bytes_read);
  size_t bytesWritten = wavFileWriter.write((uint8_t*)&i2sBuffer, bytes_read);
  
  /*mic.read((void *)i2sBuffer, 1024 * sizeof(int32_t), &bytes_read);

  if (bytes_read > 0) {
    int samplesCount = bytes_read / sizeof(int32_t);
    int16_t sampleBuffer[samplesCount];

    float gain = 8.0;
    
    for (int x = 0; x < samplesCount; x++) {
      sampleBuffer[x] = (i2sBuffer[x] >> 16) * gain;
    }
     
    size_t bytesWritten = wavFileWriter.write((uint8_t*)&sampleBuffer, samplesCount * sizeof(int16_t));
    //Serial.printf("Bytes written: %lu.\n", bytesWritten);
  } else {
    Serial.println("No bytes written.");
  }
  */
}

void buttonsLoop() {
  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();
}

void setup() {
  // we need serial output for the monitor.
  Serial.begin(115200);

  if (!SD.begin()) {
    Serial.println("SD Card initialization failed!");
    while (true);
  }
  
  Serial.println("SD Card initialized.");

  // MIC
  Serial.println("Setting mic pinouts.");
  mic.setPinout(I2S_MIC_SERIAL_CLOCK, I2S_MIC_LEFT_RIGHT_CLOCK, I2S_MIC_SERIAL_DATA);
  Serial.println("Setting mic gain.");
  mic.setGain(10);
  Serial.println("Calling mic begin..");
  mic.begin();
  
  // Set buttons.
  button1.begin(INPUT_PULLUP);
  button2.begin(INPUT_PULLUP);
  button3.begin(INPUT_PULLUP);
  button4.begin(INPUT_PULLUP);

  button1.setReleasedCallback(onButton1Released);
  button2.setReleasedCallback(onButton2Released);
  button3.setReleasedCallback(onButton3Released);
  // Record button
  button4.setPressedCallback(onButton4Pressed);
  button4.setReleasedCallback(onButton4Released);
}

void loop() {
  // Buttons
  buttonsLoop();

  if(isRecording)
    recordToWav();
}
