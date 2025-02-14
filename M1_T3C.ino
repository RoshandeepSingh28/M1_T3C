const int PIR_PIN = 2;          // PIR sensor input pin 
const int LED_MOTION = 7;       // LED for motion indication
const int TRIG_PIN = 3;         // Ultrasonic trigger pin
const int ECHO_PIN = 4;         // Ultrasonic echo pin
const int LED_DISTANCE = 8;     // LED for distance indication

volatile bool motionFlag = false;

void setup() {
    pinMode(PIR_PIN, INPUT);
    pinMode(LED_MOTION, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_DISTANCE, OUTPUT);

    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(PIR_PIN), handleMotion, RISING);
}

void loop() {
    if (!motionFlag) {
        long distance = measureDistance();
        handleDistanceLED(distance);
        delay(500);
    }
}

long measureDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH);
    return (duration / 2) / 29.1; // Convert to centimeters
}

void handleDistanceLED(long distance) {
    if (distance < 50) {
        digitalWrite(LED_DISTANCE, HIGH);
        Serial.println("Ultrasonic Sensor: Object detected!");
    } else if (distance > 50){
        digitalWrite(LED_DISTANCE, LOW);
    }
}

void handleMotion() {
    motionFlag = true;
    Serial.println("PIR Sensor: Motion Detected! Stopping all activities.");
    digitalWrite(LED_MOTION, HIGH);
    while (digitalRead(PIR_PIN) == HIGH); // Halt execution until motion stops
    Serial.println("PIR Sensor: Motion Ended! Resuming activities.");
    digitalWrite(LED_MOTION, LOW);
    motionFlag = false;
}
