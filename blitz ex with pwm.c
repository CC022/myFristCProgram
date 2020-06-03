 
/*-----------------------------------------------
  Project Blitz
  w86f
  2013.4
  ����������Ϊ9600������żУ�� ����11.0592MHz�����ͺͽ���ʹ�õĸ�ʽ��ͬ
  matrix keyboard ��λ��	
------------------------------------------------*/

#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���                        
#define KeyPort P1

sbit a1=P0^0;
sbit a2=P0^1;
sbit b1=P0^2;
sbit b2=P0^3;
sbit LED=P0^4;

/*------------------------------------------------
 ��������
 ------------------------------------------------*/
void SendStr(unsigned char *s);//���ڷ���
void DelayUs2x(unsigned char t);//us����ʱ��������
void DelayMs(unsigned char t); //ms����ʱ
void ConCheck(unsigned char t);//�����������

unsigned char KeyScan(void);
unsigned char KeyPro(void);//����ɨ��


unsigned int num,oflag,rflag,ahflag;


/*------------------------------------------------
                    ���ڳ�ʼ��
*/
void InitUART  (void)
{

    SCON  = 0x50;		        // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1   = 0xFD;               // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 ��                         
    EA    = 1;                  //�����ж�
    //ES    = 1;                  //�򿪴����ж�
}
/*------------------------------------------------
                    ������
------------------------------------------------*/
void main (void)
{   LED=1;
    InitUART();
    oflag=1;
    rflag=0;
	ahflag=0;
while (1)
    {
        num=KeyPro();    //ѭ�����ð���ɨ��
        switch(num)
        {
                
            case 2:if(ahflag==0){a1=1;a2=0;b1=1;b2=0;ahflag=1;DelayMs(36);}
					else{a1=0;a2=0;b1=1;b2=0;ahflag=0;DelayMs(2);}
                break;//ǰ��
            case 6:if(ahflag==0){a1=0;a2=1;b1=0;b2=1;ahflag=1;DelayMs(36);}
					else{a1=0;a2=0;b1=0;b2=1;ahflag=0;DelayMs(2);}//����
                break;
            case 5:a1=1;a2=0;b1=0;b2=1;DelayMs(10);//��
                break;
            case 7:a1=0;a2=1;b1=1;b2=0;DelayMs(10);//��
                break;
            case 1:a1=1;a2=0;b1=0;b2=0;DelayMs(10);//����
                break;
            case 3:a1=0;a2=0;b1=1;b2=0;DelayMs(10);//�ҵ���
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
                    ����һ���ֽ�
*/
void SendByte(unsigned char dat)
{
 SBUF = dat;
 while(!TI);
      TI = 0;
}
/*------------------------------------------------
                    ����һ���ַ���
*/
void SendStr(unsigned char *s)
{
 while(*s!='\0')// \0 ��ʾ�ַ���������־��
                //ͨ������Ƿ��ַ���ĩβ
  {
  SendByte(*s);
  s++;
  }
}
/*------------------------------------------------
 uS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��,������ʱ
 �������� T=tx2+5 uS
 ------------------------------------------------*/
void DelayUs2x(unsigned char t)
{
    while(--t);
}
/*------------------------------------------------
 mS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��
 ------------------------------------------------*/
void DelayMs(unsigned char t)
{
    
    while(t--)
    {
        //������ʱ1mS
        DelayUs2x(245);
        DelayUs2x(245);
    }
}

/*------------------------------------------------
 ����ɨ�躯��������ɨ���ֵ
 ------------------------------------------------*/
unsigned char KeyScan(void)  //����ɨ�躯����ʹ�����з�תɨ�跨
{
    unsigned char cord_h,cord_l;//����ֵ�м����
    KeyPort=0x0f;            //�������ȫΪ0
    cord_h=KeyPort&0x0f;     //��������ֵ
    if(cord_h!=0x0f)    //�ȼ�����ް�������
    {
        DelayMs(10);        //ȥ��
        if((KeyPort&0x0f)!=0x0f)
        {
            cord_h=KeyPort&0x0f;  //��������ֵ
            KeyPort=cord_h|0xf0;  //�����ǰ����ֵ
            cord_l=KeyPort&0xf0;  //��������ֵ
            
//while((KeyPort&0xf0)!=0xf0);//�ȴ��ɿ������
            ConCheck(cord_h+cord_l);
            return(cord_h+cord_l);//������������ֵ
        }
    }return(0xff);     //���ظ�ֵ
}
/*------------------------------------------------
 ����ֵ������������ɨ��ֵ
 ------------------------------------------------*/
unsigned char KeyPro(void)
{
    switch(KeyScan())
    {
        case 0x7e:return 1;break;// ������Ӧ�ļ���ʾ���Ӧ����ֵ
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
//������麯�������밴��ֵ���Դ�while
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

