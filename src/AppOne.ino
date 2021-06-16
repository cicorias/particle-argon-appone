
int led1 = D0; // Instead of writing D0 over and over again, we'll write led1
// You'll need to wire an LED to this one to see it blink.

int led2 = D7; // Instead of writing D7 over and over again, we'll write led2
// This one is the little blue LED on your board. On the Photon it is next to D7, and on the Core it is next to the USB jack.


SYSTEM_THREAD(ENABLED);

// This allows for USB serial debug logs
SerialLogHandler logHandler;

// Forward declarations (functions used before they're implemented)
void getSensor();

// This is the event name to publish
const char *eventName = "bd.ep.1.critical.loc"; //bd-wifiQuality";  com.bd.area.stuff.wifiquality

// This is how often to publish (30s = every 30 seconds)
// Other useful units include min for minutes and h for hours.
std::chrono::milliseconds publishPeriod = 2min; //10s; //2min; //5s;

// This keeps track of the last time we published
unsigned long lastPublishMs;

void setup() {

  // We are going to tell our device that D0 and D7 (which we named led1 and led2 respectively) are going to be output
  // (That means that we will be sending voltage to them, rather than monitoring voltage that comes from them)

  // It's important you do this here, inside the setup() function rather than outside it or in the loop function.

  // pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

}

struct values {
  float quality;
  float level;
} bag;

void getSensor(){
  WiFiSignal sig = WiFi.RSSI();
  bag.level = sig.getQualityValue();
  bag.quality = sig.getQuality();
}

void publish(){
   if (millis() - lastPublishMs >= publishPeriod.count())
    {
        lastPublishMs = millis();
        getSensor();
        // The event data is string but we just send our value as
        // a ASCII formatted decimal number
        String eventData = String::format("{\"body\": { \"level\":  \"%.02f%\", \"quality\": \"%.02f%%\" }}", bag.level, bag.quality);


        char buf[1024];
        JSONBufferWriter writer(buf, sizeof(buf));
        writer.beginObject();
          writer.name("level").value(bag.level);
          writer.name("quality").value(bag.quality);
        writer.endObject();
        writer.buffer()[std::min(writer.bufferSize(), writer.dataSize())] = 0;

        // Make sure we're cloud connected before publishing
        if (Particle.connected())
        {
            // Particle.publish(eventName, buf);
            Particle.publish(eventName, eventData);

            Log.info("published %s", eventData.c_str());
        }
        else
        {
            Log.info("not cloud connected %s", eventData.c_str());
        }
    }
}

// Next we have the loop function, the other essential part of a microcontroller program.
// This routine gets repeated over and over, as quickly as possible and as many times as possible, after the setup function is called.
// Note: Code that blocks for too long (like more than 5 seconds), can make weird things happen (like dropping the network connection).  The built-in delay function shown below safely interleaves required background activity, so arbitrarily long delays can safely be done if you need them.

void loop() {

  // To blink the LED, first we'll turn it on...
  // digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);

  // We'll leave it on for 1 second...
  delay(5000);

  publish();
  // Then we'll turn it off...
  // digitalWrite(led1, LOW); 
  digitalWrite(led2, LOW);

  // Wait 1 second...
  delay(5000);

  // And repeat!
}

