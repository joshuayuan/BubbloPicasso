
/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#include <Servo.h> 
Servo myservo; // create servo object to control a servo
Servo myservo1;

boolean bubbling = true;
int pos = 0; // variable to store the servo position

//MUSIC SECTION
// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

// These are the pins used
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)

// Feather M4 M0, 328, or 32u4
#define VS1053_CS       6     // VS1053 chip select pin (output)
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define CARDCS          5     // Card chip select pin
// DREQ should be an Int pin *if possible* (not possible on 32u4)
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);


/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines
/*
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(Serial1, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

boolean musicdone = false;
/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void)
{
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command Mode Example"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  ble.sendCommandCheckOK(F("AT+GAPDEVNAME=A Rashad and Debbie's father"));
  /* Reset the device for the new service setting changes to take effect */ 
  //Serial.print(F("Performing a SW reset (service changes a reset): "));
  ble.reset();

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("******************************"));
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    Serial.println(F("******************************"));
  }
  myservo.attach(11); // attaches the servo on pin 11 to the servo object
  myservo1.attach(10); // attaches the servo on pin 11 to the servo object


  pinMode(1, INPUT_PULLUP);
  digitalWrite(1, HIGH);

  // Wait for serial port to be opened, remove this line for 'standalone' operation
  while (!Serial) { delay(1); }

  Serial.println("\n\nAdafruit VS1053 Feather Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }

  Serial.println(F("VS1053 found"));
  musicPlayer.sineTest(0x44, 500); // Make a tone to indicate VS1053 is working

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1); // don't do anything more
  }
  Serial.println("SD OK!");

  // list files
  Serial.println("This is the directory");
  printDirectory(SD.open("/"), 0);
  Serial.println("End of directory");

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(10, 10);

#if defined(__AVR_ATmega32U4__) 
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  // but we don't have them on the 32u4 feather...
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int
#elif defined(ESP32)
  // no IRQ! doesn't work yet :/
#else
  // If DREQ is on an interrupt pin we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
#endif
  // Play a file in the background, REQUIRES interrupts!
  Serial.println(F("Playing full track 001"));
  musicPlayer.startPlayingFile("track001.mp3");
}



/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  if (musicPlayer.stopped()) {
    Serial.println("Done playing music");
  }
  Serial.print("loop");
  // Check for user input
  char inputs[BUFSIZE+1];
  if ( getUserInput(inputs, BUFSIZE) )
  {
    // Send characters to Bluefruit
    Serial.print("[Send] ");
    Serial.println(inputs);

    ble.print("AT+BLEUARTTX=");
    ble.println(inputs);

    // check response stastus
    if (!ble.waitForOK() ) {
      Serial.println(F("Failed to send?"));
    }
  }
  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();

  // Some data was found, its in the buffer
  Serial.print(F("[Recv] ")); Serial.println(ble.buffer);

  //if buffer input is 's', turn on servo 
  if (strcmp(ble.buffer, "s") == 0) {
    Serial.println("read s");
    musicPlayer.pausePlaying(false);
    processCommand("s");
  } else if (strcmp(ble.buffer, "e") == 0) {
    musicPlayer.pausePlaying(true);
    processCommand("e");
  }
  bubblingUpdate();
  ble.waitForOK();
}


void processCommand(String command)
{
    if(command == "s"){
        bubbling = true;
    } else {
        bubbling = false;
    }
}
void bubblingUpdate() {
  if (bubbling == true) {
    bubbleOn();
  } else {
    bubbleOff();
  }
}

void bubbleOn() {
  // 200 is towards the handle, 0 is pointing out away.
  for (pos = 200; pos >= 0; pos -= 1) { // goes from a bit past 180 degrees to 0 degrees
    myservo.write(pos);
    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 0; pos <= 200; pos += 1) { // goes from 0 degrees to a little further than 180 degrees in steps of 1 degree
    myservo.write(pos);
    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void bubbleOff() {
  int i = 0;
  for (i = pos; i < 200; i += 1) {
    myservo.write(i);
    myservo1.write(i);
    delay(15);
  }
  pos = i;
}


/**************************************************************************/
/*!
    @brief  Checks for user input (via the Serial Monitor)
*/
/**************************************************************************/
bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while( (!Serial.available()) && !timeout.expired() ) { delay(1); }

  if ( timeout.expired() ) return false;

  delay(2);
  uint8_t count=0;
  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && (Serial.available()) );

  return true;
}

/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
