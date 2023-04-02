#include <Arduino.h>
#include <Preferences.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

TaskHandle_t task1Handle;

const int TOUCH_PIN = 12;
const int TOUCH_THRESHOLD = 74;
const int OUTPUT_PIN = 4;
int touchValue = 0;
bool isTouched = false;
bool isOn = false;

Preferences preferences;

void task1(void *pvParameters) {

  while (true) {
    //Serial.print("Touch reading: ");

    touchValue = touchRead(TOUCH_PIN);
    Serial.print(touchRead(TOUCH_PIN));
    if (touchValue <= TOUCH_THRESHOLD) {
      if (!isTouched) {
        isTouched = true;
        isOn = !isOn;
        preferences.putBool("isOn", isOn);
        preferences.end();
        delay(50);
      }
    } 
    else 
    {
      isTouched = false;
    }    
  Serial.println(isOn ? "\tON" : "\tOFF");
  digitalWrite(OUTPUT_PIN, isOn ? HIGH : LOW);
  vTaskDelay(pdMS_TO_TICKS(150)); // Delay for 1 second

  if (!isOn)
  {
    ESP. restart();
  }

  }

}

void setup() {
  Serial.begin(9600);

  pinMode(OUTPUT_PIN, OUTPUT);

  preferences.begin("my_app", false);
  isOn = preferences.getBool("isOn", false);
  digitalWrite(OUTPUT_PIN, isOn ? HIGH : LOW);
  Serial.println(isOn ? "\tON" : "\tOFF");
   
  while(1)
  {
    int touchValue = touchRead(TOUCH_PIN);
    //Serial.println(touchRead(TOUCH_PIN));    // to monitor the touch threshold value
    if (touchValue <= TOUCH_THRESHOLD) {
    if (!isTouched) {
      isTouched = true;
      isOn = !isOn;
      preferences.putBool("isOn", isOn);
      preferences.end();
      delay(50);
    }
  } 
  else 
   {
    isTouched = false;
   }

  if(isOn == 1)
   {
      break;
   }
   delay(150);

  }

  xTaskCreate(task1, "Task 1", 10000, NULL, 1, &task1Handle);

}

void loop() {
  //Serial.println("\tESP32 : RUNNING\n");
  delay(250);

}