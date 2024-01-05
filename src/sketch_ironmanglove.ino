/*----------------------------------------------------------------------------------------
Iron Man Glove - Arduino Code
Tahseen Rayhan; Ali Khan
DESCRIPTION:
This code is made for the exclusive Arduino "Iron Man Glove," made by Tahseen
Rayhan & Ali Khan. To create the toy that we did, we used a Neopixel Ring,
pushbuttons, and LEDs to bring Iron Man to life and let everyone that uses this toy be
able to take on the role of one of their favorite Marvel superheroes. Various commands,
functions, loops, and other code was used to program the various components that were
used, while it's made sure that certain actions are performed after certain conditions
were met (eg; pressing a pushbutton to initiate the "blaster-effect" or the pushbutton
that turns on the "laser" LEDs). Comments below help explain what each part of the code
is responsible for.
    -----------------------------------------------------------------------------------------*/
    // CONSTANTS
#include <Adafruit_NeoPixel.h>
    //Configuring the NeoPixel, Pushbuttons, LEDS, and initializing variables
#define NEOPIXEL 11
#define NUM_LEDS 16
    const int PB = 2, PBS = 13;
const int LED1 = 7, LED2 = 12;
int num, prev_num = 0;
int brightness_off = 0, maxbrightness = 20;
int pos = 0, dir = 1; // Position and direction of the Iron Man blaster
int default_delay = 2; // Many commands/functions use a delay of 2ms
// Initializing NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL, NEO_GRB);
//-------------------------------------------------------------------------------------
// FUNCTIONS
void neopixel_startup(int init_br, int final_br, int del) {
  /* Turns on neopixel LEDs using a pattern and parameters.
    Initial brightness, final brightness, and delay is passed in; Function
    goes through the individual 16 LEDs and turns them on, one-by-one. Each
    LED fades from the init. brightness to the final brightness */
  for (int j = 0; j < NUM_LEDS ; j++) {
    for (int i = init_br; i < final_br; i ++) {
      strip.setPixelColor(j, 0, 0, i);
      strip.show();
      delay(del);
    }
  }
}
// -------------------------------------------------------------
void lightningcrash() {
  // Function for the flash sequence of the palm blaster
  for (int i = 0; i <= strip.numPixels(); i++) { // Turns all LEDs to blue first
    strip.setPixelColor(i, 0, 0, 255);
  }
  strip.show();
  // Causes the NeoPixel to flash off and on
  delay(15);
  strip.setBrightness(brightness_off);
  strip.show();
  strip.setBrightness(maxbrightness);
  delay(250);
  for (int times = 0; times < 4; times++) { // Rapid blast start
    for (int i = 0; i <= strip.numPixels(); i++) { // Turning all LEDS to white
      strip.setPixelColor(i, 255, 255, 255);
    }
    digitalWrite(NEOPIXEL, HIGH); // flashing effect similar to Ln. 77
    strip.show();
    delay(50);
    digitalWrite(NEOPIXEL, LOW);
    strip.setBrightness(brightness_off);
    strip.show();
    strip.setBrightness(maxbrightness);
    delay(50);
  }
  // Random color and decay sequence
  for (int i = 0; i <= NUM_LEDS; i++) { // For loop chooses and assigns random RGB value
    int red = rand() % 255; // to each NeoPixel LED
    int gre = rand() % 255;
    int blu = rand() % 255;
    strip.setPixelColor(i, strip.Color(red, gre, blu));
  }
  strip.show();
  // Decay the brightness gradually
  for (int i = 255; i >= 0; i--) { // for loop gradually "decays" the brightness for all LEDs
    strip.setBrightness(i);
    strip.show();
    delay(default_delay); // default delay of 2ms
  }
  delay(10);
  strip.setBrightness(maxbrightness); // sets the brightness back to the specified brightness
  digitalWrite(NEOPIXEL, LOW); // after the animation is finished
}
// -------------------------------------------------------------
void blast_animation() {
  // Set specific colors for the "eye" effect
  strip.setPixelColor(pos - 2, 5, 5, 5);
  strip.setPixelColor(pos - 1, 25, 25, 100);
  strip.setPixelColor(pos, 0, 0, 255);
  strip.setPixelColor(pos + 1, 25, 25, 100);
  strip.setPixelColor(pos + 2, 5, 5, 5);
  strip.show();
  delay(20);
  // Clear the previous "eye" effect above
  for (int j = -2; j <= 2; j++) {
    strip.setPixelColor(pos + j, 0);
  }
  // Bounce off the ends of the LED strip
  pos += dir;
  if (pos < 0) { // Execute lightning crash function above (Ln. 68)
    lightningcrash(); // if current position is less than 0
    pos = 1;
    dir = -dir;
  } else if (pos >= NUM_LEDS) {
    pos = NUM_LEDS - 2;
    dir = -dir;
  }
}
// Function for the starting sequence
void starting_sequence() {
  /*Function for the actual starting sequence. As soon as the Arduino is
    powered, this is the startup animation that the user first sees*/
  /* Using function from Ln. 47 - discluded from the for loop as the delays specified
    are different from the default delay (2 ms)*/
  neopixel_startup(0, 5, 4);
  neopixel_startup(5, 10, 3);
  for (int br = 10; br < 41; br += 5) { // Starts from brightness of 10 incrementing by 5
    neopixel_startup(br, br + 5, default_delay); // and stops at 40; end brightness is 5 more
  } // since brightness goes up by 5 each time
  neopixel_startup(45, 101, default_delay); // fades from brightness(45) to brightness(100)
}
//-------------------------------------------------------------------------------------
// ACTUAL CODE
void setup() {
  Serial.begin(9600);
  strip.begin(); // initializing NeoPixel
  strip.show();
  starting_sequence(); // running the starting sequence
  // Fade in from 0 to 100 brightness level
  for (int i = 0; i < 101; i++) {
    for (int j = 0; j < NUM_LEDS ; j++) {
      strip.setPixelColor(j, i, i, 100); // for-loop causes color to change from
    } // blue to white
    strip.show();
    delay(1);
  }
  delay(500);
  strip.setBrightness(maxbrightness); // Setting the maximum brightness level
  strip.show();
  for (int i = 100; i > -1; i--) { // Fading from 100 to 0 for the R and G values
    for (int j = 0; j < NUM_LEDS ; j++) { // to get blue
      strip.setPixelColor(j, i, i, 100);
    }
    strip.show();
    delay(1);
  }
}
void loop() {
  while (digitalRead(PBS) == 1) { // while the pushbutton is held, it executes the
    blast_animation(); // blast animation function
  }
  if (digitalRead(PBS) == 0) { // If it's not pressed, an idle flashing animation runs
    for (int i = 0; i < NUM_LEDS ; i++) {
      strip.setPixelColor(i, 100, 100, 100); // sets all LEDS to grey/white
      strip.show();
    }
    for (int i = 100; i > -1; i--) { // then, the R and G values decreases to get blue
      for (int j = 0; j < NUM_LEDS; j++) {
        strip.setPixelColor(j, i, i, 100);
      }
      strip.show();
      delay(1);
    }
  }
  while (digitalRead(PB) == 1) { // pushbutton which enables the "laser"
    analogWrite(LED1, 255); // two leds are turned on and off for the laser beam effect
    analogWrite(LED2, 255);
    delay(40);
    analogWrite(LED1, 0);
    analogWrite(LED2, 0);
    delay(40);
  }
}