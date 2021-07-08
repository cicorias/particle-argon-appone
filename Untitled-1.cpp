//int led1 = D0; // Instead of writing D0 over and over again, we'll write led1
int led2 = D7; // Instead of writing D7 over and over again, we'll write led2

void setup() {

  //pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

}

void loop() {
  // To blink the LED, first we'll turn it on...
  //digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);

  // We'll leave it on for 1 second...
  delay(1000);

  // Then we'll turn it off...
  //digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  // Wait 1 second...
  delay(1000);

  // And repeat!
}

