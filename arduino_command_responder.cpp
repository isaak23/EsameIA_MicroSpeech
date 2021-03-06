/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include "command_responder.h"
#include "Arduino.h"


// definisco i pin che verranno usati per inviare i segnali all'arduino UNO
#define SI_PIN          2
#define NO_PIN          3
#define DANIELE_PIN     4
#define UNKNOW_PIN      5

// Toggles the built-in LED every inference, and lights a colored LED depending
// on which word was detected.
void RespondToCommand(tflite::ErrorReporter* error_reporter,
                      int32_t current_time, const char* found_command,
                      uint8_t score, bool is_new_command) {
                        
  /*qui ci sono tutte le inizializzazioni, vengono impostati i pin come output e vengono inizializzati con valore BASSO (LOW) */
  static bool is_initialized = false;
  if (!is_initialized) {
    pinMode(LED_BUILTIN, OUTPUT);
    // Pins for the built-in RGB LEDs on the Arduino Nano 33 BLE Sense
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    
    pinMode(SI_PIN, OUTPUT);
    pinMode(NO_PIN, OUTPUT);
    pinMode(DANIELE_PIN, OUTPUT);
    pinMode(UNKNOW_PIN, OUTPUT);
    // Ensure the LED is off by default.
    // Note: The RGB LEDs on the Arduino Nano 33 BLE
    // Sense are on when the pin is LOW, off when HIGH.
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);

    digitalWrite(SI_PIN, LOW);
    digitalWrite(NO_PIN, LOW);
    digitalWrite(DANIELE_PIN, LOW);
    digitalWrite(UNKNOW_PIN, LOW);
    is_initialized = true;
  }
  static int32_t last_command_time = 0;
  static int count = 0;
  static int certainty = 220;
  
/* in questa parte di codice ci sono i vari if in funzione dell'avvenuto riconoscimento della parola */

  if (is_new_command) {
    TF_LITE_REPORT_ERROR(error_reporter, "Riconosciuto: %s (%d) @%dms", found_command,
                         score, current_time);
    // If we hear a command, light up the appropriate LED
    if (found_command[0] == 's' && found_command[1] == 'i' && found_command[2] == 'i') {
      last_command_time = current_time;
      digitalWrite(LEDG, LOW);  // Green for "si"
      digitalWrite(SI_PIN, HIGH);
      last_command_time = current_time;
    }

    if (found_command[0] == 'n' && found_command[1] == 'o' && found_command[2] == 'o') {
      last_command_time = current_time;
      digitalWrite(LEDR, LOW);  // Red for "no"
      digitalWrite(NO_PIN, HIGH);
      last_command_time = current_time;
    }

    if (found_command[0] == 'd') {
      last_command_time = current_time;
      digitalWrite(LEDB, LOW);  // Blu for "daniele"
      digitalWrite(DANIELE_PIN, HIGH);
      last_command_time = current_time;
    }

    if (found_command[0] == 'u') {
      last_command_time = current_time;
      digitalWrite(LEDB, LOW);  // Blue + Green for "unknown"
      digitalWrite(LEDG, LOW);  // Blue + Green for "unknown"
      digitalWrite(UNKNOW_PIN, HIGH);
      last_command_time = current_time;
    }
  }

  // If last_command_time is non-zero but was >3 seconds ago, zero it
  // and switch off the LED.
  if (last_command_time != 0) {
    if (last_command_time < (current_time - 3000)) {
      last_command_time = 0;
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(LEDR, HIGH); // i led integrati nell'arduino NANO sono spenti se viene impostato il valore HIGH
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, HIGH);

      /* se last_command_time ?? non-zero ma maggiore di 3 secondi, oltre i led integrati vengono riportati a LOW i pin utilizzati per inviare i segnali all'ARDUINO UNO */
      digitalWrite(SI_PIN, LOW);
      digitalWrite(NO_PIN, LOW);
      digitalWrite(DANIELE_PIN, LOW);
      digitalWrite(UNKNOW_PIN, LOW);
    }
    // If it is non-zero but <3 seconds ago, do nothing.
    return;
  }

  // Otherwise, toggle the LED every time an inference is performed.
  ++count;
  if (count & 1) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

#endif  // ARDUINO_EXCLUDE_CODE
