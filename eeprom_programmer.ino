/*
  Arduino sketch for ATMEL AT28C64 Programmator
*/

#define SER 2    //14 serial input
#define RCLK 3   //12 register clock
#define SRCLK 4  //11 shift clock

#define DATA0 5
#define DATA7 12

#define WE 14
#define OE 15

#define TEST

void addrShift (int addr) {
  for (int k = 7; k >= 0; k = k - 1) {
      PORTD = (PORTD & 0xFB) | (((addr >> k) & 0x1) << SER);
      PORTD = PORTD | 0x10;
      PORTD = PORTD & 0xEF;
  }
  return;
}

void setup() {

  //Serial setup
  Serial.begin(115200, SERIAL_8N1);

  pinMode(SER, OUTPUT);
  pinMode(RCLK,OUTPUT);
  pinMode(SRCLK,OUTPUT);
  for (int k = DATA0; k <= DATA7; k  = k + 1) {
    pinMode(k, OUTPUT);
  }

  pinMode(WE, OUTPUT);
  pinMode(OE, OUTPUT);
  PORTC = PORTC | 0b00000011;

  while (Serial.available() == 0) {}
  int num1 = Serial.read(); //Lower byte of the total counter

  while (Serial.available() == 0) {}
  int num2 = Serial.read(); //Upper byte of the total counter

  int num = num1 | (num2 << 8);

  for (int i = 0; i < num; i = i + 1) {
    //Reseive a byte to write
    while (Serial.available() == 0) {}
    byte data = Serial.read();

    //Shifting address
    addrShift(i >> 8);
    addrShift(i);

    //Toggle RCLK
    PORTD = PORTD | 0x8;
    PORTD = PORTD & 0xF7;

    //Set DATA
    PORTD = (PORTD & 0x1F) | (data << 5);
    PORTB = (PORTB & 0xE0) | (data >> 3);

    //Toggle WE with a bit of delay just in case
    PORTC = PORTC & 0xFE;
    asm("nop");
    PORTC = PORTC | 0x1;

    delay(1);
  }

  //In the test mode all written data will be read
  //and sent back
  #ifdef TEST
  for (int k = DATA0; k <= DATA7; k  = k + 1) {
    pinMode(k, INPUT);
  }
  byte val = 0;
  //Memory control
  for (int i = 0; i < num; i = i + 1) {
    //Shift address
    addrShift(i >> 8);
    addrShift(i);

    //Toggle RCLK
    PORTD = PORTD | 0x8;
    PORTD = PORTD & 0xF7;

    //Set OE low
    PORTC = PORTC & 0xFD;

    //Wait a bit before capturing the data
    asm("nop");
    asm("nop");
    val = val | (PINB << 3);
    val = val | (PIND >> 5);

    //Set OE high
    PORTC = PORTC | 0x2;

    char rx[30];
    sprintf(rx, "%02x ", val);
    Serial.print(rx);
    if (i % 16 == 15) {
      Serial.print("\n");
    }
    val = 0;
  }
  #endif
}

void loop() {
  //main cycle
}