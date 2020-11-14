#include "remote.h"
#include "sys.h"

void Remote_Init(void) {
	RCC->APB1ENR|=1<<3;     //TIM5时钟使能
	RCC->APB2ENR|=1<<2;     //GPIOA使能
	GPIOA->CRL&=0xFFFFFF0F; //PA1引脚刷0
	GPIOA->CRL|=0x00000080; //PA1配置为上拉
	GPIOA->ODR|=1<<1;       //配置为上拉
	TIM5->ARR=10000;        //设置计数器重装载值
	TIM5->PSC=71;           //72分频
	TIM5->CCMR1|=1<<8;
	TIM5->CCMR1|=3<<12;
	TIM5->CCMR1|=0<<10;
	TIM5->CCER|=0<<5;
	TIM5->CCER|=1<<4;       
	TIM5->DIER|=1<<2;       //允许中断  
	TIM5->DIER|=1<<0;       //允许更新中断
	TIM5->CR1|=0x01;        //使能定时器5
	MY_NVIC_Init(1,3,TIM5_IRQn,2);
	
}

u8 RmtSta=0;
u16 Dval;        //下降沿捕获值
u32 RmtRec=0;    //红外接受到的数据
u8  RmtCnt=0;    //统计红外按下的次数

void TIM5_IRQHandler(void) {
	u16 tsr;
	tsr=TIM5->SR;                    //获取当前定时器状态
	if(tsr&0x01) {                   //触发了溢出中断
		if(RmtSta&0x80) {            //如果接受到了引导标识符
			RmtSta&=~0x10;           //取消上升沿
			if((RmtSta&0x0F)==0x00)  //第一次溢出中断
				RmtSta|=1<<6;        //表示接受到全部信息
			if((RmtSta&0x0F)<14)     //如果溢出次数少于14则就继续进行溢出次数统计
				RmtSta++;            
			else {                   
				RmtSta&=~(1<<7);     //清空引导标识符
				RmtSta&=0xF0;        //清空计数器
			}
		}
	}
	if(tsr&(1<<2)) {                 //边沿触发中断
		if(RDATA) {                  //上升沿触发
			TIM5->CCER|=1<<5;        //改变为下降沿触发
			TIM5->CNT=0;             //清空定时器值
			RmtSta|=0x10;            //标记上升沿已被捕获
		} else {
			Dval=TIM5->CCR2;         //获取当前数值
			TIM5->CCER&=~(1<<5);     //改变为上升沿触发
			if(RmtSta&0x10) {
				if(RmtSta&0x80) {    //表示已经接受到了引导码
					if(Dval>300&&Dval<800) {
						RmtRec<<=1;  //表示接受到0
					} else if(Dval>1400&&Dval<1800) {
						RmtRec<<=1;
						RmtRec|=1;   //表示接受到1
					} else if(Dval>2200&&Dval<2600) {
						RmtCnt++;
						RmtSta&=0xF0;//清空计数器
					} 
				} else if(Dval>4200&&Dval<4700) {  //接受到了引导码
					RmtSta|=1<<7;
					RmtCnt=0;
				}	 
			}
			RmtSta&=~(1<<4);         //重新开始判断是否接受到上边沿
		}
		
	}
	TIM5->SR=0;                      //清除中断标志位
}

u8  Remote_Scan(void) {
	u8 sta=0;
	u8 t1,t2;
	if(RmtSta&(1<<6)) {             //已经接受到了一个按键的所有信息
		t1=RmtRec>>24;              //得到地址码
		t2=(RmtRec>>16)&0xff;       //得到地址反码
		if(t1==(u8)~t2) {           
			t1=RmtRec>>8;
			t2=RmtRec;
			if(t1==(u8)~t2) sta=t1; //判断键值
		} 
		if((sta==0)||(RmtSta&0x80)==0) { //遥控已经没有按下了
			RmtSta&=~(1<<6);
			RmtCnt=0;
		}
	}
	return sta;
}








