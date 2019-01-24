#define MainPeriod 10000
#define NumOfSamples 10
long previousMillis = 0;
volatile unsigned long duration = 0;
volatile unsigned int pulseCount = 0;
volatile unsigned long previousMicros = 0;



void setup() {
  Serial.begin(19200);
  pinMode(3, INPUT);
  attachInterrupt(digitalPinToInterrupt(3), myIntHandler, RISING);
}


void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= MainPeriod) {
    // bufferize to avoid glitches
    unsigned long _duration = duration;
    unsigned long _pulseCount = pulseCount;

    // clear counters
    duration = 0;
    pulseCount = 0;

    float Freq = 1e6 / float(_duration);
    Freq *= 60;
    Freq *= _pulseCount;


    // check if the value is not NaN or INF
    if (isnan(Freq)||isinf(Freq)) {
      return false;
    }
    // check if value is too large or small
    if ((Freq > 3000)||(Freq < 2)) {
      return false;
    }

    Serial.print(Freq);
    Serial.println(" RPM");
  }
}


void myIntHandler() {
  unsigned long currentMicros = micros();

  duration += currentMicros - previousMicros;
  previousMicros = currentMicros;
  pulseCount++;
}
