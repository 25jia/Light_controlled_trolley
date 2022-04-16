/*
4个按键
FBLS
0000-停止信号:一直亮
1000-前进
0100-后腿
0010-左转
0001-右转
*/
/*这里程序的思路是发射不同的信号（数据流）来控制车辆的运动方式，
为了避免发射端和接收端不同频率扫描导致的数据传输问题，我们选择“计数”的方式实现，
用一段0信号作为每一组数据流的间隔，接收端通过检测每一组数据流中上升沿的个数来获得传输的信息。
另外，为了防止计数较小，较慢时噪声影响，我们将数据传输频率调高，计数最低值以4起步。
*/
/*
The idea of the program here is to transmit different signals (data streams) to 
control the motion mode of the vehicle,
In order to avoid the data transmission problems caused by different frequency 
scanning at the transmitter and receiver, we choose the "counting" method,
Using a segment of 0 signal as the interval of each group of data streams, 
the receiving end obtains the transmitted information by detecting the 
number of rising edges in each group of data streams.In addition, 
in order to prevent the influence of noise when the count is 
small and slow, we increase the data transmission frequency and start with 
the lowest count of 4.
*/
//Input pins, for 4 buttons
const int FORWARD_PIN=5;//Rising interupt
const int BACKWARD_PIN=4;//Rising interupt
const int LEFT_PIN=3;//'1'out,'0'no output
const int RIGHT_PIN=2;

//Output pins, for laser
const int LASER_PIN=13;

//veriables will change
int STOP=1;//initial state, the car is stop
int FORWARD=0;
int BACKWARD=0;
int LEFT=0;
int RIGHT=0;
int pulsewidth=0;

//pulsewidth, const
#define F 4
#define B 8
#define L 12
#define R 16
//#define S 400
#define period 1000//period 500ms
//#define duty_parameter 0.5

void setup() {
  // put your setup code here, to run once:
  pinMode(FORWARD_PIN, INPUT);
  pinMode(BACKWARD_PIN, INPUT);
  pinMode(LEFT_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
  pinMode(LASER_PIN, OUTPUT);
}

void reset(void){
  STOP=1;//initial state, the car is stop
  FORWARD=0;
  BACKWARD=0;
  LEFT=0;
  RIGHT=0;
  }

void loop() {
  // put your main code here, to run repeatedly:
  reset();
  FORWARD=digitalRead(FORWARD_PIN);
  BACKWARD=digitalRead(BACKWARD_PIN);
  LEFT=digitalRead(LEFT_PIN);
  RIGHT=digitalRead(RIGHT_PIN);
  if(FORWARD|BACKWARD|LEFT|RIGHT)STOP=0;
  if(STOP){
    digitalWrite(LASER_PIN, HIGH); // sets the LED on
    delay(period); // waits for a second
  }
   else{
   if(FORWARD) pulsewidth=F; 
   else if(BACKWARD) pulsewidth=B; 
   else if(LEFT) pulsewidth=L; 
   else if(RIGHT) pulsewidth=R; 
   else pulsewidth=0; 

  //int count=duty_parameter*period/pulsewidth/2;
  int count=pulsewidth;
  for(;count>0;count--){
   digitalWrite(LASER_PIN, HIGH); // sets the LED on
   delay(50); // waits for a second,80
   digitalWrite(LASER_PIN, LOW); // sets the LED off
   delay(100); // waits for a second,150
  }
   digitalWrite(LASER_PIN, LOW); // sets the LED off
   delay(200); // waits for a second,600
   }
}
  /*
void interupt_stop(int num){
  if(!STOP){reset();}
  else{
    reset();
    if(num)FORWARD=1;
    else BACKWARD=1;
    }
  }*/
