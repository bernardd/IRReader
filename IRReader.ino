/*
  
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

#define L 400
#define IR 2

unsigned int data[L];
unsigned int gaps[3];

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  pinMode(IR, INPUT_PULLUP);
  Serial.begin(115200);
  delay(2000);
  Serial.println("Initialised");
}

// the loop routine runs over and over again forever:
void loop() {
  for (int i=0; i<L; i++)
    data[i] = 0;
  for (int i=0; i<3; i++)
    gaps[i] = 0;

  unsigned long t=1;
  unsigned int pos = 0;
  unsigned int gap = 0;
  while (digitalRead(IR)) {}
  
  digitalWrite(led, HIGH);
  while (t && pos < L) {
    t = pulseIn(IR, HIGH);
    data[pos++] = (unsigned int)t;
  }
  digitalWrite(led, LOW);
  
  if (pos == L)
    Serial.println("Warning: Storage exhausted!");
  
  int bits = 0;
  for (int i=0; i<pos; i++) {
    unsigned long d = data[i];
    if (d < 500) {
      Serial.print(0);
      bits++;
    } else if (d < 5000) {
      Serial.print(1);
      bits++;
    } else {
      Serial.println();
      gaps[gap++] = d;
    }
    if (d > 250 && d < 900) {
      Serial.print("Warning: ambiguous pulse length ");
      Serial.println(d);
    }
  }
 
  Serial.print("\nGaps: ");
  for (int i=0; i<3; i++) {
    Serial.print(gaps[i]);
    Serial.print(' ');
  } 
  
  Serial.print("\nBits: ");
  Serial.println(bits);
}


