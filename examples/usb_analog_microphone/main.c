/*
 * Copyright (c) 2021 Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * This examples creates a USB Microphone device using the TinyUSB
 * library and captures data from a PDM microphone using a sample
 * rate of 16 kHz, to be sent the to PC.
 * 
 * The USB microphone code is based on the TinyUSB audio_test example.
 * 
 * https://github.com/hathach/tinyusb/tree/master/examples/device/audio_test
 */
#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/analog_microphone.h"

#include "usb_microphone.h"

// configuration
const struct analog_microphone_config config = {
  .gpio = 26,
  .bias_voltage = 1.65,
  .sample_rate = SAMPLE_RATE,
  .sample_buffer_size = SAMPLE_BUFFER_SIZE,
};

// variables
uint16_t sample_buffer[SAMPLE_BUFFER_SIZE];

// callback functions
void on_analog_samples_ready();
void on_usb_microphone_tx_ready();

int main(void)
{
  // initialize and start the analog microphone
  analog_microphone_init(&config);
  analog_microphone_set_samples_ready_handler(on_analog_samples_ready);
  analog_microphone_start();

  // initialize the USB microphone interface
  usb_microphone_init();
  usb_microphone_set_tx_ready_handler(on_usb_microphone_tx_ready);

  while (1) {
    // run the USB microphone task continuously
    usb_microphone_task();
  }

  return 0;
}

void on_analog_samples_ready()
{
  // Callback from library when all the samples in the library
  // internal sample buffer are ready for reading.
  //
  // Read new samples into local buffer.
  analog_microphone_read(sample_buffer, SAMPLE_BUFFER_SIZE);
}

void on_usb_microphone_tx_ready()
{
  // Callback from TinyUSB library when all data is ready
  // to be transmitted.
  //
  // Write local buffer to the USB microphone
  usb_microphone_write(sample_buffer, sizeof(sample_buffer));
}
