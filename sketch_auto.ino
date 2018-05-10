#define VEL_MAX 255 //MAX 255
 
// Motores
const int PWM_A_Pin = 8;                             // PWM se conecta al pin 1 del puente-H
const int Motor_A_Pin1 = 9;                          // Entrada 2 del puente-H 
const int Motor_A_Pin2 = 10;                         // Entrada 7 del puente-H 
const int Motor_B_Pin3 = 11;                         // Entrada 15 del puente-H 
const int Motor_B_Pin4 = 12;                         // Entrada 10 del puente-H 

//Establecemos las variables a utilizar
int velocidad;                                       // Establece la velocidad
int pasos;                                           // Establece el tiempo en milisegundos de espera

//Ultrasónico
int Trig = 2, Echo = 3;

long duracion, distancia;

char input;

String vDistancia;
String stringOne ="Distancia ";

boolean Avanzando = 0;

void setup()  
{
  
  Serial.begin(9600);                                //Comunicación con Sensor HCSR04
  pinMode(Trig,OUTPUT);                              //Configura Triguer SUS como salida.
  pinMode(Echo,INPUT);                               //Configura Echo SUS como entrada.
  
  pinMode(Motor_A_Pin1, OUTPUT);                     //Configura Motor_A_Pin1 como salida 
  pinMode(Motor_A_Pin2, OUTPUT);                     //Configura Motor_A_Pin2 como salida 
  pinMode(Motor_B_Pin3, OUTPUT);                     //Configura Motor_B_Pin3 como salida 
  pinMode(Motor_B_Pin4, OUTPUT);                     //Configura Motor_B_Pin4 como salida 
  
}

void loop() 
{ 

  Get_Valores_Sensor_Ultra();

  vDistancia = stringOne + distancia;
 
  Serial.println(vDistancia);

   if(distancia >=2 && distancia <=30){          //reversa para que no choque entre 2 cm y 10 cm      
      paro(2000);                                         // Detener Marcha durante 5 segundos.
      Serial.println("Stop de 2s"); 
      if (Avanzando)
      {
        Serial.println("Retroceso de 2s");
        marchaAtras(1000,VEL_MAX);
        Avanzando = 0;
        input=0;
        paro(2000);                                         // Detener Marcha durante 5 segundos.
      }            
   }
   else{      
      Serial.println("Esperando comando BT...");
      Procesa_Comando_BT();
    }
}

void paro(int pasos)                                  // Entrada 2 y 7 del puente-H a nivel bajo (0 Vcc)
{
  digitalWrite(Motor_A_Pin1,LOW);
  digitalWrite(Motor_A_Pin2,LOW);
  digitalWrite(Motor_B_Pin3,LOW);
  digitalWrite(Motor_B_Pin4,LOW);
  delay(pasos);                                       // Espera 1 milisegundo por cada paso  
}

void marchaAdelante(int pasos, int velocidad)        // Entrada 2 a nivel bajo (0 Vcc) y la 7 en alto (5 Vcc)
{
  analogWrite(PWM_A_Pin, velocidad);
  digitalWrite(Motor_A_Pin1,LOW);
  digitalWrite(Motor_A_Pin2,HIGH);
  digitalWrite(Motor_B_Pin3,LOW);
  digitalWrite(Motor_B_Pin4,HIGH);
  delay(pasos);                                       // Espera 1 milisegundo por cada paso  
}

void marchaAtras(int pasos, int velocidad)            // Entrada 7 a nivel bajo (0 Vcc) y la 2 en alto (5 Vcc)
{
  analogWrite(PWM_A_Pin, velocidad);
  digitalWrite(Motor_A_Pin1,HIGH);
  digitalWrite(Motor_A_Pin2,LOW);
  digitalWrite(Motor_B_Pin3,HIGH);
  digitalWrite(Motor_B_Pin4,LOW);
  delay(pasos);                                       // Espera 1 milisegundo por cada paso  
}

void GiroDerecha(int pasos, int velocidad)            // Entrada 7 a nivel bajo (0 Vcc) y la 2 en alto (5 Vcc)
{
  analogWrite(PWM_A_Pin, velocidad);
  digitalWrite(Motor_A_Pin1,LOW);
  digitalWrite(Motor_A_Pin2,HIGH);
  digitalWrite(Motor_B_Pin3,HIGH);
  digitalWrite(Motor_B_Pin4,LOW);
  delay(pasos);                                       // Espera 1 milisegundo por cada paso  
}

void GiroIzquierda(int pasos, int velocidad)            // Entrada 7 a nivel bajo (0 Vcc) y la 2 en alto (5 Vcc)
{
  analogWrite(PWM_A_Pin, velocidad);
  digitalWrite(Motor_A_Pin1,HIGH);
  digitalWrite(Motor_A_Pin2,LOW);
  digitalWrite(Motor_B_Pin3,LOW);
  digitalWrite(Motor_B_Pin4,HIGH);
  delay(pasos);                                       // Espera 1 milisegundo por cada paso  
}

/************SENSOR ULTRASONICO*******************/
void Get_Valores_Sensor_Ultra(){
  digitalWrite(Trig,LOW);
  delayMicroseconds(4);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(50);
  digitalWrite(Trig,LOW);

  duracion = pulseIn(Echo,HIGH);
  distancia = (duracion/2)/29;  
}

/************* MANEJO DE MOTORES **************/
void Procesa_Comando_BT(){
  if(Serial.available()>0){
    input=Serial.read();
  }
  if(input=='1'){
    digitalWrite(LED_BUILTIN,HIGH);
    //PORTB=B00001010;
    Serial.println("marchaAdelante |");
    marchaAdelante(1000,127);
    Avanzando =1;
  }
  if(input=='2'){
    digitalWrite(LED_BUILTIN,HIGH);
    //PORTB=B00000110;
    Serial.println("<- GiroIzquierda ");
    GiroIzquierda(1100,120);
    Avanzando = 0;
    input=0;
    paro(2000);    
  }
  if(input=='3'){
    digitalWrite(LED_BUILTIN,HIGH);
    //PORTB=B00001001;
    Serial.println("GiroDerecha ->");
    GiroDerecha(1100,120);
    Avanzando = 0;
    input=0;
    paro(2000);                                         // Detener Marcha durante 5 segundos.
  }
  if(input=='4'){
    digitalWrite(LED_BUILTIN,HIGH);
    Serial.println("Marcha_Atras /");
    //PORTB=B00000101;
    marchaAtras(500,VEL_MAX);                             // Marcha atras durante 2 segundos a media potencia    
    Avanzando =0;
  }
  if(input=='0'){
    digitalWrite(LED_BUILTIN, LOW);
    //PORTB=B00000000;
    Serial.println("Stop 1 seg");
    paro(1000);
    Avanzando =0;
  }
}
