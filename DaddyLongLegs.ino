#include <M5Cardputer.h>
#include <Preferences.h>

M5Canvas canvas(&M5.Display);
//colors for easy change
#define COLOR_BACKGROUND  M5.Display.color565(250, 82, 82)  //slamon pink
#define COLOR_BODY        M5.Display.color565(33, 30, 30)    //dark spider body
#define COLOR_LEGS        M5.Display.color565(20, 15, 15)    //dark spider legs
#define COLOR_GROUND      M5.Display.color565(10, 10, 10)    //floor
//anoying physics and stuff
float bodyX          = 120.0;
float bodyY          = 60.0;
float pivotX         = 120.0;
float angle          = 0.08;        
float angleVelocity  = 0.0;

bool activeLeg       = true;    //right or left legs    
bool isGameOver      = false;
bool gameStarted     = false;
bool brokeRecord     = false;    //new highscore congrats

float cameraX        = 0.0;
int maxMeters        = 0;
Preferences prefs;
int highScore        = 0;
//ima name the character spideyy with 2 'y's
void drawCharacter(){                             // i want to make him have customizable clothes like in the game
  float sBodyX  = bodyX  - cameraX;
  float sPivotX = pivotX - cameraX;

  float swingAngle = angle + (angleVelocity * 12.0) + (activeLeg ? 0.5 : -0.5);
  
  float swingFootX = bodyX + (45.0 * sin(swingAngle));
  float swingFootY = 120.0 - abs(sin(angle * 2.0) * 18.0); 
  if (swingFootY > 120.0) swingFootY = 120.0;
  float sSwingFootX = swingFootX - cameraX;

  if (activeLeg) {
    float knee1_X = (bodyX + pivotX) / 2.0 + (3.0 * sin(angle)); 
    float knee1_Y = (bodyY + 120.0)  / 2.0;
    canvas.drawLine(sBodyX, bodyY, knee1_X - cameraX, knee1_Y, COLOR_LEGS);
    canvas.drawLine(knee1_X - cameraX, knee1_Y, sPivotX, 120, COLOR_LEGS);
  } else {
    float knee1_X = (bodyX + swingFootX) / 2.0 + (12.0 * cos(angle)); 
    float knee1_Y = (bodyY + swingFootY) / 2.0 - (6.0 * sin(angle));
    canvas.drawLine(sBodyX, bodyY, knee1_X - cameraX, knee1_Y, COLOR_LEGS); 
    canvas.drawLine(knee1_X - cameraX, knee1_Y, sSwingFootX, swingFootY, COLOR_LEGS);
  }

  if (!activeLeg) {
    float knee2_X = (bodyX + pivotX) / 2.0 + (3.0 * sin(angle));
    float knee2_Y = (bodyY + 120.0)  / 2.0;
    canvas.drawLine(sBodyX, bodyY, knee2_X - cameraX, knee2_Y, COLOR_LEGS);
    canvas.drawLine(knee2_X - cameraX, knee2_Y, sPivotX, 120, COLOR_LEGS);  
  } else {
    float knee2_X = (bodyX + swingFootX) / 2.0 + (12.0 * cos(angle)); 
    float knee2_Y = (bodyY + swingFootY) / 2.0 - (6.0 * sin(angle));
    canvas.drawLine(sBodyX, bodyY, knee2_X - cameraX, knee2_Y, COLOR_LEGS);
    canvas.drawLine(knee2_X - cameraX, knee2_Y, sSwingFootX, swingFootY, COLOR_LEGS);
  }

  canvas.fillRect(sBodyX - 10, bodyY - 10, 20, 20, COLOR_BODY); //body like torso and head
}

void setup() {
  auto cfg = M5.config();
  M5Cardputer.begin(cfg, true);
  M5Cardputer.Display.setRotation(1);
  canvas.createSprite(240, 135);

  prefs.begin("spider-game", false); // Opened in write mode so it updates variables safely
  highScore = prefs.getInt("highscore", 0);
  prefs.end();
}

void loop() {
  M5Cardputer.update();
  bool wPressed = false;

  if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
    Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
    for (auto c : status.word) {
      if (c == 'w' || c == 'W') {
        wPressed = true;
      }
    }
  }

  if (gameStarted == false) {
    canvas.fillScreen(COLOR_BACKGROUND);
    canvas.fillRect(0, 120, 240, 15, COLOR_GROUND); // Thicker block ground!
    drawCharacter();

    canvas.setTextSize(2);
    canvas.setTextColor(COLOR_GROUND);
    canvas.drawString("SPIDER STILTS", 45, 20);
    canvas.setTextSize(1);
    canvas.drawString("Press W to Stand Up & Walk", 40, 60);
    canvas.pushSprite(0, 0);

    if (wPressed) {
      gameStarted = true; // 3, 2, 1 starttt!
      delay(200);
    }
  }
  else if (isGameOver == true) {
    canvas.fillScreen(COLOR_BACKGROUND);
    canvas.fillRect(0, 120, 240, 15, COLOR_GROUND); // Fixed to fillRect to stop compiler thickness crashes
    drawCharacter();

    canvas.setTextSize(2);
    canvas.setTextColor(COLOR_GROUND); 
    canvas.drawString("FACEPLANT", 60, 20);  // maybe make it say other funnythings later
    canvas.setTextSize(1);
    canvas.drawString("Press W to Try Again", 55, 50); // i dont like this i need a better way to say it
    
    canvas.setCursor(65, 75);
    if (brokeRecord) {
      canvas.printf("🎉 NEW BEST: %d m 🎉", highScore);
    } else {
      canvas.printf("Record: %d m", highScore);
    }
    canvas.pushSprite(0 , 0);

    if (wPressed) {
      bodyX         = 120.0;
      bodyY         = 60.0;
      pivotX        = 120.0;
      angle         = 0.08;
      angleVelocity = 0.0;
      isGameOver    = false;
      brokeRecord   = false;
      maxMeters     = 0;
      cameraX       = 0.0;
      delay(300);
    }
  }
  else {
    if (wPressed) {
      float strideLength = abs(bodyX - pivotX);
      
      if (strideLength > 36.0 || strideLength < 6.0) {
        angleVelocity += (angle > 0) ? 0.06 : -0.06; 
      }
      else {
        activeLeg = !activeLeg; 
        pivotX = bodyX + (28.0 * sin(angle)) + (angleVelocity * 15.0); 
        angle = angle * -0.15;      
        angleVelocity = (angleVelocity * 0.3) + 0.012; 
      }
      delay(130); 
    }
    
    angleVelocity += sin(angle) * 0.0055; 
    angle += angleVelocity;
    
    bodyX = pivotX + (60.0 * sin(angle));
    bodyY = 120.0  - (60.0 * cos(angle)) + abs(sin(angle) * 8.0); 
   
    int currentMeters = (bodyX - 120.001) / 10;
    if (currentMeters > maxMeters) {
      maxMeters = currentMeters;
      if (maxMeters > highScore) {
        highScore = maxMeters;
        brokeRecord = true;
        prefs.begin("spider-game", false);
        prefs.putInt("highscore", highScore);
        prefs.end();
      }
    }
    
    cameraX = bodyX - 120.0;
    
    if (bodyY >= 111.0 || angle > 1.35 || angle < -1.35) {
      bodyY = 111.0;
      isGameOver = true;
    }
    
    canvas.fillScreen(COLOR_BACKGROUND);
    canvas.fillRect(0, 120, 240, 15, COLOR_GROUND); // nice thic ground
    drawCharacter();
    canvas.setTextSize(1);
    canvas.setTextColor(COLOR_GROUND);
    canvas.setCursor(10, 10); 
    canvas.printf("Distance: %d m", maxMeters);
    canvas.pushSprite(0, 0);
  }
  delay(16); 
}
