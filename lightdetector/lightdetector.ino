const int photoResistor1 = A0;
const int photoResistor2 = A1;
const int led1 = 2;
const int led2 = 3;
const int threshold = 50;  

unsigned long previousMillis = 0;
const long interval = 500;  

bool led1State = LOW;
bool led2State = LOW;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  int lightValue1 = analogRead(photoResistor1);
  int lightValue2 = analogRead(photoResistor2);

  unsigned long currentMillis = millis();

  if (abs(lightValue1 - lightValue2) <= threshold) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  } else {
    if (lightValue1 > lightValue2) {
      // Светодиод 1 мигает
      digitalWrite(led2, LOW);  
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        led1State = !led1State;  
        digitalWrite(led1, led1State);
      }
    } else {
      digitalWrite(led1, LOW);  
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        led2State = !led2State;  
        digitalWrite(led2, led2State);
      }
    }
  }
}
