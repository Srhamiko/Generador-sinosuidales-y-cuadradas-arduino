//señales sinosuidales  Autor:  Andrés Felipe García Loaiza. Inspirado por JCBRO Labs 
//señales cuadradas     Autor:  David Guzmán

//Este codigo tiene se usara para crear señales sinosuidales desde uno de los pines PWM del arduino
//La cual podrá medir como hemos estado haciendo durante el semestre

/*El generador de señales cuadradas no lo pude convertir perfecto, pero funciona */

//-----------------------------------------------------------------------------------------------------------------------------
//°°Señales sinosuidales°° Primero estaran las variables que deberan modificar durante la pratica en la parte 1 y 2

int f=2 ;          //Frecuencia de la señal. Este valor esta en HZ

float fs= 500.0;  //Separación entre valores, "Frecuencia de evaluación".   Recuerde la conversion a periodo T=1/F. 
                  //Por así decirlo esto define el espacio entre los por puntos en la función seno. Ya que no podemos generar una función continua

int pin1=10 ;     //pin del que saldra la señal sinosuidal
                  //Pin del que medira la señal que vamos a amplificar A0
                  //Tenga en cuenta que la señal (Amplificada) que vamos a medir debe entrar por el Pin A1
                  
                  

//-----------------------------------------------------------------------------------------------------------------------------


//   para osciloscipio
//   1) Escoger la tasa (baudRate) de comunicación de puerto serial. ATENCIÓN: usar esta tasa en monitor serial y serial plotter.
unsigned long miBaudRate = 9600;        //en baudios, valores válidos: {9600,19200,38400,57600,115200,230400}
//   2) Escoger periodo de muestreo.
unsigned long periodoMuestreo_ms = 1;    // intervalo en el que lee entradas (milisegundos)=periodo de muestreo.


//-----------------------------------------------------------------------------------------------------------------------------
//°°Señales Cuadradas°° Debera modificar estas variables para la parte 3

int pin2=12 ;                         //pin del que saldra la señal cuadrada

//   para generadores
int t_on_generadorD13_ms = 500;      //tiempo que dura el pulso de salida en D13 en encendido (5V)
int t_off_generadorD13_ms = 500;     //tiempo que dura el pulso de salida en D13 en apagado (0V)
int t_on_generadorD12_ms = 50;       //tiempo que dura el pulso de salida en D12 en encendido (5V)
int t_off_generadorD12_ms = 10000;   //tiempo que dura el pulso de salida en D12 en apagado (0V)





//-----------------------------------------------------------------------------------------------------------------------------


//Estos valores no deberían tener la necesidad de modificarlos

int cant=500;             // Cantidad valores que se guardan en la memoria de arduino.
int sig[500]; 
float t;
int entradaA0 = 0;        // lectura de entrada análoga A0

int entradaA1 = 0;        // lectura de entrada análoga A1
int entradaA2 = 0;        // lectura de entrada análoga A2
float voltajeA0 = 0;       // valor en voltios de entrada análoga A0
float voltajeA1 = 0;       // valor en voltios de entrada análoga A0
float voltajeA2 = 0;       // valor en voltios de entrada análoga A0
unsigned long lecturaPrevia_ms = 0;        // tiempo de la última ejecución de milis
unsigned long cronoGen13=0; //para almacenar la hora de crono generador 13
unsigned long cronoGen12=0; //para almacenar la hora de crono generador 12


void setup() {
  // inicia comunicación serial
  Serial.begin(miBaudRate);
  
  // configura pines digitales de salida
  pinMode (pin1,OUTPUT);
   pinMode(pin2, OUTPUT);
  //generar una señal seno
  for(int i=0; i<cant; i++)
  {
    t=(float)i/fs;
    //La señal ira de 0 a 255 
    sig[i]=(int)(127.0*(sin(2*3.14*f*t)+1.0));
    }
}

void loop() {

 //Genera un señal seno y una cuadrada, con los valores especificados y mide la señal entrante en A0

 unsigned long lecturaActual_ms = millis(); //lee hora actual

    generadorCuadrada(pin2,t_on_generadorD12_ms,t_off_generadorD12_ms,&cronoGen12);
   
    for (int i=0;i<cant;i++)
      {
      analogWrite(pin1,sig[i]);
      int entradaA0 = analogRead(0);
      float voltajeA0 = deEnteroAVoltaje1(entradaA0);
  
       int entradaA1 = analogRead(1);
      float voltajeA1 = deEnteroAVoltaje1(entradaA1);

      //Serial.println(sig[i]);
      //Serial.print('\t');
      Serial.print(voltajeA0,3); //el '3' indica 3 cifras decimales
      Serial.print('\t')
      Serial.println(voltajeA1,3)
      delay(f);// intervalos de evaluación
    
      }
   }


float deEnteroAVoltaje1(int valor){
  //convierte número entero de 0 a 255 en un voltaje, siendo 5V=255.
  return float(valor)*5/255; 
}

float deEnteroAVoltaje2(int valor){
  //convierte número entero de 0 a 1023 en un voltaje, siendo 5V=1023.
  return float(valor)*5/1023; 
}
void generadorCuadrada(int pinDigital, int t_on_ms, int t_off_ms, unsigned long *cronoLocal){
  //genera una señal cuadrada periodica de amplitud 5V, con las
  //duraciones de encendido y apagado especificadas.

  unsigned long actualMillis = millis(); //lee hora actual
  unsigned long siguiente_on=0;
  unsigned long siguiente_off=0;

  siguiente_off = *cronoLocal + t_on_ms;    //apagar cuando hayan pasado t_on_ms
  siguiente_on = *cronoLocal + t_on_ms + t_off_ms;  //prender cuando hayan pasado t_on_ms+t_off_ms

  if(actualMillis > siguiente_on) { //si es hora de encender
    digitalWrite(pinDigital,HIGH);  //encender
    *cronoLocal = siguiente_on;     //actualiza hora de último encendido
    while(actualMillis > (*cronoLocal + t_on_ms + t_off_ms))
    {
      //si el reloj quedó atrasado, actualizar hasta que la diferencia no supere un periodo (t_on+t_off)
      *cronoLocal = *cronoLocal + t_on_ms + t_off_ms; //si el reloj quedó atrasado, actualizar hasta 
    }
  }
  else if(actualMillis > siguiente_off) { //si es hora de apagar
    digitalWrite(pinDigital,LOW); //apagar
  } 
}
