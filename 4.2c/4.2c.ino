#define BUTTON_PIN 2
#define TRIG_PIN 4
#define ECHO_PIN 3
#define LED1 8
#define LED2 9

volatile bool led1State = LOW;

void buttonISR() {
  led1State = !led1State;
}

void setupPins() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void setupInterrupts() {
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
}

void setup() {
  Serial.begin(9600);
  setupPins();
  setupInterrupts();
  Serial.println("System Ready");
}

void loop() {
  digitalWrite(LED1, led1State);

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = (duration * 34) / 2000;  

  Serial.print("Distance: ");
  Serial.println(distance);

  digitalWrite(LED2, (distance > 0 && distance < 10) ? HIGH : LOW);

  delay(200);
}
