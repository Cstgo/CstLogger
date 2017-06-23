//DHT11*********************************************************************
#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 4
//DHT11*********************************************************************
//SONIDO********************************************************************
#define PIN_GATE_IN 2
#define IRQ_GATE_IN  0
#define PIN_LED_OUT 13
#define PIN_ANALOG_IN A1
//SONIDO*******************************************************************
//DUST SENSOR***************************************************************
int pin = 7;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000;//sampe 30s&nbsp;;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
//DUST SENSOR**************************************************************
//MQ7************************************************************
int adc_MQ = analogRead(A2);
//MQ7*************************************************************

//SONIDO***********************************************************
void soundISR()
{
  int pin_val;

  pin_val = digitalRead(PIN_GATE_IN);
  digitalWrite(PIN_LED_OUT, pin_val);   
}
//SONIDO*******************************************************************

void setup(){
//DHT11******************************************************************
  Serial.begin(9600);
  Serial.println(DHT11LIB_VERSION);
//DHT11******************************************************************

//DUST SENSOR**************************************************************
    pinMode(7,INPUT);
  starttime = millis();//get the current time;
//DUST SENSOR**************************************************************

//SONIDO*****************************************************************
     //  Configure LED pin as output
  pinMode(PIN_LED_OUT, OUTPUT);

  // configure input to interrupt
  pinMode(PIN_GATE_IN, INPUT);
  attachInterrupt(IRQ_GATE_IN, soundISR, CHANGE);

  // Display status
  Serial.println("Initialized");
//SONIDO************************************************************************
}

void loop(){
//DHT11*******************************************************************
 int chk;
  Serial.print("DHT11, \t");
  chk = DHT.read(DHT11_PIN);    // READ DATA
  switch (chk){
    case DHTLIB_OK:  
                Serial.print("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }
 // DISPLAT DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);

  delay(1000);
//DHT11*********************************************************************

//DUST SENSOR***************************************************************
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) >= sampletime_ms)//if the sampel time = = 30s
  
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=&gt;100
    concentration = 3.531073446*(1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62); // using spec sheet curve
    Serial.print("concentration = ");
    Serial.print(concentration);
    Serial.println(" particulas/L");
    lowpulseoccupancy = 0;
    starttime = millis();
//DUST SENSOR**************************************************************

//MQ7********************************************************************
  float voltaje = adc_MQ * (5.0 / 1023.0); 
  float Rs=1000*((5-voltaje)/voltaje);  
  double monoxide =76.211*pow(Rs/1284, -1.663);      // read analog input pin 0
  Serial.println(monoxide);  // prints the value read
  Serial.println(" ppmv"); 
  delay(1000);                        // wait 100ms for next reading
//MQ7****************************************************************

//SONIDO***************************************************************
  int value;

  // Check the envelope input
  value = analogRead(PIN_ANALOG_IN);

    // Convert envelope value into a message
  //deje 21 dB como el minimo y calcule el valor de x de la ecuacion
  //21=20*log(8/x) x~2.79
  
    Serial.print(20*log(value/2.8)); 
    Serial.print(" dB");
    Serial.print('\n');
    // pause for 1 second
    delay(100);
//SONIDO************************************************************
}
