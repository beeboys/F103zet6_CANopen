/****************************************************
*
*����    ����ƽ
*����    : zhpg_0803@163.com
*��������: 2016��12��1��
*���뻷��: MDK5.14
*�ļ���  ��mycan.h
*����    ���ṩcan�������������
* 
*****************************************************/

#ifndef __MYCAN_H
#define __MYCAN_H	 
#include "sys.h"	   
#include "ucos_ii.h"
             
#define  CAN1_NODE_ID   0x08       //CAN �ڵ�ID

typedef struct {
  u16 cob_id;	/**< message's ID */
  u8 rtr;		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
  u8 len;		/**< message's length (0 to 8) */
  u8 data[8];   /**< message's datas */
} can_message_t;   //CAN��Ϣ�����ṹ                         


#define CAN1_MSG_GRP_NUM    30  //can������Ϣ���д�С

typedef struct{
    OS_EVENT *q_Can_Msg;                //����CAN ��Ϣ����ָ��
    void  *MsgGrp[CAN1_MSG_GRP_NUM];    //����CAN ָ�����飬������Ÿ�����Ϣ�������׵�ַ��������Ϣ���е�ʱ��ʹ��
    can_message_t   storage[CAN1_MSG_GRP_NUM];  //��Ϣ���д����Ϣ���ڴ���
    
    u8  mbox_windex;         //����д��������������
    can_message_t *prmsg;    //���յ���Ϣָ��
}can1_recv_unit_t;

extern can1_recv_unit_t can1_recv_unit;     //CAN1������Ϣ���й���Ԫ 

extern u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��

extern u8 Can_Send_Msg(can_message_t *msg);         //CAN ������Ϣ,���ͳɹ�����0������ʧ�ܷ���1

extern void Can_Receive_Msg(can_message_t *msg);    //CAN ������Ϣ����������ʽ�ȴ���������

#endif








