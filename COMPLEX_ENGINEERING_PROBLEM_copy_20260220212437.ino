// -------- PIN DEFINITIONS --------

// Ultrasonic Sensors
#define trig1 13
#define echo1 14
#define trig2 12
#define echo2 18

// IR Sensors
#define IR1 25
#define IR2 26

// Emergency IR (PIN 32)
#define EMERGENCY_IR 32

// Pedestrian Button (Moved to safe pin)
#define PED_BUTTON 27

// Traffic Light Lane 1
#define R1 21
#define Y1 22
#define G1 23

// Traffic Light Lane 2
#define R2 19
#define Y2 4
#define G2 2

unsigned long previousMillis = 0;
unsigned long interval = 0;

int state = 0;
int currentLane = -1;

unsigned long lastPedPress = 0;
const unsigned long debounceDelay = 1000;

void allRed() {
  digitalWrite(R1, HIGH);
  digitalWrite(R2, HIGH);
  digitalWrite(G1, LOW);
  digitalWrite(G2, LOW);
  digitalWrite(Y1, LOW);
  digitalWrite(Y2, LOW);
}

int readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 25000);
  int distance = duration * 0.034 / 2;
  return distance;
}

void setup() {

  Serial.begin(115200);

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  pinMode(EMERGENCY_IR, INPUT);
  pinMode(PED_BUTTON, INPUT_PULLUP);

  pinMode(R1, OUTPUT);
  pinMode(Y1, OUTPUT);
  pinMode(G1, OUTPUT);

  pinMode(R2, OUTPUT);
  pinMode(Y2, OUTPUT);
  pinMode(G2, OUTPUT);

  allRed();

  Serial.println("===== 2-WAY ADAPTIVE TRAFFIC SYSTEM =====");
}

void loop() {

  unsigned long currentMillis = millis();

  // ================= EMERGENCY =================
  if (digitalRead(EMERGENCY_IR) == LOW) {

    Serial.println("🚑 EMERGENCY VEHICLE DETECTED");
    Serial.println("Lane 1 GREEN (Emergency)");
    Serial.println("Lane 2 RED");

    allRed();
    digitalWrite(R1, LOW);
    digitalWrite(G1, HIGH);

    delay(10000);

    digitalWrite(G1, LOW);
    digitalWrite(R1, HIGH);
    return;
  }

  // ================= PEDESTRIAN =================
  if (digitalRead(PED_BUTTON) == LOW) {

    if (currentMillis - lastPedPress > debounceDelay) {

      Serial.println("🚶 Pedestrian Crossing Activated");
      Serial.println("Both Lanes RED");

      allRed();
      delay(5000);

      lastPedPress = currentMillis;
    }
  }

  // ================= STATE MACHINE =================
  if (state == 1 && currentMillis - previousMillis >= interval) {

    if (currentLane == 1) {
      digitalWrite(G1, LOW);
      digitalWrite(Y1, HIGH);
      Serial.println("Lane 1 YELLOW");
    } else {
      digitalWrite(G2, LOW);
      digitalWrite(Y2, HIGH);
      Serial.println("Lane 2 YELLOW");
    }

    previousMillis = currentMillis;
    interval = 3000;
    state = 2;
  }

  else if (state == 2 && currentMillis - previousMillis >= interval) {

    if (currentLane == 1) {
      digitalWrite(Y1, LOW);
      digitalWrite(R1, HIGH);
      Serial.println("Lane 1 RED");
    } else {
      digitalWrite(Y2, LOW);
      digitalWrite(R2, HIGH);
      Serial.println("Lane 2 RED");
    }

    state = 0;
  }

  // ================= ADAPTIVE MODE =================
  if (state == 0) {

    int selectedLane = -1;
    int densityScore1 = 0;
    int densityScore2 = 0;

    int distance1 = 0;
    int distance2 = 0;

    if (digitalRead(IR1) == LOW) {

      distance1 = readUltrasonic(trig1, echo1);

      if (distance1 > 5 && distance1 < 20)
        densityScore1 = 3;
      else if (distance1 < 40)
        densityScore1 = 2;
      else
        densityScore1 = 1;
    }

    if (digitalRead(IR2) == LOW) {

      distance2 = readUltrasonic(trig2, echo2);

      if (distance2 > 5 && distance2 < 20)
        densityScore2 = 3;
      else if (distance2 < 40)
        densityScore2 = 2;
      else
        densityScore2 = 1;
    }

    if (densityScore1 > densityScore2)
      selectedLane = 1;
    else if (densityScore2 > densityScore1)
      selectedLane = 2;
    else
      return;

    int selectedDensity = (selectedLane == 1) ? densityScore1 : densityScore2;

    int greenTime;

    if (selectedDensity == 3) {
      greenTime = 20000;
      Serial.println("Traffic Level: HIGH");
    }
    else if (selectedDensity == 2) {
      greenTime = 12000;
      Serial.println("Traffic Level: MEDIUM");
    }
    else {
      greenTime = 6000;
      Serial.println("Traffic Level: LOW");
    }

    allRed();

    if (selectedLane == 1) {
      digitalWrite(R1, LOW);
      digitalWrite(G1, HIGH);
      Serial.println("Lane 1 GREEN");
      Serial.println("Lane 2 RED");
    } else {
      digitalWrite(R2, LOW);
      digitalWrite(G2, HIGH);
      Serial.println("Lane 2 GREEN");
      Serial.println("Lane 1 RED");
    }

    currentLane = selectedLane;
    previousMillis = currentMillis;
    interval = greenTime;
    state = 1;
  }
}