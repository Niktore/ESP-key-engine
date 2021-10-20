//include <Arduino.h>
#include "key.h"
#include "chanel.h"

volatile int count2;  
volatile int count;    // Trigger 
int totalInterrupts;   // counts the number of triggering of the alarm

#define LED_PIN 2

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Code with critica section
void IRAM_ATTR onTime() {
   // portENTER_CRITICAL_ISR(&timerMux);
   // count++;

    // if ( count%2 == 0) {
    if ( count2 == 0) {
         // Lights up the LED if the counter is even 
         digitalWrite(LED_PIN, HIGH);
         count2=1;
       } else {
         // Then swith off
         count2=0;
         digitalWrite(LED_PIN, LOW);
       }
    
   
   // portEXIT_CRITICAL_ISR(&timerMux);
}

// Code without critical section
/*void IRAM_ATTR onTime() {
   count++;
}*/

void setup() {
   Serial.begin(115200);
  
   // Configure LED output
   pinMode(LED_PIN, OUTPUT);
   digitalWrite(LED_PIN, LOW);

   // Configure the Prescaler at 80 the quarter of the ESP32 is cadence at 80Mhz
   // 80000000 / 80 = 1000000 tics / seconde
   timer = timerBegin(0, 8, true);                //80
   timerAttachInterrupt(timer, &onTime, true);    
    
   // Sets an alarm to sound every second 1000000
   timerAlarmWrite(timer, 100, true);  
   //должны получить 5Кгц (1000тиков) получили 4733Гц c критическими секциями, без секций 4996Гц
   // делитель 100 тиков  частота 49,960Гц   вполне приемлемо по скорости 
   
   timerAlarmEnable(timer);

    ch_setup();
   key_setup();
}


// скорость цикла около 500гц 
void loop() {

    // Зажигание лампочек, проверка кнопок 
    key_scan();

    // Переключение каналов и полярности
    chanel_scan();

    // Стробирование должно идти в прерывании
  
    if (count > 0) {
       // Comment out enter / exit to deactivate the critical section 
       portENTER_CRITICAL(&timerMux);
       count--;
       portEXIT_CRITICAL(&timerMux);

       totalInterrupts++;
       Serial.print("totalInterrupts");
       Serial.println(totalInterrupts);
       /*
       if ( totalInterrupts%2 == 0) {
         // Lights up the LED if the counter is even 
         digitalWrite(LED_PIN, HIGH);
       } else {
         // Then swith off
         digitalWrite(LED_PIN, LOW);
       }*/
    }
}

/*

#define DAC1 25

void loop() { // Generate a Sine wave
  int Value = 255; //255= 3.3V 128=1.65V
  
  dacWrite(DAC1, Value);
  delay(1000);
}

*/
