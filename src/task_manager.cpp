#include "task_manager.h"

#include <Arduino.h>
#include <esp_task_wdt.h>
#include <freertos/task.h>

#include "task_2.h"

//-----------------------------------------------------------------------------

TaskHandle_t loop2TaskHandle = NULL;
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
  xTaskCreatePinnedToCore(task2,             // task function
                          "task2",           // taskname
                          8192,              // stack size
                          NULL,              // pointer for task2 parameters
                          2,                 // priority
                          &loop2TaskHandle,  // handle for task
                          1                  // run in core
  );
}

//-----------------------------------------------------------------------------

void enableLoop2WDT() {
  if (loop2TaskHandle != NULL) {
    if (esp_task_wdt_add(loop2TaskHandle) != ESP_OK) {
      log_e("Failed to add loop2 task to WDT");
    } else {
      loopTask2WDTEnabled = true;
    }
  }
}

//-----------------------------------------------------------------------------

void disableLoop2WDT() {
  if (loop2TaskHandle != NULL && loopTask2WDTEnabled) {
    loopTask2WDTEnabled = false;
    if (esp_task_wdt_delete(loop2TaskHandle) != ESP_OK) {
      log_e("Failed to remove loop2 task from WDT");
    }
  }
}

//-----------------------------------------------------------------------------
