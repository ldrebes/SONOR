#include <ArduinoBLE.h>
#include <Servo.h>

BLEService newService("180A");

BLEBoolCharacteristic switchPos("2A56", BLERead | BLENotify); //Create digital value
BLEIntCharacteristic inSignal("2A58", BLERead | BLENotify);   //Create analog read value

const int ledPin = 1;
long previousMillis = 0;

Servo myservo;      //servo object
int pos = 0;        //variable for servo position


void setup() {
  Serial.begin(9600);
  //while(!Serial);        //wait for serial monitor to open

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  myservo.attach(6);  

  pinMode(LED_BUILTIN, OUTPUT);   //LED to blink when connected
  pinMode(ledPin, OUTPUT);        //LED to blink when connected
  
  if (!BLE.begin()) {
    Serial.println("starting BLE failed");
    while(1);
  }
  BLE.setLocalName("MKR 1010");
  BLE.setAdvertisedService(newService);

  newService.addCharacteristic(switchPos);
  newService.addCharacteristic(inSignal);

  BLE.addService(newService);

  switchPos.writeValue(0);
  inSignal.writeValue(0);

  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void servoctrl() {

  int switchState1 = !digitalRead(0);
  int switchState2 = !digitalRead(1);
  int switchState3 = !digitalRead(2);
  int switchState4 = !digitalRead(3);
  int switchState5 = !digitalRead(4);
  int switchState6 = !digitalRead(5); 
  
  //no switch state active
  if (switchState1 == LOW && switchState2 == LOW && switchState3 == LOW && switchState4 == LOW && switchState5 == LOW && switchState6 == LOW) { 
    myservo.attach(6);
    pos = 0;
    myservo.write(pos);
    delay(15);
    myservo.detach();
  }
  
  //switchState1 = 1
  if (switchState2 == LOW && switchState3 == LOW && switchState4 == LOW && switchState5 == LOW && switchState6 == LOW) { 
    myservo.attach(6);
    pos = 30;
    myservo.write(pos);
    delay(15);
    myservo.detach();
  }

  //switchState1 = 2
  if (switchState1 == LOW && switchState3 == LOW && switchState4 == LOW && switchState5 == LOW && switchState6 == LOW) {
    myservo.attach(6);
    pos = 60;
    myservo.write(pos);
    delay(15);
    myservo.detach();
  }
  
  //switchState1 = 3
  if (switchState1 == LOW && switchState2 == LOW && switchState4 == LOW && switchState5 == LOW && switchState6 == LOW) {
    myservo.attach(6);
    pos = 90;
    myservo.write(pos);
    delay(15);
    myservo.detach();
  } 
  
  //switchState1 = 4
  if (switchState1 == LOW && switchState2 == LOW && switchState3 == LOW && switchState5 == LOW && switchState6 == LOW) {
    myservo.attach(6);
    pos = 120;
    myservo.write(pos);
    delay(15);
    myservo.detach();
  }

  //switchState1 = 5
  if (switchState1 == LOW && switchState2 == LOW && switchState3 == LOW && switchState4 == LOW && switchState6 == LOW) {
    myservo.attach(6);
    pos = 150;
    myservo.write(pos);
    delay(15);
    myservo.detach();
  }

  //switchState1 = 6
  if (switchState1 == LOW && switchState2 == LOW && switchState3 == LOW && switchState4 == LOW && switchState5 == LOW) {
    myservo.attach(6);
    pos = 180;
    myservo.write(pos);
    delay(15);
    myservo.detach();
  }
}

void loop() {
  BLEDevice central = BLE.central(); // wait for a BLE central
  
  if (central) {  // if a central is connected to the peripheral

    Serial.print("Connected to central: ");
    
    Serial.println(central.address()); // print the central's BT address
    
    digitalWrite(LED_BUILTIN, HIGH); // turn on the LED to indicate the connection
    
    while (central.connected()) {     // while the central is connected:
      long currentMillis = millis(); 
      servoctrl();
       
      if(currentMillis - previousMillis >= 50) {        //if 50ms have passed we check the battery level
        previousMillis = currentMillis;

        int posValue = !digitalRead(2);             //gets data from digital input
        switchPos.writeValue(posValue);

        int signalValue = analogRead(A0);           //gets anaolg signal data 
        
        
        
        
        int instantVoltage = signalValue * (5 / 1023);     //converts to voltage
        inSignal.writeValue(instantVoltage);
      }
    }
    
    digitalWrite(LED_BUILTIN, LOW);   //turn off LED when not connected
    Serial.print("Disconnected from central: ");
    Serial.print(central.address());

  }
}
