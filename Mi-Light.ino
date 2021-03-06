#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET

#include <Blinker.h>
#include <Servo.h>
#define PIN_SERVO D4
Servo myservo;
char auth[] = "Blinker Key";
char ssid[] = "Wifi name";
char pswd[] = "wifi key";

bool oState = false;
BlinkerButton Button1("Light");
void button1_callback(const String & state)
{
   BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_ON) {
        myservo.write(32);
        digitalWrite(D3, HIGH);
        Button1.text("灯已开启");
        delay(500);
        digitalWrite(D3, LOW);
        myservo.write(90);
        Button1.print("on");
    }
    else if (state == BLINKER_CMD_OFF) {
        myservo.write(134);
        digitalWrite(D3, HIGH);
        Button1.text("灯已关闭");
        delay(500);
        digitalWrite(D3, LOW);
        myservo.write(90);    
        Button1.print("off");
   }
}
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        myservo.write(32);
        digitalWrite(D3, HIGH);
        delay(500);
        digitalWrite(D3, LOW);
        myservo.write(90);

        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        myservo.write(134);
        digitalWrite(D3, HIGH);
        Button1.text("灯已关闭");
        delay(500);
        digitalWrite(D3, LOW);
        myservo.write(90);
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();

        oState = false;
    }
}

void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
    }
}

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);

    pinMode(D3, OUTPUT);
    digitalWrite(D3, LOW);

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);
}

void loop()
{
    Blinker.run();
}
