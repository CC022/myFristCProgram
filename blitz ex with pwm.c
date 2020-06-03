 
/*-----------------------------------------------
  Project Blitz
  w86f
  2013.4
  波特率设置为9600，无奇偶校验 晶振11.0592MHz，发送和接收使用的格式相同
  matrix keyboard 上位机	
------------------------------------------------*/

#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义                        
#define KeyPort P1

sbit a1=P0^0;
sbit a2=P0^1;
sbit b1=P0^2;
sbit b2=P0^3;
sbit LED=P0^4;

/*------------------------------------------------
 函数声明
 ------------------------------------------------*/
void SendStr(unsigned char *s);//串口发送
void DelayUs2x(unsigned char t);//us级延时函数声明
void DelayMs(unsigned char t); //ms级延时
void ConCheck(unsigned char t);//连续按键检查

unsigned char KeyScan(void);
unsigned char KeyPro(void);//键盘扫描


unsigned int num,oflag,rflag,ahflag;


/*------------------------------------------------
                    串口初始化
*/
void InitUART  (void)
{

    SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 打开                         
    EA    = 1;                  //打开总中断
    //ES    = 1;                  //打开串口中断
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{   LED=1;
    InitUART();
    oflag=1;
    rflag=0;
	ahflag=0;
while (1)
    {
        num=KeyPro();    //循环调用按键扫描
        switch(num)
        {
                
            case 2:if(ahflag==0){a1=1;a2=0;b1=1;b2=0;ahflag=1;DelayMs(36);}
					else{a1=0;a2=0;b1=1;b2=0;ahflag=0;DelayMs(2);}
                break;//前进
            case 6:if(ahflag==0){a1=0;a2=1;b1=0;b2=1;ahflag=1;DelayMs(36);}
					else{a1=0;a2=0;b1=0;b2=1;ahflag=0;DelayMs(2);}//后退
                break;
            case 5:a1=1;a2=0;b1=0;b2=1;DelayMs(10);//右
                break;
            case 7:a1=0;a2=1;b1=1;b2=0;DelayMs(10);//左
                break;
            case 1:a1=1;a2=0;b1=0;b2=0;DelayMs(10);//左单轮
                break;
            case 3:a1=0;a2=0;b1=1;b2=0;DelayMs(10);//右单轮
                break;
            case 4:if(oflag==0){SendStr("#1P1100T100\r\n");oflag=1;LED=0;}
            else{SendStr("#1P790T100\r\n");oflag=0;LED=1;}
                break;
            case 10:
                break;
            case 8:if(rflag==0){SendStr("#2P2250T100\r\n");rflag=1;}
            else{SendStr("#2P620T100\r\n");rflag=0;}
                break;
            case 9:SendStr("#3P1550#4P2500T1000\r\n");
                break;
            case 11:SendStr("#3P1322#4P2500T1000\r\n");
                break;
            case 12:SendStr("#3P1633#4P2278T1000\r\n");
                break;
            case 13:SendStr("#3P1400#4P2500T1000\r\n");
                break;
            case 14:SendStr("#3P2300#4P2500T1000\r\n");
                break;
            case 15:SendStr("#3P1767#4P2100T1000\r\n");
                break;
            case 16:SendStr("#3P2500#4P1322T1000\r\n");
                break;
            case 0:a1=0;a2=0;b1=0;b2=0;
                break;
	        default:break;
        }//switch    
    }//while
}//main

/*------------------------------------------------
                    发送一个字节
*/
void SendByte(unsigned char dat)
{
 SBUF = dat;
 while(!TI);
      TI = 0;
}
/*------------------------------------------------
                    发送一个字符串
*/
void SendStr(unsigned char *s)
{
 while(*s!='\0')// \0 表示字符串结束标志，
                //通过检测是否字符串末尾
  {
  SendByte(*s);
  s++;
  }
}
/*------------------------------------------------
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS
 ------------------------------------------------*/
void DelayUs2x(unsigned char t)
{
    while(--t);
}
/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
 ------------------------------------------------*/
void DelayMs(unsigned char t)
{
    
    while(t--)
    {
        //大致延时1mS
        DelayUs2x(245);
        DelayUs2x(245);
    }
}

/*------------------------------------------------
 按键扫描函数，返回扫描键值
 ------------------------------------------------*/
unsigned char KeyScan(void)  //键盘扫描函数，使用行列反转扫描法
{
    unsigned char cord_h,cord_l;//行列值中间变量
    KeyPort=0x0f;            //行线输出全为0
    cord_h=KeyPort&0x0f;     //读入列线值
    if(cord_h!=0x0f)    //先检测有无按键按下
    {
        DelayMs(10);        //去抖
        if((KeyPort&0x0f)!=0x0f)
        {
            cord_h=KeyPort&0x0f;  //读入列线值
            KeyPort=cord_h|0xf0;  //输出当前列线值
            cord_l=KeyPort&0xf0;  //读入行线值
            
//while((KeyPort&0xf0)!=0xf0);//等待松开并输出
            ConCheck(cord_h+cord_l);
            return(cord_h+cord_l);//键盘最后组合码值
        }
    }return(0xff);     //返回该值
}
/*------------------------------------------------
 按键值处理函数，返回扫键值
 ------------------------------------------------*/
unsigned char KeyPro(void)
{
    switch(KeyScan())
    {
        case 0x7e:return 1;break;// 按下相应的键显示相对应的码值
        case 0x7d:return 2;break;//
        case 0x7b:return 3;break;//
        case 0x77:return 4;break;//
        case 0xbe:return 5;break;//
        case 0xbd:return 6;break;//
        case 0xbb:return 7;break;//
        case 0xb7:return 8;break;//
        case 0xde:return 9;break;//
        case 0xdd:return 10;break;//
        case 0xdb:return 11;break;//
        case 0xd7:return 12;break;//
        case 0xee:return 13;break;//
        case 0xed:return 14;break;//
        case 0xeb:return 15;break;//
        case 0xe7:return 16;break;//
        default:return 0;break;
    }
}
//连续检查函数，输入按键值，自带while
void ConCheck(unsigned char t)
{
	switch(t)
	{
		case 0x7d: break;//2
    	case 0xbe: break;//5
		case 0x7e: break;//1
    	case 0x7b: break;//3
		case 0xbb: break;//7
		case 0xbd: break;//6
		default: while((KeyPort&0xf0)!=0xf0);break;
	}
}

