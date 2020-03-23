/****************************************************
*
*����    ����ƽ
*����    : zhpg_0803@163.com
*��������: 2016��12��2��
*���뻷��: MDK5.14
*�ļ���  ��stm32_canfestival.c
*����    ��CANopen Э�� ��CANfestival��ֲ��Ҫʵ�ֵĺ���
* 
*****************************************************/

#include "stm32f1_canfestival.h"
#include "sys.h"
#include "mycan.h"

/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time �洢���һ����ʱ��ֵ�Լ��㾭����ʱ��
static TIMEVAL last_time_set = TIMEVAL_MAX;

unsigned int TimeCNT=0;             //ʱ�����
unsigned int NextTime=0;            //��һ�δ���ʱ�����
unsigned int TIMER_MAX_COUNT=70000; //����ʱ�����

//ͨ�ö�ʱ��4�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��4!
static void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM5��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx					 
}

void initTimer(void)
{
    TIM4_Int_Init(1000-1, 72-1);     //1Mhz�ļ���Ƶ�ʣ�������1000Ϊ1ms
}

//���⻹Ҫ��һ��1����Ķ�ʱ����ÿ��1ms����һ���������
void TimerForCan(void)
{
    TimeCNT++;
    
    if (TimeCNT >= TIMER_MAX_COUNT)
    {
        TimeCNT=0;
    }
    if (TimeCNT == NextTime)
    {
        TimeDispatch();     //��ʱʱ�䵽��ִ��ʱ����صķַ�����
    }
}

//��ʱ��4ÿ��1ms�ж�1��
void TIM4_IRQHandler(void)
{	
    OSIntEnter(); 
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
        TimerForCan();                                 //���ʱ���������
	}
    OSIntExit();  		    
}
 
UNS8 canSend(CAN_PORT notused, Message *m)
{ 
    return Can_Send_Msg((can_message_t *) m);   //����CAN��Ϣ
}

UNS8 canChangeBaudRate(CAN_PORT port, char* baud)
{
    return 0;
}

void setTimer(TIMEVAL value)
{ 
    NextTime = (TimeCNT+value)%TIMER_MAX_COUNT;
}

TIMEVAL getElapsedTime(void)
{
    int ret=0;
    
    ret = TimeCNT> last_time_set ? TimeCNT - last_time_set : TimeCNT + TIMER_MAX_COUNT - last_time_set;
    last_time_set = TimeCNT;
    return ret;
}


 


