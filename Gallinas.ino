/*
  ITES - Microcontroladores 2
  15/05/23 - Trabajo Practico Integrador

  Autor: Sabetta Enrique
  Profesor: Ing. Alejo Giles

  Enunciado: Dispenser Automatico de Alimento Balanceado para Gallinas

  Descripción del problema:

  En el marco de un proyecto de control para realizar un Dispenser Automatico de Alimento Balanceado para Gallinas, se solicita desarrollar un sistema utilizando Arduino
  
  El sistema permitirá controlar el motor de un dispenser de alimentos para gallinas, teniendo en cuenta que debara solo funcionar mientras sea de dias y cada ciertos 
  intervalos de tiempo, hasta tanto se haga de noche nuevamente.

  El sistema constará de los siguientes elementos: Motor 12v: que permitirá realizar la dispercion del alimento a baja velocidad ya que se necesita que el alimento caiga en un comedero, 
  Interfaz de usuario: La comunicación con el sistema se realizará a través del puerto serie (UART) del Arduino.
   Se utilizará un programa externo en la computadora para enviar comandos y recibir notificaciones desde el Arduino.
   Programa Arduino: El programa desarrollado en Arduino deberá gestionar los eventos sincrónicos y asincrónicos, recibir comandos por el puerto serie y controlar el motor 
   para realizar un test del motor.
  Requerimientos funcionales:Evento sincrónico: Cuando se reciba un comando por el puerto serie desde la interfaz de usuario para testear el funcionamiento del motor,
*/

#define MS_INTERVALO_LED_TEST            500 //Function LedTest()
#define LED_TEST                         13
#define CONFIG_LED_TEST                  pinMode(LED_TEST, OUTPUT)
#define ACTUALIZAR_LED_TEST(x)           digitalWrite(LED_TEST, x)

#define PIN_MOTOR                        11
#define CONFIG_PIN_MOTOR                 pinMode(PIN_MOTOR, OUTPUT)
#define MOTOR_ENCENDIDO                  analogWrite(PIN_MOTOR, 140)
#define MOTOR_APAGADO                     analogWrite(PIN_MOTOR, 0)

#define PIN_LDR                          A0
#define CONFIG_PIN_LDR                   pinMode(PIN_LDR, INPUT)
#define LEER_LDR                         analogRead(PIN_LDR)

#define PIN_ESTACION                     10
#define CONFIG_PIN_ESTACION              pinMode(PIN_ESTACION, INPUT)
#define LEER_PIN_ESTACION                digitalRead(PIN_ESTACION)
#define INVIERNO                         4
#define VERANO                           7

#define TPO_ENCENDIDO                    3500
#define TPO_APAGADO                      180000
#define TPO_TEST                         1500  // tiempo para testeo de funcionamiento del motor

//Variables Globales

int lectura_ldr;
int pasada = 0;
int Tiempo_Motor = 0;
int estacion = 0;
unsigned long Millis_ant_espera = millis();

void setup() { 
  Serial.begin(9600);
  CONFIG_LED_TEST; 
  CONFIG_PIN_MOTOR;
  CONFIG_PIN_LDR;
  if(LEER_PIN_ESTACION) estacion ? VERANO : INVIERNO;  

  ACTUALIZAR_LED_TEST(0); 
}

void loop()
 {
  LedTest();  
  Leer_LDR();
  RecepcionSerie();
  Ctrl_Motor();
 }


void LedTest()
{
  static int ledState= 0;
  static unsigned long antMillis = 0;
 
  if(millis() - antMillis < MS_INTERVALO_LED_TEST) return;    
  antMillis = millis();

  ledState = !ledState;
  ACTUALIZAR_LED_TEST(ledState);
}

void Leer_LDR()
{
  // static unsigned long Millis_ant_espera = 0;
  lectura_ldr = LEER_LDR;
  
 
  if(lectura_ldr < 600) return;
  if(pasada > estacion) return;
  if(Tiempo_Motor != 0) return;
  if(millis() - Millis_ant_espera < TPO_APAGADO) return;
   Tiempo_Motor = 3500;
   pasada++;
}

void RecepcionSerie()
{
  //Preguntar si hay datos en el puerto Serie, esperando a que sean leidos
  //Si no hay datos, salir de la función
  //Si hay datos, leer el dato y procesarlo
  byte dato;
 
  if(!Serial.available()) return;
  if(Tiempo_Motor != 0) return;
  if(dato !='t' || dato != 'T') return;
    Tiempo_Motor = 1500;    
  }

void Ctrl_Motor(){
  if(Tiempo_Motor == 0) MOTOR_APAGADO;
  Tiempo_Motor = Tiempo_Motor--;
  Millis_ant_espera = millis();
  MOTOR_ENCENDIDO;
}
