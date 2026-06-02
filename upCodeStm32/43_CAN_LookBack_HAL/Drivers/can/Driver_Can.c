#include "Driver_Can.h"

void Driver_Can_FilterConfig(void){
   CAN_FilterTypeDef sFilterConfig;
   sFilterConfig.FilterBank=0;/*使用过滤器组0*/
   sFilterConfig.FilterFIFOAssignment=CAN_RX_FIFO0;/*过滤器组0绑定fifo0*/
   sFilterConfig.FilterMode=CAN_FILTERMODE_IDMASK;/*掩码模式 */
   sFilterConfig.FilterScale=CAN_FILTERSCALE_32BIT;
   sFilterConfig.FilterIdHigh=0x0;
   sFilterConfig.FilterIdLow=0x0;
   sFilterConfig.FilterMaskIdHigh=0x0;
   sFilterConfig.FilterMaskIdLow=0x0;
   sFilterConfig.FilterActivation=CAN_FILTER_ENABLE;

   HAL_CAN_ConfigFilter(&hcan,&sFilterConfig);
}

void Driver_Can_Init(void)
{
    MX_CAN_Init();
    Driver_Can_FilterConfig();
    //启动CAN
    HAL_CAN_Start(&hcan);
}


    /**
    ?* @description: 发送消息
    ?* @param {uint16_t} stdId 标准帧id
    ?* @param {uint8_t} *data 要发送的数据
    ?* @param {uint8_t} length 发送的数据的字节数
    ?*/
void CAN_SendMsg(uint16_t stdId, uint8_t *data, uint8_t length)
{
    /**先判断是否有空的发送邮箱 等待有发送的邮箱 */
    while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan)==0);//返回空闲的发送邮箱的个数
    CAN_TxHeaderTypeDef pHeader;
    pHeader.StdId=stdId;
    pHeader.IDE=CAN_ID_STD;
    pHeader.RTR=CAN_RTR_DATA;
    pHeader.DLC=length;
    uint32_t pTxMailbox;
    HAL_CAN_AddTxMessage(&hcan,&pHeader,data,&pTxMailbox);
}

/**
?* @description:最多有三个报文，一次性读完
?* @param {uint16_t} *stdId 读取数据的标准id
?* @param {uint8_t} *data 读取到的数据
?* @param {uint8_t} *length 读取到的数据的长度
?*/
void CAN_ReceiveMsg(RxDataStruct rxDataStruct[], uint8_t *msgCount)
{
    /**获取目前fifo中收到的报文数 */
    *msgCount=HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_RX_FIFO0);

    /**遍历出所有报文 */
    CAN_RxHeaderTypeDef pHeader;
    for(uint8_t i=0;i<*msgCount;i++){
        /**取出fifo中的报文的具体数据 */
        HAL_CAN_GetRxMessage(&hcan,CAN_RX_FIFO0,&pHeader,rxDataStruct[i].data);
        rxDataStruct[i].stdId=pHeader.StdId;
        rxDataStruct[i].length=pHeader.DLC;
    }

}
