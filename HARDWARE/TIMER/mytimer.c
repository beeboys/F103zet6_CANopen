/****************************************************
*
*����    ����ƽ
*����    : zhpg_0803@163.com
*��������: 2016��7��19��10:11:32
*���뻷��: MDK5.14
*�ļ���  ��timer.c
*����    ���ṩ��ʱ��صĹ��ܺ���
* 
*****************************************************/

#include "mytimer.h" 	 
                                    
/******************************��ʱ��3��ʼ*********************************/

//�����Լ�ʱ�� 2^32 *��С��λ
//��С��λΪ1usʱ������ʱ = 4294967296us=  4294967 ms = 4294 s = 71 min        //��λ̫С����ʱ��׼
//��С��λΪ10usʱ������ʱ = 42949672960us=  42949670 ms = 42940 s = 710 min =11.8h       //��λ̫С����ʱ��׼
//��С��λΪ100us=0.1msʱ������ʱ = 429496729600us=  429496700 ms = 429400 s = 7100 min  =118h   
 
static unsigned long cg_timer3_cnt = 0;    

//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		  		
    OSIntEnter(); 
    
	if(TIM3->SR&0X0001)//����ж�
	{
 		cg_timer3_cnt = cg_timer3_cnt + 1;    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	   

    OSIntExit();     
}
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{ 
    NVIC_InitTypeDef  NVIC_InitStructure;
    
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms    
	TIM3->PSC=psc;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
	TIM3->DIER|=1<<0;   //��������ж�	  
	TIM3->CR1&=~0x01;    //ʧ�ܶ�ʱ��3 
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //��ʱ��3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;     //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		    //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			    //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                            //����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
    cg_timer3_cnt = 0;								 
}

//������ʱ��3
void TIM3_Start(void)
{
    cg_timer3_cnt = 0;
    TIM3->CR1|=0x01;        //ʹ�ܶ�ʱ��3
}

//�رն�ʱ��3
void TIM3_Stop(void)
{
    TIM3->CR1 &=~0x01;      //ʧ�ܶ�ʱ��3
}

//��ȡ��ʱ��3����ʱ�䣬��λ0.01ms(�ø��ݶ�ʱ��3������)
unsigned long GetRunTime(void)
{   
    unsigned long temp = cg_timer3_cnt;

    cg_timer3_cnt = 0;
    return temp;
}

/******************************��ʱ��3����*********************************/

///******************************��ʱ��4��ʼ*********************************/
//
////�����Լ�ʱ�� 2^32 *��С��λ
////��С��λΪ1usʱ������ʱ = 4294967296us=  4294967 ms = 4294 s = 71 min        //��λ̫С����ʱ��׼
////��С��λΪ10usʱ������ʱ = 42949672960us=  42949670 ms = 42940 s = 710 min =11.8h       //��λ̫С����ʱ��׼
////��С��λΪ100us=0.1msʱ������ʱ = 429496729600us=  429496700 ms = 429400 s = 7100 min  =118h   
// 
//static unsigned long cg_timer4_cnt = 0;    
//
////��ʱ��4�жϷ������	 
//void TIM4_IRQHandler(void)
//{ 		    		  		
//    OSIntEnter(); 
//    
//	if(TIM4->SR&0X0001)//����ж�
//	{
// 		cg_timer4_cnt = cg_timer4_cnt + 1;    				   				     	    	
//	}				   
//	TIM4->SR&=~(1<<0);//����жϱ�־λ 	   
//
//    OSIntExit();     
//}
////ͨ�ö�ʱ��3�жϳ�ʼ��
////����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
////arr���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
////����ʹ�õ��Ƕ�ʱ��4!
//void TIM4_Int_Init(u16 arr,u16 psc)
//{
//    NVIC_InitTypeDef  NVIC_InitStructure;
//    
//	RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ��    
// 	TIM4->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms    
//	TIM4->PSC=psc;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
//	TIM4->DIER|=1<<0;   //��������ж�	  
//	TIM4->CR1&=~0x01;    //ʧ�ܶ�ʱ��4
//    
//    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             //��ʱ��4�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;     //��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		    //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			    //IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	                            //����ָ���Ĳ�����ʼ��VIC�Ĵ���
//    
//    cg_timer4_cnt = 0;								 
//}
//
////������ʱ��4
//void TIM4_Start(void)
//{
//    cg_timer4_cnt = 0;
//    TIM4->CR1|=0x01;        //ʹ�ܶ�ʱ��4
//}
//
////�رն�ʱ��4
//void TIM4_Stop(void)
//{
//    TIM4->CR1 &=~0x01;      //ʧ�ܶ�ʱ��4
//}
//
////��ȡ��ʱ��4����ʱ�䣬��λ1ms(�ø��ݶ�ʱ��4������)
//unsigned long GetRunMsCnt(void)
//{   
//    unsigned long temp = cg_timer4_cnt;
// 
//    return temp;
//}
//
///******************************��ʱ��4����*********************************/



