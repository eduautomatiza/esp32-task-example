#include "task_manager.h"

#include <Arduino.h>
#include <esp_task_wdt.h>
#include <freertos/task.h>

#include "task_2.h"

//-----------------------------------------------------------------------------

static TaskHandle_t taskHandle_task2 = NULL;
static bool loopTask2WDTEnabled = false;

//-----------------------------------------------------------------------------

static void task2(void *pvParameters) {
  // setup
  setup2();

  // loop
  for (;;)  // A Task shall never return or exit.
  {
    if (loopTask2WDTEnabled) {
      esp_task_wdt_reset();
    }
    loop2();
  }
}

//-----------------------------------------------------------------------------

void createTask2(void) {
  loopTask2WDTEnabled = false;
  xTaskCreatePinnedToCore(task2,              // task function
                          "task2",            // taskname
                          8192,               // stack size
                          NULL,               // pointer for task2 parameters
                          2,                  // priority
                          &taskHandle_task2,  // handle for task
                          1                   // run in core
  );
}

//-----------------------------------------------------------------------------

void enableLoop2WDT(){
    if(taskHandle_task2 != NULL){
        if(esp_task_wdt_add(taskHandle_task2) != ESP_OK){
            log_e("Failed to add loop2 task to WDT");
        } else {
            loopTask2WDTEnabled = true;
        }
    }
}

//-----------------------------------------------------------------------------

void disableLoop2WDT(){
    if(taskHandle_task2 != NULL && loopTask2WDTEnabled){
        loopTask2WDTEnabled = false;
        if(esp_task_wdt_delete(taskHandle_task2) != ESP_OK){
            log_e("Failed to remove loop2 task from WDT");
        }
    }
}

//-----------------------------------------------------------------------------
