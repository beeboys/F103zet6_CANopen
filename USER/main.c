#include "sys.h" 	
#include "delay.h"	
#include "usart.h"
#include "led.h"
#include "mycan.h"
#include "EwtCanAnalysis_objdict.h"
#include "stm32f1_canfestival.h"

#include "includes.h"
#include "mytimer.h"

 

/************************************************
 ALIENTEKս��STM32������ʵ��51
 UCOSIIʵ��1-������� ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	
 			   
//LED0����
//�����������ȼ�
#define LED0_TASK_PRIO       			7 
//���������ջ��С
#define LED0_STK_SIZE  		    		64
//�����ջ	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//������
void led0_task(void *pdata);


//LED1����
//�����������ȼ�
#define LED1_TASK_PRIO       			6 
//���������ջ��С
#define LED1_STK_SIZE  					64
//�����ջ
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void led1_task(void *pdata);

 int main(void)
 {	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
    uart_init(115200);
    TIM3_Int_Init(100-1, 72-1);   //1Mhz�ļ���Ƶ�ʣ�����10��Ϊ100us, ��ʱ��3ÿ��100us�ж�һ�� 
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	  	 
}
	  
//��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
    
    CAN_Mode_Init(CAN_SJW_1tq, CAN_BS2_8tq, CAN_BS1_9tq,4, CAN_Mode_Normal);//CAN��ʼ������ģʽ,������500Kbps   
        
    initTimer();
    setNodeId(&EwtCanAnalysis_Data, CAN1_NODE_ID);  //����CAN �ڵ�ID
    setState(&EwtCanAnalysis_Data, Initialisation);	// ��ʼ��CAN����״̬
    setState(&EwtCanAnalysis_Data, Operational);	// debug, ��ʽ�����Ҫȥ��
    
 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}

//LED0����
void led0_task(void *pdata)
{	
//    u8 i=0;
//    can_message_t  msg;
//    msg.cob_id = 0x01;
//    msg.len = 8;
//    msg.rtr = 0;
//    
//    for(i=0; i<8; i++)
//    {
//        msg.data[i] = 0x0 + i;
//    }
//    
    Message can_msg;
	//Message can_msg = {
	//					0x181,	/**< message's ID */
	//					0,		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
	//					8,		/**< message's length (0 to 8) */
	//					{1,2,3,4,5,6,7,8}, /**< message's datas */
	//					};
     
	while(1)
	{
//        Can_Send_Msg(&msg);
//        msg.cob_id++;

        Can_Receive_Msg((can_message_t *)&can_msg);     //������Ϣ
        canDispatch(&EwtCanAnalysis_Data, &can_msg);    //������Ϣ
		//OSTimeDlyHMSM(0, 0, 0, 200);
	};
}

void CheckReadSDO(CO_Data* d, UNS8 nodeid)
{
	UNS32 abortCode;
	UNS32 data=0;
	UNS32 size=64;

	if(getReadResultNetworkDict(&EwtCanAnalysis_Data, nodeid, &data, &size, &abortCode) != SDO_FINISHED)
		printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
	else
		printf("\nResult : %x\n", data);

	/* Finalize last SDO transfer with this node */
	closeSDOtransfer(&EwtCanAnalysis_Data, nodeid, SDO_CLIENT);
}

extern unsigned long g_time5_cnt;
//LED1����
void led1_task(void *pdata)
{	  
    unsigned long tempcnt = 0;
    
	while(1)
	{
		LED1=0;
        
        TIM3_Start();     
        OSTimeDlyHMSM(0, 0, 0, 50);        
        TIM3_Stop(); 
        tempcnt = GetRunTime();
        DEBUG("����ʱ��1Ϊ:%d\r\n", (int)(tempcnt/10));        
		
        LED1=1;
        
        TIM3_Start();      
        OSTimeDlyHMSM(0, 0, 0, 950);        
        TIM3_Stop(); 
        tempcnt = GetRunTime();
        DEBUG("����ʱ��2Ϊ:%d\r\n", (int)(tempcnt/10));      
        
        //readNetworkDictCallback (&EwtCanAnalysis_Data, 5, 0x2001, 0x01, 0, CheckReadSDO, 0);
	};
}






