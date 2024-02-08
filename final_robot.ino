int trigPin = 12;  // Ultrasonic sensor pins
int echoPin = 11;
int buzzerPin = 12;  // Buzzer pin
int enA = A0; // Enable pin for Motor 1
int motor1pin1 = 9;
int motor1pin2 = 10;
int enB = A1; // Enable pin for Motor B
int motor2pin1 = 8;
int motor2pin2 = 7;
long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
}

void loop() {
  // Ultrasonic sensor logic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Move the robot based on commands received from Python
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'F') {
      if (dist()) {
        stop();
      } else {
        forward();
        delay(500);
        stop();
      
      }
    } else if (command == 'R') {
      if (dist()) {
        stop();
        delay(500);
      } else {
        right();
        delay(500);
        stop();
        }
    } else if (command == 'S') {
      stop();
    }
  }

  // Ultrasonic sensor triggers the buzzer if an object is within 10cm
  if (distance < 10) {
    digitalWrite(buzzerPin, HIGH);
    stop();
  } else {
    digitalWrite(buzzerPin, LOW);
  }
}

void forward() {
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor1pin1, LOW);
  analogWrite(enA, 70);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  analogWrite(enB, 70);
}
void right() {
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  analogWrite(enA, 70);
  analogWrite(enB, 70);
}

void stop() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

bool dist() {
  return (distance < 10);
}
