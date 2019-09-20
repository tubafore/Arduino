const int rPin = 9;
const int gPin = 8; 
const int bPin = 7;

const int readPin = 0;

int lightLevel;
int rgb[3];

String Format(String input, int count, int outputParams[])
{
  String result = String(input), temp = String("{X}");
  
  for (int i = 0; i < count; i++)
  {
    temp.replace("X", String(i));
    result.replace(temp, String(outputParams[i]));
    temp.replace(String(i), "X");
  } 
  return result;
}

void setup() {
	pinMode(rPin, OUTPUT);
        pinMode(gPin, OUTPUT);
        pinMode(bPin, OUTPUT);
        Serial.begin(9600);
}

void loop() {
	lightLevel = analogRead(readPin);
	
//	lightLevel = map(lightLevel, 0, 1023, 0, 255);
//	lightLevel = constrain(lightLevel, 0, 255);
  LightIt(1024 - lightLevel);
}

void LightIt(int lightLevel)
{
  if (lightLevel < 600)
  {
    rgb[0] = 0;
    rgb[1] = 255;
    rgb[2] = 255;
  }
  else if (lightLevel <= 700)
  {
    rgb[0] = 255;
    rgb[1] = 0;
    rgb[2] = 255;
  }
  else if (lightLevel <= 800)
  {
    rgb[0] = 255;
    rgb[1] = 255;
    rgb[2] = 0;
  }
  else
  {
    rgb[0] = 0;
    rgb[1] = 255;
    rgb[2] = 0;
  }
    
  Serial.println(Format("Converted color values: R = {0}, G = {1}, B = {2}", 3, rgb));
  analogWrite(rPin, rgb[0]);
  analogWrite(gPin, rgb[1]);
  analogWrite(bPin, rgb[2]);
}
