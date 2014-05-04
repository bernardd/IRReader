int LED = 13;

#define L 400
#define IR 2

unsigned int data[L];
unsigned int gaps[3];

void setup() {
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);
	pinMode(IR, INPUT_PULLUP);
	Serial.begin(115200);
	delay(2000);
	Serial.println("Initialised");
}

void loop() {
	for (int i=0; i<L; i++)
		data[i] = 0;
	for (int i=0; i<3; i++)
		gaps[i] = 0;

	unsigned long t=1;
	unsigned int pos = 0;
	unsigned int gap = 0;
	unsigned long on_duration;
	while (digitalRead(IR)) {}

	digitalWrite(LED, HIGH);
	while (t && pos < L) {
		unsigned long s = micros();
		t = pulseIn(IR, HIGH);
		if (t) {
			data[pos++] = (unsigned int)t;
			on_duration = micros() - s - t;
		}
	}
	digitalWrite(LED, LOW);

	if (pos == L)
		Serial.println("Warning: Storage exhausted!");

	int bits = 0;
	int offset = 0;
	for (int i=0; i<pos; i++) {
		unsigned long d = data[i];
		if (d > 100 && d < 400) {
			Serial.print(0);
			bits++;
		} else if (d > 800 && d < 1800) {
			Serial.print(1);
			bits++;
		} else {
			Serial.println();
			gaps[gap++] = d;
			offset = -1;
		}
		if ((d >= 400 && d <= 800) || d <= 100) {
			Serial.print("Warning: ambiguous pulse length ");
			Serial.println(d);
		}
		if (!((offset++)%8))
			Serial.print('.');
	}
	Serial.println();
	for (int i=0; i<pos; i++) {
		Serial.print(data[i]);
		Serial.print(',');
	}

	Serial.print("\nLast on time: ");
	Serial.println(on_duration);

	Serial.print("Gaps: ");
	for (int i=0; i<3; i++) {
		Serial.print(gaps[i]);
		Serial.print(' ');
	}

	Serial.print("\nBits: ");
	Serial.println(bits);
}
