const int aPin = 0;
const int rPin = 9;
const int gPin = 8;
const int bPin = 7;
String temperatures[2];
float averageReading = 0;
float voltReading;
int count = 0;

String Format(String input, int count, String outputParams[])
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

void setup()
{
	pinMode(rPin, OUTPUT);
	pinMode(gPin, OUTPUT);
	pinMode(bPin, OUTPUT);
	Serial.begin(9600);
}

void loop()
{
        voltReading = analogRead(aPin);
        
        if (count >= 10000) {
          count = 0;
          averageReading = 0;
        }
        
        averageReading = ((averageReading * count + voltReading) / ++count);
                 
        if (voltReading > averageReading) {
          analogWrite(rPin, 0);
          analogWrite(gPin, 255);
          analogWrite(bPin, 255);
        }
        else {
          analogWrite(rPin, 255);
          analogWrite(gPin, 255);
          analogWrite(bPin, 0);
        }
}
