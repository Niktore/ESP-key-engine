
// Модуль сканирования кнопок и лампочек

// Глобальные переменные для передачи значений 
volatile byte ledSwich;  // включенные лампочки 
volatile byte press_key; // нажатые кнопки
byte step_scan;          // состояние автомата

// Линии включения ламп
#define LED_SCAN_VD11 2  // Активный уровень 0
#define LED_SCAN 15

// Линии сканирования кнопок
#define SCAN_S1 4   // Активный уровень 1
#define SCAN_S2 5
#define SCAN_S3 12
#define SCAN_S4 14

#define OUTIN_KEY 32  // возвратная линия кнопок, активный уровень 1

void key_setup() {

  pinMode(SCAN_S1, OUTPUT);
  pinMode(SCAN_S2, OUTPUT);
  pinMode(SCAN_S3, OUTPUT);
  pinMode(SCAN_S4, OUTPUT);

  pinMode(LED_SCAN_VD11, OUTPUT);
  pinMode(LED_SCAN, OUTPUT);

  digitalWrite(SCAN_S1, LOW);
  digitalWrite(SCAN_S2, LOW);
  digitalWrite(SCAN_S3, LOW);
  digitalWrite(SCAN_S4, LOW);

  // Погасить фоарики
  digitalWrite(LED_SCAN_VD11, HIGH);
  digitalWrite(LED_SCAN, HIGH);

  pinMode(OUTIN_KEY, INPUT);
}

// Проверка линии нажатия кнопки
byte key_get(byte n) {
  if (HIGH == digitalRead(OUTIN_KEY)) {
        press_key = press_key | (1<<n);
      } else { 
        // самосбрасывание при отпускании кнопки
        // убрать самосброс будет debouncing, в обработчике кнопок сбрасывать флаги нажатых кнопок самостоятельно 
        press_key = press_key & (~(1<<n));
      }
}

// Зажигание лампочек
void led_set(byte n) {
  if ((ledSwich & n) & 0b000001111) {
     digitalWrite(LED_SCAN, LOW);
  }

  if ((ledSwich & n) & 0b000010000) {
     digitalWrite(LED_SCAN_VD11, LOW);
  }
}


// Здесь имеем автомат переходов за 4 вызова проверяет 4 кнопки, а не за раз все 
// Возвращает нажатую кнопку, через глобальную переменную
byte key_scan() {

  // Гасим все лампочки
  digitalWrite(SCAN_S1, LOW);
  digitalWrite(SCAN_S2, LOW);
  digitalWrite(SCAN_S3, LOW);
  digitalWrite(SCAN_S4, LOW);

  digitalWrite(LED_SCAN_VD11, HIGH);
  digitalWrite(LED_SCAN, HIGH);
 
 switch (step_scan) {
    case 0:
      digitalWrite(SCAN_S1, HIGH);
      break;
    case 1:
      digitalWrite(SCAN_S2, HIGH);
      break;
    case 2:
      digitalWrite(SCAN_S3, HIGH);
      break;
    case 3:
      digitalWrite(SCAN_S4, HIGH);
      break;
  
    default: 
    step_scan = 0;
  }

  // Фонарик (один из) горит до следущего цикла опроса
  led_set(step_scan);
  key_get(step_scan);
  step_scan++;
  if (step_scan > 3) step_scan = 0;
}
