void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorInput = analogRead(0);

  Serial.print("Input value ");
  Serial.print(sensorInput);

  sensorInput = map(sensorInput, 274, 130, 0, 90);
  Serial.print(", mapped to ");
  Serial.println(sensorInput);
}
