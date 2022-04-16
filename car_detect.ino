/*******************************
analogPin填入输入信号端口x
digPin填入输出信号端口y（直接输入到控制端？）
x的输入使用AD转化器
 *******************************/
 /*这部分程序的功能是捕捉输入的激光，记录上升沿个数并分析对应的车辆动作。
 通过软件差分的方式将间隔输入的模拟信号差分（1024,5V），以排除噪声，
 即只有两组分开的二极管光强造成的电压差值会被作为有效的输入信号进行分析。
 记录上升沿个数并进行比较。
 对于上升沿以及不同数据流区分通过信号量方法和记录持续时间来实现。在检测到一个上升沿之后改变rise保证不会持续计数，
 同时，有delay时间计时开始记录本次的信号持续时间（1停止判断，0区分不同数据流）。
 另外，由于硬件原因，电压变化需要时间，对于模拟信号转数字信号的部分需要2个阈值，
 以保证在每次电压变化的开始被记录到。阈值在（1024/5）*25V和（1024/5）*205V，
 这样可以得到灵敏度较高的探测效果。
 */
 /*
The function of this part of the program is to capture the input laser, 
record the number of rising edges and analyze the corresponding vehicle action.
The analog signals input at intervals are differentiated by software (1024,5v) to eliminate noise,
That is, only the voltage difference caused by the light intensity of two 
separate groups of diodes will be analyzed as an effective input signal.
Record the number of rising edges and compare them.
For the rising edge and different data streams, the differentiation is 
realized by semaphore method and recording duration. 
Changing rise after detecting a rising edge ensures that the count will not continue,
At the same time, the delay time timer starts to record the signal duration of this 
time (1 stops judgment, 0 distinguishes different data streams).
In addition, due to hardware reasons, voltage change takes time, and 
two thresholds are required for the part from analog signal to digital signal,
To ensure that it is recorded at the beginning of each voltage change. 
The threshold is (1024 / 5) * 25V and (1024 / 5) * 205v,
In this way, the detection effect with high sensitivity can be obtained.
*/
//analog pin for input signal from laser
const int analogPin1=A4;//1
const int analogPin2=A6;//3
const int analogPin3=A5;//2
const int analogPin4=A7;//4
//LED pin for show situation
const int ledPin=13;
//wheel control pin
const int right_wheel_1=2;//右轮输出端口
const int right_wheel_2=3;
const int left_wheel_1=4;//左轮输出端口
const int left_wheel_2=5;

#define F 4//forward
#define B 8//backward
#define L 12//left
#define R 16//right
void do_forward();
void do_backward();
void do_left();
void do_right();
void do_stop();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(analogPin1,INPUT);
  //pinMode(analogPin2,INPUT);
  pinMode(right_wheel_1,OUTPUT);
  pinMode(right_wheel_2,OUTPUT);
  pinMode(left_wheel_1,OUTPUT);
  pinMode(left_wheel_2,OUTPUT);
  pinMode(ledPin,OUTPUT);
  do_stop();
  //digitalWrite(test,LOW);
}

  int count1=0;//记1的个数
  int rise=1;//上升沿
  int count1_1=0;//记1持续时间，超过多少stop
  int count2=0;//记1之间的时间
void loop() {
  // put your main code here, to run repeatedly:
  //deal with the four inputs for a final value laser on1 off0
  int sensorValue1,sensorValue2,sensorValue3,sensorValue4;
  int diff1,diff2,datain;
  //read analogy pin value 0-1023 <= 0-5V
  sensorValue1 = analogRead(analogPin1);//4
  sensorValue2 = analogRead(analogPin2);//6
  sensorValue3 = analogRead(analogPin3);//5
  sensorValue4 = analogRead(analogPin4);//7
  diff1=(sensorValue1>sensorValue3)?sensorValue1-sensorValue3:sensorValue3-sensorValue1;
  diff2=(sensorValue2>sensorValue4)?sensorValue2-sensorValue4:sensorValue4-sensorValue2;
  //datain=((diff1>15)|(diff2>15));
  //if(datain) digitalWrite(ledPin, HIGH);
  //else digitalWrite(ledPin, LOW);
  datain=((diff1>25)|(diff2>25));
  //count the 1 signal until 0000--come
  if(datain==1){
    digitalWrite(ledPin,HIGH);//for test
    count2=0;
    if(rise){count1++; rise=0;}//how many 1 for 1 data sequence
    count1_1++; //记录一次1的持续时间
    delay(1);
    if(count1_1>80){//300ms 1 data
      count1=0;
      count1_1=0;
      do_stop(); 
      }
    datain=((diff1>205)|(diff2>205));
  }
  else{
    digitalWrite(ledPin,LOW);//for test
    rise=1; count1_1=0;
    count2++;//记录一次0的持续时间,与count1_1等价
    delay(1);
    if(count2>120){// data sequence fnish
      if(2<count1&&count1<=F) do_forward();
      else if(F+1<count1&&count1<=B) do_backward();
      else if(B+1<count1&&count1<=L) do_right();
      else if(count1>L) do_left();
      count2=0;
      count1=0;//begin to receive another data sequence
    }
  }
}

void do_left(){
  digitalWrite(left_wheel_1,HIGH);//pinwrite()
  digitalWrite(left_wheel_2,LOW);
  digitalWrite(right_wheel_1,LOW);
  digitalWrite(right_wheel_2,HIGH);
  //delay(200); do_stop();//新加的行
}

void do_right(){
  digitalWrite(left_wheel_1,LOW);
  digitalWrite(left_wheel_2,HIGH);
  digitalWrite(right_wheel_1,HIGH);//pinwrite()
  digitalWrite(right_wheel_2,LOW);
  //delay(200); do_stop();//新加的行，每次只执行2s中，在去检索新的信号
}

void do_forward(){
  digitalWrite(left_wheel_1,HIGH);//pinwrite()
  digitalWrite(left_wheel_2,LOW);
  digitalWrite(right_wheel_1,HIGH);
  digitalWrite(right_wheel_2,LOW);
  delay(1000); do_stop();//新加的行
}

void do_backward(){
  digitalWrite(left_wheel_1,LOW);
  digitalWrite(left_wheel_2,HIGH);//pinwrite()
  digitalWrite(right_wheel_1,LOW);
  digitalWrite(right_wheel_2,HIGH);
  delay(1000); do_stop();//新加的行
}

void do_stop(){
  digitalWrite(left_wheel_1,LOW);
  digitalWrite(left_wheel_2,LOW);//pinwrite()
  digitalWrite(right_wheel_1,LOW);
  digitalWrite(right_wheel_2,LOW);
}
