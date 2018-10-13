#include "led.h"

#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "includes.h"

//STM32F103核心板例程
//库函数版本例程
/************** 嵌入式开发网  **************/
/********** mcudev.taobao.com       ********/

//UCOSII实验2-2-任务创建，删除，挂起，恢复 
 
 
/////////////////////////UCOSII任务堆栈设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//创建任务堆栈空间	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数接口
void start_task(void *pdata);	
 			   
//LED任务
//设置任务优先级
#define LED_TASK_PRIO       			7 
//设置任务堆栈大小
#define LED_STK_SIZE  		    		64
//创建任务堆栈空间	
OS_STK LED_TASK_STK[LED_STK_SIZE];
//任务函数接口
void led_task(void *pdata);


//蜂鸣器任务
//设置任务优先级
#define BEEP_TASK_PRIO       			5 
//设置任务堆栈大小
#define BEEP_STK_SIZE  					64
//创建任务堆栈空间	
OS_STK BEEP_TASK_STK[BEEP_STK_SIZE];
//任务函数接口
void beep_task(void *pdata);


//按键扫描任务
//设置任务优先级
#define KEY_TASK_PRIO       			3 
//设置任务堆栈大小
#define KEY_STK_SIZE  					64
//创建任务堆栈空间	
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务函数接口
void key_task(void *pdata);
			
 int main(void)
 {	 
  
 
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Debug_USART_Config();
	LED_Init();		  		//初始化与LED连接的硬件接口
 
	KEY_Init();				//按键初始化

	UsartPrintf(USART3, "hardware init finish!!!!!!!!!!!\r\n");             //串口输出
	UsartPrintf(USART3, "mcudev.taobao.com\r\n");             //串口输出 	 
	 
	 
	 
	 
	OSInit();  	 			//初始化UCOSII		 			  
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	    
}

//开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 		  		 			  
	OSStatInit();					//初始化统计任务.这里会延时1秒钟左右	
 	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
 	OSTaskCreate(led_task,(void *)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);						    				   
 	//OSTaskCreate(beep_task,(void *)0,(OS_STK*)&BEEP_TASK_STK[BEEP_STK_SIZE-1],BEEP_TASK_PRIO);	 				   				   
 	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	 				   
 	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}	  
//LED任务
void led_task(void *pdata)
{   
	while(1)
	{  
	      LED=0;//LED灯亮，程序运行
	   		delay_ms(500);
	}									 
}	   

//蜂鸣器任务
void beep_task(void *pdata)
{
	while(1)
	{  	
	   if(OSTaskDelReq(OS_PRIO_SELF)==OS_ERR_TASK_DEL_REQ) //判断是否有删除请求
		 {
		 OSTaskDel(OS_PRIO_SELF);						   //删除任务本身TaskLed
		 }
		 
	   delay_ms(60);
    	 
		 delay_ms(940);
	}									 
}

//按键扫描任务
void key_task(void *pdata)
{	
	u8 key;		    						 
	while(1)
	{
		key=KEY_Scan(0);
		
		if(key==0)
		{
			  LED=1;//LED灯亮，程序挂起
			
		    OSTaskSuspend(LED_TASK_PRIO);//挂起LED任务，LED停止闪烁
		
				UsartPrintf(USART3, "LED Task 挂起\r\n");             //串口输出
				UsartPrintf(USART3, "嵌入式开发网\r\n");             //串口输出
				UsartPrintf(USART3, "mcudev.taobao.com\r\n");             //串口输出
				UsartPrintf(USART3, "\r\n");             //串口输出
			
		}
		else 
		{
		    OSTaskResume(LED_TASK_PRIO);	//恢复LED任务，LED恢复闪烁
			
				UsartPrintf(USART3, "LED Task 运行\r\n");             //串口输出
				UsartPrintf(USART3, "嵌入式开发网\r\n");             //串口输出
				UsartPrintf(USART3, "mcudev.taobao.com\r\n");             //串口输出
				UsartPrintf(USART3, "\r\n");             //串口输出
			
			
		}
//		else if (key==KEY_UP)
//		{
//		  OSTaskDelReq(BEEP_TASK_PRIO);	//发送删除BEEP任务请求，任务睡眠，无法恢复
//		}
//		else if(key==KEY_DOWN)
//		{
//		 OSTaskCreate(beep_task,(void *)0,(OS_STK*)&BEEP_TASK_STK[BEEP_STK_SIZE-1],BEEP_TASK_PRIO);//重新创建任务beep	 				   				   
//		}   
 		delay_ms(5000);
	}
}


