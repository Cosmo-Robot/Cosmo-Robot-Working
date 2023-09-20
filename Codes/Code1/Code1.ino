###


//Program to 

//define the arduino pin numbers connected to motor

int L1 = 2;         

int L2 = 3;

int r1 = 4;

int r2 = 5;

int en1 = 9;

int en2 = 10;

// array to store five sensor values

int a[5] = {0, 0, 0, 0, 0};

int last_proportional = 0;

int integral = 0;

//function prototype of different functions

char select_turn(unsigned char found_left, unsigned char found_right, unsigned char found_st);

int mod(int v);

int set_motors(int a, int b);

void turn(char dir);

void PID();

int right = 0;

int left = 0;

//setup function to define the pin Mode is input or output

void setup() 

{

  pinMode(L1, OUTPUT);

  pinMode(L2, OUTPUT);

  pinMode(r1, OUTPUT);

  pinMode(r2, OUTPUT);

  pinMode(en1, OUTPUT);

  pinMode(en2, OUTPUT);

  Serial.begin(9600);

}

//infinite loop function

void loop() 

{

  PID();  // calling the pid function

  

  unsigned char found_left = 0;

  unsigned char found_right = 0;

  unsigned char found_st = 0;

  readline();  //calling readline function to read sensor values

  if(a[0] == HIGH)  // condition check for left senor is high or not

  {

    found_left = 1;

  }

  else if(a[0] == HIGH && a[1] == HIGH && a[2] == HIGH) // condition check  for left sensor is high 

  {

    found_left = 1;

  }

  else if(a[4] == HIGH && a[3] == HIGH)   // condition check for right sensor is high or not

  {

    found_right == 1;

  }

  if(a[2]== HIGH)    // condition check for middle sensor is high or not

  {

    found_st = 1;

  }

  unsigned char dir;

  dir = select_turn(found_left, found_right, found_st);

  turn(dir);

}

void PID()     // PID function definition

{

// Variable initialization

  int i;

  int power_difference = 0;

  float Kp, Ki, Kd;

  unsigned int position;

  int derivative,proportional;

  while(1) 

  {

    position = readline();            //reading the sensor value and storing in variable position

     Serial.println("position = ");

    Serial.println(position);

     //replace value 2000 with your position by placing your robot at the dead centre and read the value

    proportional = ((int)position - 2000);       derivative = proportional - last_proportional;

    integral = integral+proportional;

    last_proportional = proportional;

  // value of kp ki kd, you have to make changes to make your robot move without oscillation (hit and trial method)

    Kp = 0.08;

    Ki = 0.0002;

    Kd = 1.0;

  //formula for pid to calculate error (i.e power difference)

    power_difference = proportional * Kp + integral * Ki + derivative *Kd;

    const int max = 180;  //setting the maximum speed of motor

    if(power_difference>max)

     power_difference = max;

    if(power_difference < -max)

      power_difference = (-1*max);

      if(power_difference < 0)

      {

        //set_motors(max, max-power_difference);

        set_motors(max+power_difference, max);

      }

      else

      {

        //set_motors(max+power_difference, max);

        set_motors(max, max-power_difference);

      }

      readline();

      if(a[0] == LOW && a[1] == LOW && a[2] == LOW && a[3] == LOW && a[4] == LOW)

      return;

      else if(a[0]== HIGH || a[4] == HIGH)

      return;

  }

}

int readline()

{

//Read the sensor values

  a[0] = digitalRead(6);

  a[1] = digitalRead(7);

  a[2] = digitalRead(11);

  a[3] = digitalRead(12);

  a[4] = digitalRead(13);

  int v;

//calculating the average value of sensors

  v = (4000*a[0] + 3000*a[1] + 2000*a[2] + 1000*a[3] + 0*a[4])/(a[0] + a[1] + a[2] + a[3] + a[4]);

  Serial.println(v);

  return v; 

}

char select_turn(unsigned char found_left, unsigned char found_right, unsigned char found_st)

{

  if(found_left == 1)

  return 'L';

  if(found_st == 1)

  return 'S';

  if(found_right == 1)

  return 'R';

  else

  return 'B';

}

void turn(char dir)

{

  switch(dir)

  {

    case 'L':

    set_motors(0,150);

    delay(350);

    break;

    case 'R':

    set_motors(150,0);

    delay(350);

    break;

     case 'B':

    set_motors(-150,150);

    delay(200);

    break;

    case 'S':

    break;

  }

}

int set_motors(int l, int r)

{

  Serial.println(r);

  Serial.println(l);

  if(l>=0 && r>=0)

  {

    analogWrite(en1,mod(l));

    analogWrite(en2, mod(r));

    digitalWrite(L1, HIGH);

    digitalWrite(L2, LOW);

    digitalWrite(r1, HIGH);

    digitalWrite(r2, LOW);

  }

 else if(l<=0 && r>=0)

  {

    analogWrite(en1,mod(l));

    analogWrite(en2, mod(r));

    digitalWrite(L1, LOW);

    digitalWrite(L2, HIGH);

    digitalWrite(r1, HIGH);

    digitalWrite(r2, LOW);

  }

  else if(l>=0 && r<=0)

  {

    analogWrite(en1,mod(l));

    analogWrite(en2, mod(r));

    digitalWrite(L1, HIGH);

    digitalWrite(L2, LOW);

    digitalWrite(r1, LOW);

    digitalWrite(r2, HIGH);

  }

  else if(l==0 && r==0)

  {

    analogWrite(en1,0);

    analogWrite(en2, 0);

    digitalWrite(L1, LOW);

    digitalWrite(L2, LOW);

    digitalWrite(r1, LOW);

    digitalWrite(r2, LOW);

}

}

int mod(int v)

{

  if(v<0)

  return -1*v;

  else if(v>0)

  return v;

}

###
