/*case 'F': goForward(); break;
        case 'L': turnLeft(); break;
        case 'B': goBack(); break;
        case 'R': turnRight(); break;
        case 'S': stop(); break;
        case 'M': turnOnLed(); break;
        case 'm': turnOffLed(); break;
        case 'N': turnOnLed2(); break;
        case 'n': turnOffLed2(); break;
        case 'J':
*/

#include <BluetoothSerial.h> 
BluetoothSerial ESP_BT;

#define r_pwm 13
#define l_pwm 32
#define RF 33
#define LF 27
#define RB 25
#define LB 26


int incoming;
int speed = 150;
long duration;
float distanceCm;


void stopC(){
  digitalWrite(RF, LOW);
  digitalWrite(LB, LOW);
  digitalWrite(LF, LOW);
  digitalWrite(RB, LOW);                     
}

void setup(){
  Serial.begin(9600);
  ESP_BT.begin("roboS");

  pinMode(RF, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(RB, OUTPUT);
  digitalWrite(RF, LOW);
  digitalWrite(LB, LOW);
  digitalWrite(LF, LOW);
  digitalWrite(RB, LOW);    
  Serial.begin(115200);
}

void loop(){
  if(ESP_BT.available()){
    incoming = ESP_BT.read();

if(incoming == 'S'){  // stop all
        digitalWrite(RF,LOW);
        digitalWrite(LB,LOW);
        digitalWrite(LF,LOW);
        digitalWrite(RB,LOW); 
      }
    
      if(incoming == 'B'){ // back
        digitalWrite(RF,LOW);
        digitalWrite(LB,HIGH);
        digitalWrite(LF,LOW);
        digitalWrite(RB,HIGH);
        analogWrite(l_pwm,speed);
        analogWrite(r_pwm,speed);                    
      }
            
      if(incoming == 'F'){
            digitalWrite(RF, HIGH);
            digitalWrite(LB, LOW);
            digitalWrite(LF, HIGH);
            digitalWrite(RB, LOW);  
            analogWrite(l_pwm, speed);
            analogWrite(r_pwm, speed);  
        
         }                  
      }
      if (incoming == 'H'){ // back left
         digitalWrite(RF, HIGH);
         digitalWrite(LB, HIGH);
         digitalWrite(LF, LOW);
         digitalWrite(RB, LOW);  
         analogWrite(l_pwm, speed);
         analogWrite(r_pwm, speed);                      
      }
      if(incoming == 'L'){ // left
        digitalWrite(RF, HIGH);
        digitalWrite(LB, LOW);
        digitalWrite(LF, HIGH);
        digitalWrite(RB, LOW);  
        analogWrite(l_pwm, 150);
        analogWrite(r_pwm, 255);                      
      }
      if(incoming == 'G'){  //forward left
        digitalWrite(RF,HIGH);
        digitalWrite(LB,LOW);
        digitalWrite(LF,HIGH);
        digitalWrite(RB,LOW);  
        analogWrite(l_pwm,150);
        analogWrite(r_pwm,255);                      
       }
      if(incoming == 'J'){ //backright
        digitalWrite(RF,LOW);
        digitalWrite(LB,LOW);
        digitalWrite(LF,HIGH);
        digitalWrite(RB,HIGH);    
        analogWrite(l_pwm,speed);
        analogWrite(r_pwm,speed);                    
       }
      if(incoming == 'R'){ // right
        digitalWrite(RF,HIGH);
        digitalWrite(LB,LOW);
        digitalWrite(LF,HIGH);
        digitalWrite(RB,LOW);    
        analogWrite(l_pwm,255);
        analogWrite(r_pwm,150);                    
      }
      if (incoming == 'I'){ // forward right
        digitalWrite(RF,HIGH);
        digitalWrite(LB,LOW);
        digitalWrite(LF,HIGH);
        digitalWrite(RB,LOW);    
        analogWrite(l_pwm,255);
        analogWrite(r_pwm,190);                    
      }          
      
}