String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete


void setup() {
  // initialize serial:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
    // print the string when a newline arrives:
  if (stringComplete) {
    CheckAndProcessPinMode(inputString);
    CheckAndProcessDigitalWrite(inputString);
    CheckAndProcessAnalogWrite(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

}

void CheckAndProcessPinMode(String string){
  int pin = 0;
  String mode = "";
  String str;

  if(getCommandData(string, "pinMode", pin, mode) == true){
    if(mode.equals("OUTPUT")){
      pinMode(pin,OUTPUT);
      Serial.println(str + "Pin " + pin + " is set as OUTPUT");
    }
    else if(mode.equals("INPUT")){
      pinMode(pin,INPUT);
      Serial.println(str + "Pin " + pin + " is set as INPUT");
      
    }
    else{
      Serial.println("Port Mode is not matching please use INPUT or OUTPUT");
    }
  }
}

void CheckAndProcessDigitalWrite(String string){
  int pin = 0;
  String val = "";
  String str;

  if(getCommandData(string, "digitalWrite", pin, val) == true){
    if(val.equals("HIGH")){
      digitalWrite(pin,HIGH);
      Serial.println(str + "Pin " + pin + " is set to HIGH");
    }
    else if(val.equals("LOW")){
      digitalWrite(pin,LOW);
      Serial.println(str + "Pin " + pin + " is set to LOW");
    }
    else{
      Serial.println("Digital Value is not matching please use HIGH or LOW");
    }
  }
}

void CheckAndProcessAnalogWrite(String string){
  int pin = 0;
  int analogVal = 0;
  String val = "";
  String str;

  if(getCommandData(string, "analogWrite", pin, val) == true){
    analogVal = val.toInt();
    Serial.println(analogVal);
    if((analogVal >= 0)||(analogVal <= 255)){
      analogWrite(pin,analogVal);
      Serial.println(str + "Pin " + pin + " is set to a duty of " + analogVal + "/255");
    }
    else{
      Serial.println("Analog Value is not matching please use 0-255");
    }
  }
}

bool getCommandData(String string, String command, int& pin, String& mode){
  int positionCommer = 0;
  int positionBracketOpen = 0;
  int positionBracketClose = 0;

  if(string.indexOf(command) == -1) return false;

  positionCommer = string.indexOf(",");
  if(positionCommer == -1) return false;

  positionBracketOpen = string.indexOf("(");
  if(positionBracketOpen == -1) return false;

  positionBracketClose = string.indexOf(")");
  if(positionBracketClose == -1) return false;

  pin = string.substring(positionBracketOpen +1  , positionCommer).toInt();
  mode  = string.substring(positionCommer +1 , positionBracketClose);
  return true;
}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
