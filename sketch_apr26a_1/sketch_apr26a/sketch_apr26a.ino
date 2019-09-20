const int ledPin = 13;
const int button1Pin = 2;
const int button2Pin = 3;

void setup() {
	pinMode(ledPin, OUTPUT);
	pinMode(button1Pin, OUTPUT);
	pinMode(button2Pin, OUTPUT);
}

void loop() {
	bool pin1, pin2;
	
	//pin1 = (digitalRead(button1Pin) == LOW);
	//pin2 = (digitalRead(button2Pin) == LOW);
	
	if (pin1 ^ pin2)
	{
		digitalWrite(ledPin, HIGH);
	}
}
