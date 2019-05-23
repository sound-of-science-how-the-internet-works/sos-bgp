#include <FastLED.h>

#define LED_PIN     10
#define NUM_LEDS    150
#define BRIGHTNESS  64
#define LED_TYPE    WS2813
#define COLOR_ORDER GRB
#define DEBUG_LEVEL 0 // 0 = Display nothing, 1 = test sequence, 2 = game events, 3 = small events, 5 = detail
CRGB leds[NUM_LEDS];
int start = 0;
int end = 0;
int broken = 0;
bool solved = true;

// ID of led
int loc_leds[6] {
  0,  // 0
  13, // 1
  32, // 2
  37, // 3
  62, // 4
  66  // 5
};
// Configure pin of connection
int connection_pins[6] {
  2, // 0
  3, // 1
  4, // 2
  5, // 3
  6, // 4
  7  // 5
};

// lane leds array
int lane_leds[8][2] = {
  {1, 12},  // 0
  {14, 31}, // 1
  {33, 36}, // 2
  {38, 46}, // 3
  {47, 61}, // 4
  {63, 65}, // 5
  {67, 69}, // 6
  {70, 73}, // 7
};

// Save status
char lane_stats[8] = { 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B' };
char loc_stats[6] = { 'O', 'O', 'O', 'O', 'O', 'O' };

// Create map
int connmap[6][3][2] {
  {
    {1, 0},  // 0 is connected to 1 via lane 0,
    {3, 3},  // 0 is connected to 3 via lane 3,
    {4, 4}   // 0 is connected to 4 via lane 4,
  },
  {
    {0, 0},  // 1 is connected to 0 via lane 0
    {2, 1}   // 1 is connected to 2 via lane 1
  }, 
  {
    {1, 1}, // 2 is connected to 1 via lane 1
    {3, 2}, // 2 is connected to 3 via lane 2
    {5, 7}  // 2 is connected to 5 via lane 7
  },
  {
    {0, 3}, // 3 is connected to 0 via lane 3
    {2, 2}, // 3 is connected to 2 via lane 2
    {5, 6}  // 3 is connected to 5 via lane 6
   },
  {
    {0, 4}, // 4 is connected to 0 via lane 4
    {5, 5}  // 4 is connected to 5 via lane 5
  },
  {
    {2, 7}, // 5 is connected to 2 via lane 7
    {3, 6}, // 5 is connected to 3 via lane 6
    {4, 5}  // 5 is connected to 4 via lane 5
    }
  };

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    if (DEBUG_LEVEL > 0) {
      // We love debugging
      Serial.begin(9600);  
    }

    if (DEBUG_LEVEL >= 1) {
      Serial.println("Start Test Sequence");
     // changeLedsSteps(CRGB::Red, 0, 74);
     // changeLedsSteps(CRGB::Green, 74, 0);
     // changeLedsSteps(CRGB::Blue, 0, 74);
     // changeLedsSteps(CRGB::Black, 74, 0);

     // changeLedsSteps(CRGB::Green, lane_leds[0][0], lane_leds[0][1]);
     // delay(1000);
     // changeLedsSteps(CRGB::Black, lane_leds[0][0], lane_leds[0][1]);

     // changeLedsSteps(CRGB::Green, lane_leds[1][0], lane_leds[1][1]);
     // delay(1000);
     // changeLedsSteps(CRGB::Black, lane_leds[1][0], lane_leds[1][1]);

     // changeLedsSteps(CRGB::Green, lane_leds[2][0], lane_leds[2][1]);
     // delay(1000);
     // changeLedsSteps(CRGB::Black, lane_leds[2][0], lane_leds[2][1]);

     // changeLedsSteps(CRGB::Green, lane_leds[3][0], lane_leds[3][1]);
     // delay(1000);
     // changeLedsSteps(CRGB::Black, lane_leds[3][0], lane_leds[3][1]);

     // changeLedsSteps(CRGB::Green, lane_leds[4][0], lane_leds[4][1]);
     // delay(1000);
     // changeLedsSteps(CRGB::Black, lane_leds[4][0], lane_leds[4][1]);

     // changeLedsSteps(CRGB::Green, lane_leds[5][0], lane_leds[5][1]);
     // delay(1000);
     // changeLedsSteps(CRGB::Black, lane_leds[5][0], lane_leds[5][1]);

     // changeLedsSteps(CRGB::Green, lane_leds[6][0], lane_leds[6][1]);
     // delay(1000);
     // changeLedsSteps(CRGB::Black, lane_leds[6][0], lane_leds[6][1]);

     // changeLedsSteps(CRGB::Green, lane_leds[7][0], lane_leds[7][1]);
     // delay(1000);
     // changeLedsSteps(CRGB::Black, lane_leds[7][0], lane_leds[7][1]);
    }
}

void clsterm() {
  Serial.write(27);
  Serial.print("[2J");
  Serial.write(27);
  Serial.print("[H");
}

void printmap() {
  Serial.println((String)"");
  Serial.println((String)"    " + loc_stats[1]);
  Serial.println((String)"    |\\");
  Serial.println((String)"    |  \\");
  Serial.println((String)"    |    \\  "+ lane_stats[0]);
  Serial.println((String)"  "+ lane_stats[1] +" |      \\");
  Serial.println((String)"    |        \\");
  Serial.println((String)"    |          \\");
  Serial.println((String)"    |  "+ lane_stats[2] +"         \\");
  Serial.println((String)"    "+ loc_stats[2] +"-----"+ loc_stats[3] +"-------"+ loc_stats[0]);
  Serial.println((String)"    \\     |   "+ lane_stats[3] +"   |");
  Serial.println((String)"    "+ lane_stats[7] +" \\   | "+ lane_stats[6] +"     |");
  Serial.println((String)"        \\ |       |");
  Serial.println((String)"          "+ loc_stats[5] +"       | "+ lane_stats[4] +"");
  Serial.println((String)"            \\     |");
  Serial.println((String)"            "+ lane_stats[5] +" \\   |");
  Serial.println((String)"                \\ |");
  Serial.println((String)"                  " + loc_stats[4]);
}

void flash(int led, CRGB color1, CRGB color2, int flash)
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println((String)"Flash: " + flash + " times on led " + led);
  }
  for (int i = 0; i < flash; i++) {
      leds[led] = color1;
      FastLED.show();
      delay(500);
      leds[led] = color2;
      FastLED.show();
      delay(500);
  }
  leds[led] = color1;
  FastLED.show();
}

// Set leds in a different color one by one
void changeLedsSteps(CRGB color, int startled, int endled)
{ 
  if (startled < endled) {
    for (int i = startled; i <= endled; i++) {
      leds[i] = color;
      FastLED.show();
      delay(40);
    }
  } else {
    for (int i = startled; i >= endled; i--) {
      leds[i] = color;
      FastLED.show();
      delay(40);
    }
  }
  if (DEBUG_LEVEL >= 3) {
    Serial.println((String)"ChangeledsSteps: " + startled + " to " + endled + " in " + color);
  }
}

// Change all leds at once
void changeLeds(CRGB color, int startled, int endled)
{ 
  if (startled < endled) {
    for (int i = startled; i <= endled; i++) {
      leds[i] = color;
    }
  } else {
    for (int i = startled; i >= endled; i--) {
      leds[i] = color;
    }
  }
  FastLED.show();
  if (DEBUG_LEVEL >= 3) {
    Serial.println((String)"changeLeds: " + startled + " to " + endled + " in " + color);
  }
}

// Check if 2 pins are connected
bool checkConnection(int pin1, int pin2) {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, INPUT_PULLUP);
  digitalWrite(pin1, LOW);
  
  if (digitalRead(pin2) == LOW) {
    //digitalWrite(pin1, HIGH);
    if (DEBUG_LEVEL >= 4) {
      Serial.println((String)" Pin " + pin1 + " and pin " + pin2 + " are connected");
    }
    return true;
  } else {
    digitalWrite(connection_pins[pin1], HIGH);
    if (DEBUG_LEVEL >= 4) {
      Serial.println((String)" Pin " + pin1 + " and pin " + pin2 + " are NOT connected");
    }
    return false;
  }
  delay (10);
}

void winner() {
  if (DEBUG_LEVEL == 2) {
    clsterm();
    printmap();
  }
  Serial.println((String)"Winner, winner, chicken dinner!");
  changeLedsSteps(CRGB::Red, 0, 149);
  changeLedsSteps(CRGB::Green, 149, 0);
  changeLedsSteps(CRGB::Blue, 0, 149);
  changeLedsSteps(CRGB::Black, 149, 0);
  solved = true;
}

void checkSolution(int start, int finish, int back) {
  for (int i=0; i<sizeof connmap[start]/sizeof connmap[start][0]; i++) {
       if (connmap[start][i][0] != back) {
       if (DEBUG_LEVEL >= 5) {
         Serial.println((String)"Checking " + connection_pins[start] + " to " + connection_pins[connmap[start][i][0]]);
       }
      if (checkConnection(connection_pins[start], connection_pins[connmap[start][i][0]])) {
        // Its connected
        if (lane_stats[connmap[start][i][1]] == 'B') {
          lane_stats[connmap[start][i][1]] = 'G';
          changeLedsSteps(CRGB::Green, lane_leds[connmap[start][i][1]][0], lane_leds[connmap[start][i][1]][1]);
        }
        // FTW!
        if (connmap[start][i][0] == finish) {
          winner();
        }else {
          checkSolution(connmap[start][i][0], finish, start);
        }
      }
      else {
        // Its not connected
        if ((lane_stats[connmap[start][i][1]] != 'B') && (lane_stats[connmap[start][i][1]] != 'R')) {
          lane_stats[connmap[start][i][1]] = 'B';
          changeLedsSteps(CRGB::Black, lane_leds[connmap[start][i][1]][0], lane_leds[connmap[start][i][1]][1]);
        }
      }
    }
  }
}

void loop()
{
  if (solved == true) {
    if (DEBUG_LEVEL >= 2) {
      Serial.println((String)"Starting new game");
    }

    // A truer random
    randomSeed(analogRead(0));

    changeLeds(CRGB::Black, 149, 0);
    
    // Keep generating numbers until 3 different numbers are generated
    while ((start == end) || (start == broken) || (broken == end)) {
      for(int i = 0; i<6; i++){
        leds[loc_leds[i]] = CRGB::Green;
      }
      FastLED.show();
  
      start = random(0,5);
      end = random(0,5);
      broken = random(0,5);
    }
    if (DEBUG_LEVEL >= 2) {
      Serial.println((String)"Start = " + start + ", End = " + end + ", Broken = " + broken);
      Serial.println((String)"Flashing locations");
    }
    
    // Flash locations
    if (DEBUG_LEVEL >= 2) {
      Serial.println((String)"Flashing Start");
    }
    loc_stats[start] = 'S';
    flash(loc_leds[start], CRGB::Blue, CRGB::Black, 3);

    if (DEBUG_LEVEL >= 2) {
      Serial.println((String)"Flashing Finish");
    }
    loc_stats[end] = 'F';
    flash(loc_leds[end], CRGB::Blue, CRGB::Black, 3);
    loc_stats[broken] = 'X';
    if (DEBUG_LEVEL >= 2) {
      Serial.println((String)"Flashing Broken");
    }
    flash(loc_leds[broken], CRGB::Red, CRGB::Black, 3);

    if (DEBUG_LEVEL >= 2) {
      Serial.println((String)"Breaking lanes");
    }
    for (int i=0; i<sizeof connmap[broken]/sizeof connmap[broken][0]; i++) {
      lane_stats[connmap[broken][i][1]] = 'R';
      changeLedsSteps(CRGB::Red, lane_leds[connmap[broken][i][1]][0], lane_leds[connmap[broken][i][1]][1]);
    }

    // Reset game
    solved = false;
  } 
  checkSolution(start, end, broken);

  if (DEBUG_LEVEL == 2) {
    clsterm();
    printmap();
    delay(500);
  }
}