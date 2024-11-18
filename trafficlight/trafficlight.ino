#define RED_LED_PIN 7
#define YELLOW_LED_PIN 5
#define GREEN_LED_PIN 3

#define STATE_RED 0
#define STATE_YELLOW 1
#define STATE_GREEN 2
#define STATE_GREEN_BLINK 3

#define RED_DURATION 10000      // время работы красного света в миллисекундах
#define YELLOW_DURATION 1500    // время работы желтого света в миллисекундах
#define GREEN_DURATION 10000     // время работы зеленого света в миллисекундах
#define BLINK_DURATION 3000      // время мигания зеленого света в миллисекундах

int state = STATE_RED;
long int state_start;
bool manual_mode = false;

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  
  // Начинаем с красного света
  digitalWrite(RED_LED_PIN, HIGH);
  state_start = millis();
}

void update_state() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    if (cmd == 'm') { // Переключение на ручной режим
      manual_mode = !manual_mode;
      Serial.println(manual_mode ? "Manual mode ON" : "Manual mode OFF");
    }
    
    if (manual_mode) {
      if (cmd == 'r') {
        state = STATE_RED;
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        state_start = millis();
      }
      else if (cmd == 'y') {
        state = STATE_YELLOW;
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(YELLOW_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);
        state_start = millis();
      }
      else if (cmd == 'g') {
        state = STATE_GREEN;
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        state_start = millis();
      }
      else {
        Serial.println("Unknown command");
      }
    }
  }
}

void process_state() {
  if (!manual_mode) {
    long int current_time = millis();
    
    if (state == STATE_RED && current_time - state_start >= RED_DURATION) {
      state = STATE_GREEN;
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, HIGH);
      state_start = current_time;
    }
    else if (state == STATE_GREEN) {
      if (current_time - state_start >= GREEN_DURATION - BLINK_DURATION) {
        // Проверяем, пришло ли время мигания
        int blink_time = (current_time - state_start) % 500; // мигаем каждые 500 мс
        if (blink_time < 250) {
          digitalWrite(GREEN_LED_PIN, HIGH); // Включаем зеленый свет
        } else {
          digitalWrite(GREEN_LED_PIN, LOW); // Выключаем зеленый свет
        }

        // Если прошло достаточно времени, переключаем на желтый
        if (current_time - state_start >= GREEN_DURATION) {
          state = STATE_YELLOW;
          digitalWrite(GREEN_LED_PIN, LOW);
          digitalWrite(YELLOW_LED_PIN, HIGH);
          state_start = current_time;
        }
      }
    }
    else if (state == STATE_YELLOW && current_time - state_start >= YELLOW_DURATION) {
      state = STATE_RED;
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);
      state_start = current_time;
    }
  }
}

void loop() {
  update_state();
  process_state();
}
