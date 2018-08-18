#include <reg51.h>
#include <absacc.h>
#define uchar unsigned char
#define uint unsigned int

void delayms(uint xms)		 //延时
{
	int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}

uchar code dispcode[]={0x3F,0x06,0x5B,0x4F,0x66,	//七段共阴管
						0x6D,0x7D,0x07,0x7F,0x6F,
						0xBF,0x86,0XCB,0XCF,0XEF,
						0XED,0XFD,0X87,0XFF,0XDF};

sbit P3_0=P3^0;	   //引脚
sbit P3_1=P3^1;
sbit P3_2=P3^2;
sbit P3_7=P3^7;
sbit P1_0=P1^0;
sbit P1_1=P1^1;
sbit alarm=P3^7;
uchar sec,min,hour,sec1,min1,hour1,mstcnt;

void time()		   //时间处理
{
	if(sec==60)
	{
		sec=0;
		min++;
	}
	if(min==60)
	{
		min=0;
		hour++;
	}
	if(hour==24)
	{
		hour=0;
	}
}

void display()				         //显示函数
{
	P2=0xfe;    
    P0=dispcode[hour/10];            //显示小时的十位    
    delayms(5);    
    P2=0xfd;     
    P0=dispcode[hour%10];          //显示小时的个位        
    delayms(5);     
    P2=0xfb;      
    P0=dispcode[min/10];          //显示分的十位    
    delayms(5);      
    P2=0xf7;       
    P0=dispcode[min%10];          //显示分的个位      
    delayms(5);      
    P2=0xef;
    P0=dispcode[sec/10];          //显示秒的十位     
    delayms(5);     
    P2=0xdf;     
    P0=dispcode[sec%10];          //显示秒的个位     
    delayms(5);
}

void keyscan()
{
	if(P3_0==0)
	{
		delayms(5);
		if(P3_0==0)
		{
			sec++;
			if(sec==60)
			{
				sec=0;
			}
		}
	}

	if(P3_1==0)
	{
		delayms(5);
		if(P3_1==0)
		{
			min++;
			if(min==60)
			{
				min=0;
			}
		}
	}
	if(P3_2==0)
	{
		delayms(5);
		if(P3_2==0)
		{
			hour++;
			if(hour==24)
			{
				hour=0;
			}
		}
	}
}

void timer0() interrupt 1
{
	TH0=(65536-50000)/256;			  //定时50ms
	TL0=(65536-50000)%256;
	TMOD=0X01;			  //2种定时器0001 0001
	mstcnt++;
	if(mstcnt==20)
	{	
		sec++;
		mstcnt=0;
	}
}
void time__()
{
	if(sec1==60)
		 {  sec1=0;  min1++;
		    if(min1==60)
			{  min1=0;  hour1++;
			  if(hour1==24)
			  {  hour1=0; }	}  }
}
void keyscan__()
{
	if(P3_0==0)
	{
		delayms(5);
		if(P3_0==0)
		{
			sec1++;
			if(sec1==60)
			{
				sec1=0;
			}
		}
	}

	if(P3_1==0)
	{
		delayms(5);
		if(P3_1==0)
		{
			min1++;
			if(min1==60)
			{
				min1=0;
			}
		}
	}
	if(P3_2==0)
	{
		delayms(5);
		if(P3_2==0)
		{
			hour1++;
			if(hour1==24)
			{
				hour1=0;
			}
		}
	}
}
void display__()				  //闹钟显示函数
{   
	P2=0xfe;    
    P0=dispcode[hour1/10];            //显示小时的十位    
    delayms(2);    
    P2=0xfd;     
    P0=dispcode[hour1%10];            //显示小时的个位        
    delayms(2);     
    P2=0xfb;      
    P0=dispcode[min1/10];          //显示分的十位    
    delayms(2);      
    P2=0xf7;       
    P0=dispcode[min1%10];          //显示分的个位      
    delayms(2);      
    P2=0xef;
    P0=dispcode[sec1/10];          //显示秒的十位     
    delayms(2);     
    P2=0xdf;     
    P0=dispcode[sec1%10];          //显示秒的个位     
    delayms(2);
}

void main()
{
	TH0=(65536-50000)/256;			  //定时50ms
	TL0=(65536-50000)%256;
	TMOD=0X01;
	EA=1;
	ET0=1; 
	TR0=1;
	while(1)
	{
		time();
		keyscan();
		display();
		if(P1_0==0)
		{
			time__();
			keyscan__();
			display__();
		}
		if(sec1==sec&&min1==min&&hour1==hour)
		{
			alarm=~alarm;
		}	
			
	}
}