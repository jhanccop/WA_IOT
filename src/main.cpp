/*
UBM analyzer version for unipetro abc
*/

/* ====================== LCD CONFIG ======================== */
#include <Arduino.h>

#define _Digole_Serial_I2C_
#include <DigoleSerial.h>

#include <Wire.h>
DigoleSerialDisp mydisp(&Wire, '\x27');

const unsigned char fonts[] = {6, 10, 18, 51, 120, 123};
const char *fontdir[] = {"0\xb0", "90\xb0", "180\xb0", "270\xb0"};

/* ====================== MQTT CONFIG ======================== */
#include <WiFi.h>

String ssid = "";
String password = "c0l053n5353:20";

WiFiClient client;
#include <PubSubClient.h>

PubSubClient mqtt(client);

const char *broker = "broker.emqx.io";
const int mqtt_port = 1883;
const char *mqtt_id = "gle_client_234";
const char *mqtt_user = "gle_client_1";
const char *mqtt_pass = "gle2022_ttxx";

const char* topicSubscribe = "gle/data/#";

/* ========================== TOOLS ========================== */
#include <Average.h>
#include <Separador.h>
#include <ArduinoJson.h>

Separador s;

/* ========================== Variables ========================== */

char task = '0';
unsigned long now = 0;
float pos_raw[300] = {0.0014, 0.0028, 0.0044, 0.006,
                      0.0083, 0.0107, 0.0135, 0.0163,
                      0.0197, 0.0231, 0.0272, 0.0313,
                      0.0359, 0.0406, 0.0458, 0.0511,
                      0.0565, 0.062, 0.0679, 0.0738,
                      0.0804, 0.087, 0.0939, 0.1008,
                      0.108, 0.1152, 0.1229, 0.1305,
                      0.1385, 0.1466, 0.1634, 0.172,
                      0.1805, 0.1894, 0.1982, 0.2072,
                      0.2162, 0.2254, 0.2346, 0.2442,
                      0.2538, 0.2637, 0.2736, 0.2835,
                      0.2934, 0.3034, 0.3134, 0.3234,
                      0.3334, 0.3435, 0.3536, 0.3638,
                      0.3739, 0.384, 0.3941, 0.4042,
                      0.4143, 0.4243, 0.4343, 0.4441,
                      0.454, 0.4735, 0.4833, 0.493,
                      0.5026, 0.5121, 0.5217, 0.5312,
                      0.5405, 0.5499, 0.5591, 0.5683,
                      0.5774, 0.5865, 0.5955, 0.6044,
                      0.6132, 0.622, 0.6308, 0.6395,
                      0.6481, 0.6567, 0.6651, 0.6735,
                      0.6816, 0.6897, 0.6976, 0.7055,
                      0.7133, 0.7211, 0.7364, 0.7439,
                      0.7513, 0.7585, 0.7657, 0.7727,
                      0.7796, 0.7863, 0.793, 0.7996,
                      0.8062, 0.8126, 0.819, 0.8253,
                      0.8316, 0.8377, 0.8437, 0.8495,
                      0.8552, 0.8608, 0.8665, 0.8719,
                      0.8774, 0.8827, 0.8879, 0.8931,
                      0.8982, 0.903, 0.9079, 0.9124,
                      0.917, 0.9258, 0.93, 0.9341,
                      0.9381, 0.942, 0.9457, 0.9495,
                      0.9529, 0.9563, 0.9595, 0.9628,
                      0.9657, 0.9687, 0.9714, 0.9741,
                      0.9766, 0.9791, 0.9814, 0.9836,
                      0.9856, 0.9875, 0.9893, 0.991,
                      0.9925, 0.9939, 0.9951, 0.9963,
                      0.9971, 0.998, 0.9993, 0.9996,
                      0.9999, 1, 1, 0.9998,
                      0.9996, 0.9991, 0.9986, 0.9976,
                      0.9967, 0.9953, 0.9939, 0.9922,
                      0.9905, 0.9884, 0.9863, 0.9838,
                      0.9813, 0.9785, 0.9756, 0.9725,
                      0.9694, 0.9658, 0.9623, 0.9584,
                      0.9544, 0.95, 0.9456, 0.9408,
                      0.9361, 0.926, 0.9207, 0.9153,
                      0.9096, 0.9039, 0.8978, 0.8917,
                      0.8852, 0.8787, 0.8718, 0.8649,
                      0.8576, 0.8504, 0.8428, 0.8352,
                      0.8274, 0.8195, 0.8115, 0.8036,
                      0.7953, 0.787, 0.7783, 0.7696,
                      0.7606, 0.7515, 0.7423, 0.7331,
                      0.7237, 0.7143, 0.6949, 0.6849,
                      0.675, 0.6651, 0.6552, 0.6462,
                      0.6372, 0.6286, 0.62, 0.6111,
                      0.6022, 0.5925, 0.5828, 0.574,
                      0.5652, 0.5563, 0.5474, 0.537,
                      0.5265, 0.5163, 0.5061, 0.4958,
                      0.4854, 0.4744, 0.4634, 0.4524,
                      0.4414, 0.4305, 0.4197, 0.4091,
                      0.3986, 0.3784, 0.3682, 0.358,
                      0.3475, 0.3369, 0.3264, 0.316,
                      0.3064, 0.2969, 0.2881, 0.2793,
                      0.2709, 0.2625, 0.2539, 0.2453,
                      0.2366, 0.2278, 0.2195, 0.2111,
                      0.2029, 0.1947, 0.1868, 0.1789,
                      0.1708, 0.1627, 0.1546, 0.1465,
                      0.1389, 0.1312, 0.1162, 0.1089,
                      0.1016, 0.0949, 0.0883, 0.0821,
                      0.076, 0.0701, 0.0642, 0.0588,
                      0.0533, 0.0481, 0.0429, 0.0387,
                      0.0345, 0.0309, 0.0273, 0.0239,
                      0.0204, 0.0176, 0.0147, 0.0123,
                      0.0098, 0.0078, 0.0058, 0.0043,
                      0.0027, 0.0017, 0.0007, 0};
float load_raw[300] = {0.3606115, 0.33903414, 0.3922795, 0.4211483, 0.4550223,
                       0.47765988, 0.5050532, 0.5142181, 0.55885583, 0.5775468,
                       0.664648, 0.61376065, 0.6219772, 0.64155227, 0.66569746,
                       0.5580315, 0.5525977, 0.513313, 0.5420354, 0.59175265,
                       0.60140926, 0.6649601, 0.6495745, 0.6696029, 0.72067523,
                       0.7913527, 0.8353251, 0.8399867, 0.87123525, 0.8728295,
                       0.8833198, 0.91717243, 0.91462076, 0.8959379, 0.8569939,
                       0.88733065, 0.9412348, 0.9440607, 0.9583187, 0.95494676,
                       0.96113646, 0.952586, 0.9418225, 0.93997306, 0.90967417,
                       0.8680111, 0.8620733, 0.8669461, 0.804965, 0.8126818,
                       0.85008234, 0.84087443, 0.8198643, 0.8238918, 0.8124782,
                       0.8224453, 0.8103448, 0.7724205, 0.76821053, 0.7780187,
                       0.7744225, 0.80508965, 0.78776824, 0.76435626, 0.75493336,
                       0.7887965, 0.82779276, 0.8043143, 0.76733184, 0.79054946,
                       0.8029038, 0.8050232, 0.81068075, 0.82786775, 0.8838308,
                       0.88724387, 0.84669924, 0.8523105, 0.8588941, 0.8260691,
                       0.8537613, 0.8228015, 0.82802486, 0.8233645, 0.79121184,
                       0.7538272, 0.78828204, 0.79991925, 0.8268205, 0.81290245,
                       0.7727095, 0.7454257, 0.71291834, 0.7347312, 0.73392826,
                       0.73111475, 0.70487773, 0.7552767, 0.7180718, 0.7707094,
                       0.76444054, 0.80604535, 0.8300744, 0.8416637, 0.8173355,
                       0.8367058, 0.8187766, 0.8261168, 0.83926773, 0.82781243,
                       0.88463706, 0.8739078, 0.8764258, 0.861392, 0.8620511,
                       0.88500214, 0.8666898, 0.8242898, 0.7961605, 0.78832877,
                       0.80317485, 0.8004968, 0.79433334, 0.7513094, 0.8270639,
                       0.7746248, 0.787143, 0.76209223, 0.8075487, 0.78486955,
                       0.75595963, 0.80039084, 0.8045794, 0.78936183, 0.80841863,
                       0.8504063, 0.86570895, 0.8512932, 0.8607421, 0.86735165,
                       0.89415026, 0.8695968, 0.8573177, 0.85699606, 0.86738586,
                       0.8907181, 0.87729573, 0.88787436, 0.90686035, 0.8752161,
                       0.8547424, 0.84555656, 0.80298483, 0.79247785, 0.7872876,
                       0.78450334, 0.74554926, 0.70407027, 0.6369108, 0.59780383,
                       0.5532343, 0.45749414, 0.47528288, 0.431284, 0.41979182,
                       0.49753302, 0.46154612, 0.4573014, 0.43306053, 0.4715577,
                       0.5183486, 0.49342868, 0.4888159, 0.5233357, 0.51772344,
                       0.5160141, 0.539628, 0.49725333, 0.48841026, 0.4695868,
                       0.5233943, 0.56342995, 0.52499217, 0.5719448, 0.5258349,
                       0.46837407, 0.52440554, 0.49893892, 0.46065307, 0.5048745,
                       0.5165652, 0.44865412, 0.46483424, 0.5372943, 0.5606333,
                       0.4725001, 0.51316327, 0.5328481, 0.54023993, 0.4862748,
                       0.54036444, 0.5451182, 0.5074548, 0.60317874, 0.5545964,
                       0.53890127, 0.55679655, 0.60497975, 0.56109995, 0.587025,
                       0.59533197, 0.5318593, 0.60887045, 0.5648048, 0.4834313,
                       0.3977546, 0.31755555, 0.20914239, 0.15898412, 0.08423555,
                       0.07330677, 0.07761616, 0.09327832, 0.07639554, 0.09941348,
                       0.08002061, 0.09765211, 0.08518991, 0.06350625, 0.06660935,
                       0.08852655, 0.0969362, 0.14467105, 0.21598798, 0.27866432,
                       0.32346076, 0.42808956, 0.49052542, 0.44920576, 0.49967977,
                       0.5009391, 0.46910706, 0.47279832, 0.45726538, 0.47780937,
                       0.45358694, 0.35850388, 0.45474082, 0.46069658, 0.3786825,
                       0.37911958, 0.36964303, 0.2756323, 0.30942374, 0.20248419,
                       0.16962358, 0.16273305, 0.18048078, 0.14912087, 0.17688003,
                       0.15245947, 0.22473991, 0.2053492, 0.25939244, 0.22605795,
                       0.21223244, 0.21097353, 0.2266089, 0.27240372, 0.30615088,
                       0.3106324, 0.30006024, 0.39688137, 0.3661871, 0.4734667,
                       0.45800468, 0.38199878, 0.4207209, 0.4314303, 0.44544455,
                       0.4433374, 0.41029093, 0.45660704, 0.54108155, 0.50227606,
                       0.5470507, 0.53670824, 0.44938523, 0.3577729, 0.3025245,
                       0.30317277, 0.290632, 0.2333867, 0.2849285, 0.2302016,
                       0.25801674, 0.29357973, 0.3058403, 0.23381004, 0.3365273};

/* ************************************************************************************************************** */
/* ************************************************* FUNCTIONS ************************************************** */
/* ************************************************************************************************************** */

void mqttCallback(char* topic, byte* payload, unsigned int length);
void reconnect();


/* ********************** RESET POSITION *********************** */
void resetpos1(void) // for demo use, reset display position and clean the demo line
{
    mydisp.setPrintPos(0, 0, _TEXT_);
    delay(3000);         // delay 2 seconds
    mydisp.println(" "); // display space, use to clear the demo line
    mydisp.setPrintPos(0, 0, _TEXT_);
}

/* ********************** LCD PREPARE *********************** */
void lcd_prepare(void)
{
    // delay(100);
    mydisp.backLightOff();
    // delay(100);
    mydisp.clearScreen();
    mydisp.setFont(fonts[0]);

    // mydisp.backLightOff();
    //  mydisp.setColor(1);

    uint8_t i = 2;
    mydisp.setRotation(i);
    // mydisp.drawStr(0, 0, "RPC ANALYZER v1");
    // mydisp.drawStr(20, 2, "SPM:");
    // mydisp.drawLine(0, 6, 127, 6);
}

/* ******************* PRINT DATETIME  ********************** */
String print_datetime(boolean show)
{
    mydisp.setFont(fonts[0]);
    mydisp.setPrintPos(16, 0, _TEXT_);
    // DateTime now = rtc.now();
    // String year = String(now.year());
    // year.remove(0,2);
    // String datetime = year + "/" + String(now.month()) + "/" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute());

    String datetime = "2022-12-23 08:56";
    if (show)
    {
        mydisp.print(datetime);
    }
    return datetime;
}

/* ******************* PRINT CLASS  ********************** */
void print_CLASS(String label)
{

    String row1 = s.separa(label, ' ', 0);
    String row2 = s.separa(label, ' ', 1);
    mydisp.setFont(fonts[1]);
    mydisp.setMode('|');
    // mydisp.setFont(120);
    mydisp.setPrintPos(13, 3, _TEXT_);
    mydisp.print(row1);
    mydisp.setPrintPos(13, 4, _TEXT_);
    mydisp.print(row2);
}

/* ************************* LCD 2 CURRENT STATE ************************ */
void printValue(String label, float value, int pos_x, int pos_y, int separe)
{

    mydisp.setPrintPos(pos_x, pos_y, _TEXT_);
    mydisp.print("             ");
    mydisp.setPrintPos(pos_x, pos_y, _TEXT_);
    mydisp.print(label);

    mydisp.setPrintPos(pos_x + separe, pos_y, _TEXT_);
    mydisp.print("       ");
    mydisp.setPrintPos(pos_x + separe, pos_y, _TEXT_);
    mydisp.print(value);
}

void currentState(void)
{
    mydisp.clearScreen();
    mydisp.setFont(fonts[0]);

    mydisp.drawStr(8, 0, "CURRENT  STATE");
    mydisp.drawLine(0, 6, 127, 6);

    printValue("Pump Fillage (%):", 90.78, 1, 2, 25);
    printValue("Pumping speed (spm):", 13.48, 1, 3, 25);
    printValue("Production today (bbl):", 124.56, 1, 5, 25);
    printValue("Tomorrow project (bbl):", 125.32, 1, 6, 25);
    printValue("Run Time today (hours):", 17.59, 1, 8, 25);
}

/* ************************* LCD3 BACKGROUND ************************ */

void valueOverview(float value, int pos_x, int pos_y)
{
    mydisp.setPrintPos(pos_x, pos_y, _TEXT_);
    delay(50);
    mydisp.print("       ");

    mydisp.setPrintPos(pos_x, pos_y, _TEXT_);
    delay(50);
    mydisp.print(value);
    // delay(25);
}

void backgorund()
{
    mydisp.clearScreen();
    mydisp.setFont(fonts[0]);

    // print datetime
    print_datetime(true);

    // print box dynachart
    mydisp.drawHLine(0, 63, 65);
    mydisp.drawVLine(0, 0, 63);

    // print labels
    mydisp.setPrintPos(51, 2, _TEXT_);
    mydisp.print("Load");

    mydisp.setPrintPos(52, 3, _TEXT_);
    mydisp.print("Pos");

    mydisp.setPrintPos(49, 4, _TEXT_);
    mydisp.print("P Load");

    mydisp.setPrintPos(49, 5, _TEXT_);
    mydisp.print("m Load");

    mydisp.setPrintPos(49, 6, _TEXT_);
    mydisp.print("P fill");

    mydisp.setPrintPos(52, 7, _TEXT_);
    mydisp.print("SPM");

    valueOverview(12, 56, 4);
    valueOverview(12, 56, 5);

    valueOverview(12.54, 56, 6);
    valueOverview(12.4, 56, 7);
}

/* ************************* LCD 3 OVERVIEW REAL TIME ************************ */
void plotOverview(float pos, float load, float pos_pump, float load_pump)
{
    int x_surf = map(pos * 100, 0, 100, 5, 60);
    int y_surf = map(load * 100, 0, 100, 30, 2);
    mydisp.drawPixel(x_surf, y_surf);

    int x_pump = map(pos_pump * 100, 0, 100, 5, 60);
    int y_pump = map(load_pump * 100, 0, 100, 61, 33);
    mydisp.drawPixel(x_pump, y_pump);
}

void overview()
{
    // mydisp.clearScreen();
    // mydisp.setFont(fonts[0]);
    int count = 0;
    float currentLoad, currentPos;

    backgorund();
    while (task == '3')
    {
        Serial.println("overview");
        if (Serial.available() > 0)
        {
            char key = Serial.read();
            if (key != '\n')
            {
                task = key;
            }
        }

        currentLoad = load_raw[count];
        currentPos = pos_raw[count];
        if( count % 5 == 0 ){
            valueOverview(currentLoad, 56, 2);
            valueOverview(currentPos, 56, 3);
        }

        float pos_pump = currentPos;
        float load_pump = currentLoad;

        plotOverview(currentPos, currentLoad, pos_pump, load_pump);

        count += 1;
        if(count == 300){
            count=0;
            backgorund();
            }

        delay(150);
    }

    //valueOverview(currentLoad, 56, 2);
    // delay(50);
    //valueOverview(currentPos, 56, 3);
    // delay(50);
    
}

/* ************************* PLOTTER ************************ 
void plotter()
{

    int n_data = 300;
    yield();

    // int pos_i = map(pos_raw[0] * 100, 0, 100, 2, 69);
    // int load_i = map(load_raw[0] * 100, 0, 100, 63, 10);

    for (int i = 0; i < n_data; i++)
    {

        if (millis() > now + 50){
            now = millis();
            overview(load_raw[i], pos_raw[i]);
        }
        //overview(load_raw[i], pos_raw[i]);
        // int x = analogRead(34);
        // int y = analogRead(39);
        // overview(x, y);
        // delay(200);
    }
}

*/


/* ******************* RECONNECT  ********************** */
void reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    //Serial.print("Attempting MQTT connection...");
    //mqtt.publish(topicInit_pub,"reconnect");
    // Attempt to connect

    if (mqtt.connect(mqtt_id, mqtt_user, mqtt_pass)) {
      mqtt.subscribe(topicSubscribe);
    } else {

      delay(5000);
    }
  }
}

/* ******************* SETUP  ********************** */
void setup()
{
    Serial.begin(9600);
    mydisp.begin();
    delay(3000);
    lcd_prepare();

    // mydisp.backLightOff(); // poweroff led background

    currentState();
    delay(5000);

    // mydisp.clearScreen();
    // mydisp.setFont(fonts[0]);

    // print_datetime(true);
    // print_SPM(6.64);
    // print_CLASS("FLUID POUND");
    // backgorund();
    // plotter();
    // print_state("Local", "2", "run");
    // Serial.begin(115200);
}

void loop()
{
    if (Serial.available() > 0)
    {
        char key = Serial.read();
        if(key != '\n'){
            task = key;
        }
        
    }
    switch(task)
    {
        case '0':
            Serial.println("task 0");
            break;
        case  '1':
            Serial.println("task 1");
            break;
        case  '2':
            Serial.println("task 2");
            break;
        case  '3':
            Serial.println("task 3");
            overview();
            break;
        default:
            Serial.println("Default");
    }

    //backgorund();
    //plotter();
    //delay(3000);
}
