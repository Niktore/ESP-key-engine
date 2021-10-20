
// Модуль включения оптопар
// Вызывается в основном цикле (не прерывании) заранее, перед стробированием 
// Обвернуть в критические секции/мютех - переключение не должно начатся в момент стробирования 


// Какие каналы включить
volatile byte mode_chanel; 
/* двухбитовое кодирование импульса на выходе, можно применить и массив 
PM PM PM PM
*/

byte step_chanel;          // состояние автомата 8 шагов

// Линии сканирования катодов оптопар / каналов 
#define CH1 13   // Активный уровень 0
#define CH2 16   
#define CH3 17   
#define CH4 18   


// Линии ключей полярности/анодов
#define P1 19   // Активный уровень 1
#define M2 23   
#define P2 22   // Диагональная пара    
#define M1 21   

/*
+100В  сверху
19              22
клемма + R - клемма 
21              23
снизу "земля"
*/

void ch_all_off() {
  digitalWrite(CH1, HIGH);
  digitalWrite(CH2, HIGH);
  digitalWrite(CH3, HIGH);
  digitalWrite(CH4, HIGH);
  
  digitalWrite(P1, LOW);
  digitalWrite(P2, LOW);
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
}

void ch_setup() {
  pinMode(CH1, OUTPUT);
  pinMode(CH2, OUTPUT);
  pinMode(CH3, OUTPUT);
  pinMode(CH4, OUTPUT);

  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M1, OUTPUT);

  ch_all_off();
}


void set_polarity(byte n) {
  if (mode_chanel & (1<<n)) {         // есть что включать 
    if  ( n%2 == 0)  {
         digitalWrite(P1, HIGH);      // плюсы
         digitalWrite(M2, HIGH);
    } else {
         digitalWrite(P2, HIGH);
         digitalWrite(M1, HIGH);      //минусы
    }
  } //if
}


void chanel_scan() {

  // всё выключаем
  ch_all_off();

  // Сначала включаем нужную полярность 
  set_polarity(step_chanel);

  // Потом нужный канал 
  if ((1 == step_chanel) || (0 == step_chanel)) {
    digitalWrite(CH1, LOW);
  }
  if ((3 == step_chanel) || (2 == step_chanel)) {
    digitalWrite(CH2, LOW);
  }
  if ((5 == step_chanel) || (4 == step_chanel)) {
    digitalWrite(CH3, LOW);
  }
  if ((7 == step_chanel) || (6 == step_chanel)) {
    digitalWrite(CH4, LOW);
  }

  // канал вКлючен, и остаётся таким !!!!
  // нужно ВСё выключить или по таймауту или после стробирования 
  step_chanel++;
  if ( 8 == step_chanel)  step_chanel = 0;
}
