// settings.h

#ifndef _SETTINGS_h
#define _SETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif





// Настройки радиомодуля

#define RECEIVER_PIPE_ADDR 0xABCDABCDE1LL			//адрес приемной трубы у переключателя-приемника
#define TRANSMITTER_PIPE_ADDR 0xABCDABCDE2LL		//адрес приемной трубы у пульта управления - передатчика
#define CHANNEL	10									//номер используемого радиочастотного канала
#define DATA_RATE RF24_1MBPS						// Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
#define POWER_GAIN RF24_PA_MAX						// Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)

#define KEEPALIVE_TIMEOUT 20						// пауза перед отправкой кипэлайва 20 миллисекунд

#define SET_UP_RF	radio.begin();\
					radio.setChannel(CHANNEL);\
					radio.setDataRate(DATA_RATE);\
					radio.setPALevel(POWER_GAIN);\
					radio.setAutoAck( false );\
					radio.enableDynamicAck();\
					radio.openWritingPipe (TRANSMITTER_PIPE_ADDR);\
					radio.openReadingPipe (1, RECEIVER_PIPE_ADDR);\
					radio.startListening  ();

//Максимальное кол-во приемников
#define RECEIVER_QTY 7					
					
// Настройки реле - какому выходу соответствует какой цвет
#define RED 2										// реле управления красным светом подключается на второй цифровой выход
#define YELLOW 3									// реле управления желтым светом подключается на третий цифровой выход
#define GREEN 4										// реле управления зеленым светом подключается на четвертый цифровой выход

//Порядок цветов в управляющем пакете
#define IS_BIT_SET(number,bit) (!!(number & (1 << bit)))
#define SET_COLOR(command, color) (IS_BIT_SET(command, color) ? HIGH : LOW) // красный - 0, желтый - 1, зеленый - 2

// Инициализация выходов
#define ENABLE_TC_COLOR(color)	pinMode(color, OUTPUT);\
								digitalWrite(color, LOW);

// Настройки логической адресации
// Задаем адрес устройства дип-переключателем. Каждый переключатель кодирует один бит, положение бита соответствует номеру пина (PIN0 - 2^0 и т.д.)
#define DEV_ADDR_PIN0 A0
#define DEV_ADDR_PIN1 A1
#define DEV_ADDR_PIN2 A2
#define DEV_ADDR_PIN3 A3
#define DEV_ADDR_PIN4 A4

//Устанавливает правильный режим работы аналоговых входов.
#define ENABLE_DEV_ADDR pinMode(DEV_ADDR_PIN0, INPUT_PULLUP); \
						pinMode(DEV_ADDR_PIN1, INPUT_PULLUP); \
						pinMode(DEV_ADDR_PIN2, INPUT_PULLUP); \
						pinMode(DEV_ADDR_PIN3, INPUT_PULLUP); \
						pinMode(DEV_ADDR_PIN4, INPUT_PULLUP);

//#define ADDR_PIN_STATE(pin_num) (digitalRead(DEV_ADDR_PIN##pin_num)) ; 						

//#define SET_DEV_ADDRESS for (int i = 0; i < 4; i++) devAddr += ADDR_PIN_STATE(i) << i;

#define SET_DEV_ADDRESS devAddr = (((digitalRead(DEV_ADDR_PIN4))<<4) | ((digitalRead(DEV_ADDR_PIN3))<<3) | ((digitalRead(DEV_ADDR_PIN2))<<2) | ((digitalRead(DEV_ADDR_PIN1))<<1) | (digitalRead(DEV_ADDR_PIN0)));

#endif