#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

int jumpPin = 13;
int ledblue = 12;
int ledred = 11;

// Game state variables (moved to global scope so they don't reset every loop)
int dino_pos[2] = {3, 3}; // row, col
int cactus_pos = 19;
int cloud_pos = 19;
int score = 0;
boolean jumping = false;
boolean falling = false;

// Custom characters
byte dino[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b10110,
  0b11110,
  0b11110,
  0b01010,
  0b01010,
};
byte cactus[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b10101,
  0b10111,
  0b11100,
  0b00100,
  0b00100
};
byte cloud1[8] = {
  0b00000,
  0b00000,
  0b00111,
  0b00111,
  0b00011,
  0b00000,
  0b00000,
  0b00000
};
byte cloud2[8] = {
  0b00000,
  0b01111,
  0b11111,
  0b11111,
  0b11111,
  0b00000,
  0b00000,
  0b00000
};
byte cloud3[8] = {
  0b00000,
  0b10000,
  0b11000,
  0b11100,
  0b11100,
  0b00000,
  0b00000,
  0b00000
};
byte cloud4[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00011,
  0b01111,
  0b01111,
  0b00000
};
byte cloud5[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11000,
  0b11100,
  0b11110,
  0b00000
};

void setup() {
  lcd.init();            
  lcd.backlight();  

  lcd.createChar(1, dino);
  lcd.createChar(2, cactus); 
  lcd.createChar(3, cloud1);
  lcd.createChar(4, cloud2);
  lcd.createChar(5, cloud3);
  lcd.createChar(6, cloud4);
  lcd.createChar(7, cloud5);

  pinMode(jumpPin, INPUT_PULLUP); // safer for button
  pinMode(ledblue, OUTPUT);
  pinMode(ledred, OUTPUT);

  digitalWrite(ledblue, LOW);
  digitalWrite(ledred, LOW);
}

void cactus_dis(int pos) {
  lcd.setCursor(pos, 3);
  lcd.write(2);
}
void cloud_dis(int pos) {
  lcd.setCursor(pos, 0);
  lcd.write(3); lcd.write(4);
}
void dino_dis() {
  lcd.setCursor(dino_pos[1], dino_pos[0]);
  lcd.write(1);
}

void resetGame() {
  dino_pos[0] = 3; dino_pos[1] = 3;
  cactus_pos = 19;
  cloud_pos = 19;
  score = 0;
  jumping = false;
  falling = false;
  digitalWrite(ledblue, LOW);
  digitalWrite(ledred, LOW);
}

void loop() {
  // Show "Press Jump" screen
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Press Jump To Start");
  while (digitalRead(jumpPin) == HIGH); // wait for press

  resetGame();

  // Main game loop
  while (true) {
    lcd.clear();

    // Display elements
    cloud_dis(cloud_pos);
    dino_dis();
    cactus_dis(cactus_pos);

    // Move cactus
    if (cactus_pos > 0) cactus_pos--;
    else {
      cactus_pos = 19;
      score++;
    }

    // Read button
    int jumpbuttonState = digitalRead(jumpPin);

    // Handle jump state machine
    if (jumpbuttonState == LOW && !jumping && !falling) {
      jumping = true;
    }
    if (falling) {
      if (dino_pos[0] < 3) dino_pos[0]++;
      else falling = false;
    } else if (jumping) {
      if (dino_pos[0] > 1) dino_pos[0]--;
      else {
        jumping = false;
        falling = true;
      }
    }

    // Collision check
    if (dino_pos[1] == cactus_pos && dino_pos[0] == 3) {
      lcd.clear();
      lcd.setCursor(5, 1);
      lcd.print("GAME OVER");
      lcd.setCursor(7, 2);
      lcd.print("Score:");
      lcd.print(score);

      digitalWrite(ledblue, HIGH);
      digitalWrite(ledred, HIGH);

      delay(2000);
      break; // exit game loop
    }

    delay(100);
  }
}
