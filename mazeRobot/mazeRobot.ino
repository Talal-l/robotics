
#include <Servo.h>

// definitions and declarations of variables of the robot code maze resolver;
Servo r;
Servo l;

long duracao_frente, duracao_esq, duracao_dir, b_frente, right, left, front, back;
int tri_f = 12;  // controls the pulse sent from the front sensor
int echo_f = 13; // controls the pulse received from the front sensor

int tri_l = 2;  // controls the sent left sensor pulse
int echo_l = 3; // controls the received pulse from the left sensor

int tri_r = A5;  // controls the forward sensor sent pulse
int echo_r = A4; // controls the received pulse from the front sensor

int tri_b = A1;
int echo_b = A0;

// pins to control the color sensor 
int s0 = 4;
int s1 = 5;
int s2 = 6;
int s3 = 7;
// pin used to read the output of the sensor
int sensorIn = A3 ;



//////////////////////////////////
//Forward With Speed
void ForWSpeed(int s)
{
  s = s * 2;
  r.writeMicroseconds(1500 - s);
  l.writeMicroseconds(1500 + s);
}
/////////////////////////////////////////////
//Stay

/////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// configuration of input types of declared variables;
void setup()
{
  Serial.begin(9600);

  r.attach(11);
  l.attach(9);
  pinMode(tri_f, OUTPUT); // arduino signal output from trigger_frente
  pinMode(echo_f, INPUT); // arduino signal input from echo_frente

  pinMode(tri_l, OUTPUT); // arduino signal output from trigger_frente
  pinMode(echo_l, INPUT); // arduino signal input from echo_frente

  pinMode(tri_r, OUTPUT); // triggered arduino signal output
  pinMode(echo_r, INPUT); // arduino signal input from echo_frente

  pinMode(tri_b, OUTPUT); // triggered arduino signal output
  pinMode(echo_b, INPUT);

  // set the color sensor pins
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(sensorIn, INPUT);
  
  // set frequency-scalling to 20% to insure that the pulse is detected 
  digitalWrite(s0,LOW);
  digitalWrite(s1,HIGH);


}
  // code in infinite repetition of the project;

  void loop()
  {
    
    read();
    
    
    if (front < 30) {
      // check wall
//      stop();
//      delay(1000);
      while(front > 5){
        moveForward(1);
        read();
      }
      checkWall();
    } 
    else if (right > 35){
      // turn right 
//      stop();
//      delay(1000);
      moveForward(25);
      turnRight();
    }

    
    else {
      //course correct
      courseCorrect();
      // move forward
      moveForward(1);
    }
  }

void courseCorrect(){

    if(right < 5){
      turnWithAngle(2);
    }
    else if(left < 5){
      turnWithAngle(-2);
    }
    
}
void stop()
{
  r.writeMicroseconds(1500);
  l.writeMicroseconds(1500);
}
//move forward with destance d
void moveForward(int d){
    // move by des
    int t = d * 36.6 * 1.5;
    int s = 200;
    r.writeMicroseconds(1300); // move right servo with speed 200(max)
    l.writeMicroseconds(1620); // move left servo with speed 120 to match the pace of the left servo
    delay(t);

}
/**
  *@desc turn with degree
  *@param int theta - the angle from the center line of the robot to the angle desired in the counterclockwise direction
*/
void turnWithAngle(int theta)
{
  int c = 9; //or 11 with the AAA batrays
  int t = 36.6 * 2 * theta / 180 * 3.1415 * c;
  int s = 200;
  if (t >= 0)
  {
    r.writeMicroseconds(1300); //the values of the servo are diffrent because of the same problem above
    l.writeMicroseconds(1400);
    delay(t);
  }
  else
  {
    r.writeMicroseconds(1700);
    l.writeMicroseconds(1600);
    delay(-1 * t);
  }
  stop();
}
  void turnRight(){

    // turn inplace
    turnWithAngle(-90);
    // move some small distance to enter between the walls
    moveForward(10);
    Serial.println("Turn Right");
  }

  void turnLeft(){

    // just turn left
    turnWithAngle(90);
    moveForward(10);
    Serial.println("Turn Left");
  }


void color(bool *b, bool *r)  
{    
// only read color if sensorIn is high
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);   
int  red = pulseIn(sensorIn, digitalRead(sensorIn) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);  
  //count OUT, pBLUE, BLUE  
  int blue = pulseIn(sensorIn, digitalRead(sensorIn) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);  
  //count OUT, pGreen, GREEN  
 int  green = pulseIn(sensorIn, digitalRead(sensorIn) == HIGH ? LOW : HIGH);  


    Serial.print("red freq= ");//printing name
    Serial.println(red);//printing RED color frequency


    Serial.print("green freq= ");//printing name
    Serial.println(green);//printing RED color frequency


    Serial.print("blue freq= ");//printing name
    Serial.println(blue);//printing RED color frequency

    *r = red < 500;
    *b = blue < 500;
    
    //delay(2000);
}



  void checkWall(){

    bool b = false, r = false;
    color(&b,&r);
    // if blue call turnRight
    if(b && !r){
      turnRight();
    }
    // else if red call turnLeft
    else if(r && !b){
      turnLeft();
    }
    // else if right open call turnRight
    else if(right > 30){
      turnRight();
    }
    // else if no right and left open call turnLeft
    else if(left > 30){
      turnLeft();
    }
    // else call turn180
    else{
      turn180();
    }

    delay(2000);
  }

  void turn180(){
    turnWithAngle(180);
    Serial.println("turn 180");
  }
  
  bool read()
  {
    duracao_frente = duracao_esq = duracao_dir = right = b_frente = back = left = front = 0;
    // Front
    digitalWrite(tri_f, LOW);               // their respective signal inputs and outputs are declared
    delayMicroseconds(2);                   // Ultrasonic sensor and stored by sensor variable
    digitalWrite(tri_f, HIGH);              // which converts the velocity of sound which is 340 m / s or
    delayMicroseconds(5);                   // 29 microseconds per centimeter, as the signal goes back and forth
    digitalWrite(tri_f, LOW);               // this time is half being sensor = time / 29/2;
    duracao_frente = pulseIn(echo_f, HIGH); // so also on the other two sensors.
    front = duracao_frente / 29 / 2;
    // Left
    digitalWrite(tri_l, LOW);
    delayMicroseconds(2);
    digitalWrite(tri_l, HIGH);
    delayMicroseconds(5);
    digitalWrite(tri_l, LOW);
    duracao_esq = pulseIn(echo_l, HIGH);
    left = duracao_esq / 29 / 2;

    // Right
    digitalWrite(tri_r, LOW);
    delayMicroseconds(2);
    digitalWrite(tri_r, HIGH);
    delayMicroseconds(5);
    digitalWrite(tri_r, LOW);
    duracao_dir = pulseIn(echo_r, HIGH);
    right = duracao_dir / 29 / 2;

    // Front
    digitalWrite(tri_b, LOW);         // their respective signal inputs and outputs are declared
    delayMicroseconds(2);             // Ultrasonic sensor and stored by sensor variable
    digitalWrite(tri_b, HIGH);        // which converts the velocity of sound which is 340 m / s or
    delayMicroseconds(5);             // 29 microseconds per centimeter, as the signal goes back and forth
    digitalWrite(tri_b, LOW);         // this time is half being sensor = time / 29/2;
    b_frente = pulseIn(echo_b, HIGH); // so also on the other two sensors.
    back = b_frente / 29 / 2;
    //Print
//    Serial.print("right : ");
//    Serial.println(right);
//    Serial.print("left:   ");
//    Serial.println(left);
//    Serial.print("front:   ");
//    Serial.println(front);
//    Serial.print("back:   ");
//    Serial.println(back);
    //delay(1000);
    return true;
  }


  //this method will make the robot turn if it finds a right turn and there is a str8 path
  /*
  void turnRight2()
  {
    read();
    moveWD(17);
    int dist = 10000;
    while (back > 10)
    {
      r.writeMicroseconds(1530);
      l.writeMicroseconds(1530);
      read();
    }
    while (back < dist)
    {
      if (back <= 30)
      {
        dist = back;
      }
      else
      {
        dist = 10000;
      }
      r.writeMicroseconds(1530);
      l.writeMicroseconds(1530);
      read();
    }
    S();
    //delay(2000);
    while (right > 12)
    {
      read();
      if (left < 9)
      {
        r.writeMicroseconds(1530);
        l.writeMicroseconds(1500);
      }
      else
        ForWSpeed(50);
    }
    S();
  }
  */
  
  //this method is for the 90 degree with a wall infront of the robot
  /*
  void turnRigh1()
  {
    read();
    while (front > 5)
    {
      read();
      ForWSpeed(50);
    }
    S();
    int dist = 10000;
    Serial.println("Front Stop!!");
    delay(2000);
    while (left < dist)
    {
      if (left <= 30)
      {
        dist = left;
      }
      else
      {
        dist = 10000;
      }
      r.writeMicroseconds(1530);
      l.writeMicroseconds(1530);
      read();
    }
    S();
    delay(2000);
    while (right > 12)
    {
      read();
      if (left < 9)
      {
        r.writeMicroseconds(1530);
        l.writeMicroseconds(1500);
      }
      else
        ForWSpeed(50);
    }
    S();
  }
  void turnLeft()
  {
    read();
    moveWD(16);
    int dist = 10000;
    while (back < dist)
    {
      if (back <= 30)
      {
        dist = back;
      }
      else
      {
        dist = 10000;
      }
      r.writeMicroseconds(1470);
      l.writeMicroseconds(1470);
      read();
    }

    //
    S();
    delay(2000);
    while (left > 12)
    {
      read();
      if (right < 9)
      {
        r.writeMicroseconds(1470);
        l.writeMicroseconds(1500);
      }
      else
        ForWSpeed(50);
    }
    S();
  }
  */

  //u will know from the name
  // // // // // // // // // // //
  //there is an issue with this function it might get to close to the
  /*
  void turn180()
  {
    while (front < 50)
    {
      read();
      r.writeMicroseconds(1470);
      l.writeMicroseconds(1470);
    }
    S();
    delay(1000);
    while (left > 4)
    {
      read();
      r.writeMicroseconds(1470);
      l.writeMicroseconds(1470);
    }
    S();
  }
  */
  
