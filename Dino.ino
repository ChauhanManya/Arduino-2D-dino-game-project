#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,20,4);
int jumpPin = 13;
int ledblue = 12;
int ledred = 11;

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
byte cactus[8]= {
  0b00000,
  0b00100,
  0b00100,
  0b10101,
  0b10111,
  0b11100,
  0b00100,
  0b00100
  };

byte cloud1[8]= {
  0b00000,
  0b00000,
  0b00111,
  0b00111,
  0b00011,
  0b00000,
  0b00000,
  0b00000
  };
byte cloud2[8]= {
  0b00000,
  0b01111,
  0b11111,
  0b11111,
  0b11111,
  0b00000,
  0b00000,
  0b00000
  };
byte cloud3[8]= {
  0b00000,
  0b10000,
  0b11000,
  0b11100,
  0b11100,
  0b00000,
  0b00000,
  0b00000
  };

byte cloud4[8]= {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00011,
  0b01111,
  0b01111,
  0b00000
  };
byte cloud5[8]= {
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
  lcd.createChar(3, cloud1);//Cloud1 part 1
  lcd.createChar(4, cloud2); //Cloud1 part 2
  lcd.createChar(5, cloud3); //Cloud2 part 1
  lcd.createChar(6, cloud4);//Cloud2 part 2
  lcd.createChar(7, cloud5); //Cloud2 part 3 
  pinMode(jumpPin, INPUT);
  pinMode(ledblue, OUTPUT);
  pinMode(ledred, OUTPUT);
  
}

void cactus_dis(int pos){
  lcd.setCursor(pos,3);
  lcd.write(2);

}
void cloud_dis(int pos){
  lcd.setCursor(pos,0);
  lcd.write(3);lcd.write(4);
}

void dino_dis(int dino_pos[]) {
  lcd.setCursor( dino_pos[1],dino_pos[0]); //col,row
  lcd.write(1);
}


void loop() {
  lcd.clear();

  int jumpbuttonState = digitalRead(jumpPin);
  int dino_pos[2] = {3, 3}; // row, col
  int cactus_pos = 19; 
  int cloud_pos = 19; 
  int score =0;
  boolean jumping, falling= false;
  
  //while jump button hasnt been pressed do not start the game
  do{
  lcd.setCursor(0, 1);
  lcd.print("Press Jump To Start");
  jumpbuttonState = digitalRead(jumpPin);
  }while(jumpbuttonState != LOW); //low means pressed , high means not pressed
  
  do {
    lcd.clear();
    
    cloud_dis(cloud_pos);
    dino_dis(dino_pos);
    cactus_dis(cactus_pos); //display cactus
    
    if (cactus_pos> 0) --cactus_pos; //change cactus position to move left
    else cactus_pos = 19; //cactus goes back to start position if it reached the end of the line
    
    if (jumpbuttonState == HIGH && !jumping && !falling) jumping = true;  // Start jump
    if (falling) {
      if (dino_pos[0] < 3) ++dino_pos[0];  // Move down
      else falling = false; // Landed
    }
    else if (jumping) {
      if (dino_pos[0] > 1) --dino_pos[0];  // Move up
      else {
        jumping = false;
        falling = true; // Start falling when top is reached
      }
    }
    delay(100);

  } while (dino_pos[1] != cactus_pos); //if cactus and dino collide
  
  // Game Over
  lcd.setCursor(5, 1);
  lcd.print("GAME OVER");
  lcd.setCursor(9,2); //col,row
  lcd.print(score);
  ledblue =HIGH 
  ledred = HIGH
  while (true) {
    jumpbuttonState = digitalRead(jumpPin);
    if (jumpbuttonState == HIGH) break;
  }
  
}
