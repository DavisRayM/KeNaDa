#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

const int trigPin  = 22 ; // Sonar sensor pin Trigger
const int echoPin  = 23 ; // Sonar sensor pin Echo
long duration ; // Sonar Duration Variable

const int pathS1     =  52;        
const int pathS2     =  50;
const int pathS3     =  48;
const int pathS4     =  46;
const int pathS5     =  44;   
const int touchPin   =  42;    // Touch Sensor on the 5 Channel Line Tracker Sensor.
int S1, S2, S3, S4, S5, S6 ;

const int Motor1A = 2;
const int Motor1B = 3; 
        
const int Motor2A = 5; 
const int Motor2B = 4;

const int PWM1 = 6;
const int PWM2 = 7;

const int ledLeft  = 31;    // signals as the robot turns Left....
const int ledRight = 30;    // signals as the robot turns Right....

const int buzzerPin   = 10;

#define I2C_ADDR          0x3F        //Define I2C Address
#define BACKLIGHT_PIN      3
#define En_pin             2
#define Rw_pin             1
#define Rs_pin             0
#define D4_pin             4
#define D5_pin             5
#define D6_pin             6
#define D7_pin             7

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void setup() {
      Serial.begin(9600); // connect serial 
      
      pinMode(Motor1A, OUTPUT);
      pinMode(Motor1B, OUTPUT);
      pinMode(Motor2A, OUTPUT);
      pinMode(Motor2B, OUTPUT);
      
      pinMode(PWM1, OUTPUT);
      pinMode(PWM2, OUTPUT);

      pinMode( pathS1 , INPUT);
      pinMode( pathS2 , INPUT);
      pinMode( pathS3 , INPUT);
      pinMode( pathS4 , INPUT);
      pinMode( pathS5 , INPUT);
      pinMode( touchPin , INPUT);
      
      pinMode( ledLeft  , OUTPUT);
      pinMode( ledRight , OUTPUT);
      pinMode( buzzerPin   , OUTPUT);

      digitalWrite( ledLeft  , LOW );
      digitalWrite( ledRight , LOW );
      digitalWrite( buzzerPin   , LOW );

      digitalWrite(Motor1A, LOW); 
      digitalWrite(Motor1B, LOW); 
      digitalWrite(Motor2A, LOW); 
      digitalWrite(Motor2B, LOW); 

      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
 
      lcd.begin (20,4);
      
      //Switch on the backlight
      lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
      lcd.setBacklight(HIGH);
      
      //goto first column (column 0) and first line (Line 0)
      lcd.setCursor(0,0);
      
      lcd.print(" -RIARA UNIVERSITY- ");
      
      //goto first column (column 0) and second line (line 1)
      lcd.setCursor(0,1);
      lcd.print("  Robotics Contest  ");
  
      lcd.setCursor(0,2);
      lcd.print(" - - - - -- - - - - ");
  
      lcd.setCursor(0,3);
      lcd.print(" - - - - -- - - - - ");

      delay(5000);
      
      lcd.setCursor(0,2);
      lcd.print(" -ROBOT NAVIGATING- ");

      analogWrite(buzzerPin, 0);

      searchPath();

      
}

void loop() {
  
  while( checkObjectDistance() > 20   )

     {

        digitalWrite(ledRight, LOW);   //  Switch off signals 
        digitalWrite(ledLeft,  LOW);
      
        readpathsensor();
        
        // Robot moving on the path - While there is no object on the path.  
        while (S1 == 0 && S2 == 1 && S3 == 1 && S4 == 1 && S5 == 0 &&( checkObjectDistance() > 20   ))
              {
               digitalWrite(ledRight, LOW); 
               digitalWrite(ledLeft,  LOW);
                               
               analogWrite(PWM1, 200);
               analogWrite(PWM2, 180);
                
               lcd.setCursor(0,1);
               lcd.print("                    ");
               lcd.setCursor(0,2);
               lcd.print("       ON PATH      ");
               lcd.setCursor(0,3);
               lcd.print("                    ");
          
               
               digitalWrite(Motor1A, LOW);     //  
               digitalWrite(Motor1B, HIGH);    // 
               digitalWrite(Motor2A, LOW);     // 
               digitalWrite(Motor2B, HIGH);    // 

               readpathsensor();
                   
                
               }
        // Robot deviating to the left

 
        while ((S1 == 0 && S2 == 1 && S3 == 1 && S4 == 1 && S5 == 1 ) || (S1 == 0 && S2 == 0 && S3 == 1 && S4 == 1 && S5 == 1 ) || (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 1 && S5 == 1 ) || (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 1 )&&( checkObjectDistance() > 20   ))
              {
                  digitalWrite(ledRight, HIGH);
                  digitalWrite(ledLeft,  LOW); 
                  
                  analogWrite(PWM1, 80);
                  analogWrite(PWM2, 145);      
                            
                  lcd.setCursor(0,2);
                  lcd.print("   Deviating Left   ");  
                     
                  digitalWrite(Motor1A, LOW);      //  
                  digitalWrite(Motor1B, HIGH);      //  
                  digitalWrite(Motor2A, LOW);      //  
                  digitalWrite(Motor2B, HIGH);     // 
                  
                  readpathsensor();          
               }
               
       // Robot deviating to the Right
         while ( ( S1 == 1 && S2 == 1 && S3 == 1 && S4 == 1 && S5 == 0 ) || ( S1 == 1 && S2 == 1 && S3 == 1 && S4 == 0 && S5 == 0 ) || ( S1 == 1 && S2 == 1 && S3 == 0 && S4 == 0 && S5 == 0 )  || ( S1 == 1 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 0 )&&( checkObjectDistance() > 20   ))
            
               { 
                  digitalWrite(ledRight, LOW );    
                  digitalWrite(ledLeft,  HIGH); 
                  
                  analogWrite(PWM1, 130);
                  analogWrite(PWM2, 60);     
                          
                  lcd.setCursor(0,2);
                  lcd.print("  Deviating Right   "); 
                          
                  digitalWrite(Motor1A, LOW);      //  
                  digitalWrite(Motor1B, HIGH);     // 
                  digitalWrite(Motor2A, LOW);      // 
                  digitalWrite(Motor2B, HIGH);      // 
                  
                  readpathsensor();                       
                }


                      
        
           if (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 0 )
              
                { 
                    digitalWrite(ledRight, LOW);   
                    digitalWrite(ledLeft,  LOW);
                    
                    lcd.setCursor(0,2);
                    lcd.print("   -- NO PATH --    ");  
                      
                    digitalWrite(Motor1A, LOW);      
                    digitalWrite(Motor1B, LOW);     
                    digitalWrite(Motor2A, LOW);     
                    digitalWrite(Motor2B, LOW);       
                    
                    robotTurnRight();
                    readpathsensor();                   
        
                }
        

        
        // No path available - The robot should not move
           if (S1 == 1 && S2 == 1 && S3 == 1 && S4 == 1 && S5 == 1 )
           
                {   
                    digitalWrite(ledRight, LOW);   
                    digitalWrite(ledLeft,  LOW);
                    
                    lcd.setCursor(0,2);
                    lcd.print(" -PATH NOT DEFINED- ");       
                     
                    digitalWrite(Motor1A, LOW);      
                    digitalWrite(Motor1B, LOW);     
                    digitalWrite(Motor2A, LOW);     
                    digitalWrite(Motor2B, LOW);     

   
                    
                    readpathsensor();
                    robotTurnRight();

          
                }

     }
     

        objectFound();           

}

int checkObjectDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  long cm = (duration/2) / 29.1;
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  return cm;
}

void hoot(){
    tone(buzzerPin, 400, 1000);         
    delay(300);
    tone(buzzerPin, 450, 1000);
    delay(300);          
    tone(buzzerPin, 500, 1500);
    noTone(buzzerPin);
}

void readpathsensor(){
  
   S1 = digitalRead(pathS1);
   S2 = digitalRead(pathS2);
   S3 = digitalRead(pathS3);
   S4 = digitalRead(pathS4);
   S5 = digitalRead(pathS5);

   return S1, S2, S3, S4, S5;
}


void brake(){
         digitalWrite(Motor1A, HIGH);       
         digitalWrite(Motor1B, HIGH);      
         digitalWrite(Motor2A, HIGH);     
         digitalWrite(Motor2B, HIGH);    
}

void robotTurnRight(){

  analogWrite(PWM1, 160);
  analogWrite(PWM2, 160);
        
  digitalWrite(Motor1A, HIGH);     //  
  digitalWrite(Motor1B, LOW);      // 
  digitalWrite(Motor2A, LOW);      // 
  digitalWrite(Motor2B, HIGH);     // 
  
  delay(200);
  
  readpathsensor();
  
  while  ( S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 0 ) 
  
     { 
        digitalWrite(ledRight, LOW );    // 
        digitalWrite(ledLeft,  HIGH);    //   signaling the robot turning Left  
        
        analogWrite(PWM1, 160);
        analogWrite(PWM2, 160);             
        lcd.setCursor(0,2);
        lcd.print("   Turning Right    ");       
        
        digitalWrite(Motor1A, HIGH);     //  
        digitalWrite(Motor1B, LOW);      // 
        digitalWrite(Motor2A, LOW);      // 
        digitalWrite(Motor2B, HIGH);     //  
  
        readpathsensor(); 
      }  
          delay(200);                   
          
          digitalWrite(Motor1A, LOW);      //  
          digitalWrite(Motor1B, LOW);      // 
          digitalWrite(Motor2A, LOW);      // 
          digitalWrite(Motor2B, LOW);      // 
  
          delay(100);                   
  
          return;   
          
}

void robotTurnLeft(){         
           analogWrite(PWM1, 160);
           analogWrite(PWM2, 160);
  
           digitalWrite(Motor1A, LOW);     //  
           digitalWrite(Motor1B, HIGH);      // 
           digitalWrite(Motor2A, HIGH);      // 
           digitalWrite(Motor2B, LOW);     // 
  
           delay(200);
  
           readpathsensor();
  
           while  ( S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 0 ) 
            
               { 
                  digitalWrite(ledRight, HIGH );
                  digitalWrite(ledLeft,  LOW);
                     
                  analogWrite(PWM1, 180);
                  analogWrite(PWM2, 200);             
                  lcd.setCursor(0,2);
                  lcd.print("   Turning Left    ");       
                   
                  digitalWrite(Motor1A, LOW);     //  
                  digitalWrite(Motor1B, HIGH);      // 
                  digitalWrite(Motor2A, HIGH);      // 
                  digitalWrite(Motor2B, LOW);     // 
  
                  readpathsensor(); 
                  }   
                    delay(200);
                    
                    digitalWrite(Motor1A, LOW);      //  
                    digitalWrite(Motor1B, LOW);      // 
                    digitalWrite(Motor2A, LOW);      // 
                    digitalWrite(Motor2B, LOW);      // 
  
                    delay(100);          
  
                    return;
}

void searchPath(){
  
          lcd.setCursor(0,1);
          lcd.print("Robot searching path");
          
          lcd.setCursor(0,2);
          lcd.print(" .................. ");
          
          lcd.setCursor(0,3);
          lcd.print("  - PLEASE WAIT. -  ");

          readpathsensor();

          if(S1 == 0 && S2 == 1 && S3 == 1 && S4 == 1 && S5 == 0){
            brake();
            readpathsensor();
            return;
          }

          while(checkObjectDistance > 20){
            readpathsensor();
            if(S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 0 && (checkObjectDistance > 20)){
              analogWrite(PWM1, 160);
              analogWrite(PWM2, 160);
           
              digitalWrite(Motor1A, LOW);     //  
              digitalWrite(Motor1B, HIGH);    // 
              digitalWrite(Motor2A, LOW);     // 
              digitalWrite(Motor2B, HIGH);    //
              
              readpathsensor();
              
            }

            if (S1 == 1 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 0 && (checkObjectDistance > 20) )  
                   
                    {                        
                        while( S3 != 1  )
                      
                        { 
                          analogWrite(PWM1, 160);
                          analogWrite(PWM2, 160);
                          digitalWrite(Motor1A, LOW);     //  
                          digitalWrite(Motor1B, HIGH);    //  
                          digitalWrite(Motor2A, LOW);     //  
                          digitalWrite(Motor2B, HIGH);     //
  
                          readpathsensor();
                       }
                       
                       brake();
                       return;
                    }
             if (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 1 && (checkObjectDistance > 20))  
                   
                    {   
                     while( S3 != 1  )
                      
                        { 
                            analogWrite(PWM1, 160);
                            analogWrite(PWM2, 160);
                          digitalWrite(Motor1A, LOW);      
                          digitalWrite(Motor1B, HIGH);     
                          digitalWrite(Motor2A, LOW);      
                          digitalWrite(Motor2B, HIGH);    
  
                          readpathsensor();
                       }
                       
                       brake();
                       return;

                    }



            if (S1 == 1 && S2 == 1 && S3 == 1 && S4 == 1 && S5 == 1&& (checkObjectDistance > 20) )  
                   
                    {   
                         brake();
                         robotTurnRight();
                         readpathsensor();
                         return;
                    }
                   

          if (S1 == 0 && S2 == 1 && S3 == 1 && S4 == 1 && S5 == 0 && (checkObjectDistance > 20) )  
         
          {   
               brake();
               readpathsensor();
               return;
          }
          
     }

     while( checkObjectDistance < 19){
      
          lcd.setCursor(0,1);
          lcd.print("                    ");
          
          lcd.setCursor(0,2);
          lcd.print("-! OBJECT ON PATH !-");

          lcd.setCursor(0,3);
          lcd.print("                    ");
          
          brake();
          hoot();
          robotTurnLeft();
          delay(200);
          readpathsensor();
     }
}

void objectFound(){
                  // If the Robot finds and oject on the path, It should stop moving, diplay, and hoot 
                  // Then should Resume path navigation once the path is cleared.
          while( checkObjectDistance() < 19  )
         
         {

          lcd.setCursor(0,1);
          lcd.print("                    ");
          
          lcd.setCursor(0,2);
          lcd.print("-! OBJECT ON PATH !-");

          lcd.setCursor(0,3);
          lcd.print("                    ");

          brake();
          hoot();         
          delay(200);
         }       
}

