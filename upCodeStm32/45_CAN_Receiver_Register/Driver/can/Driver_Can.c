#include "Driver_Can.h"


void workMode_Id(void){
   /* 4.7.2 过滤器组0工作模式： 掩码模式 0:掩码模式 1：标识符模式 */
    CAN1->FM1R = CAN_FM1R_FBM0;
    /* 4.7.2 过滤器组0为单个32位配置 0:2给16位 1:单个32位*/
    CAN1->FS1R |= CAN_FS1R_FSC0;
    /* 4.7.3 给过滤器组0分配FIFO 0:FIFO0 1:FIFO1. 通过后的报文会放入这个FIFO中*/
    CAN1->FFA1R &= ~CAN_FFA1R_FFA0;
    /* 4.7.4 设置过滤器组0 标识符寄存器FR1 */
    //CAN1->sFilterRegister[0].FR1 = 0x00000000; /* id每位都是0 */
    /* 4.7.5 设置过滤器组0 屏蔽位寄存器FR2 */
    //CAN1->sFilterRegister[0].FR2 = 0x00000000; /* 屏蔽位是0，表示不关心ID对应的位。都是0，表示接收所有消息 */
    uint32_t id1=0x066<<21; // 标准ID存储位段[31:21]
    CAN1->sFilterRegister[0].FR1 = id1;    // FR1=标识符（ID=0x001）
    uint32_t id2=0x067<<21; // 标准ID存储位段[31:21]
    CAN1->sFilterRegister[0].FR2 = id2;  // FR2=标识符（全1）
}

void workMode_Mask(void){
    /* 4.7.2 过滤器组0工作模式： 掩码模式 0:掩码模式 1：标识符模式 */
    CAN1->FM1R &= ~CAN_FM1R_FBM0;
    /* 4.7.2 过滤器组0为单个32位配置 0:2给16位 1:单个32位*/
    CAN1->FS1R |= CAN_FS1R_FSC0;
    /* 4.7.3 给过滤器组0分配FIFO 0:FIFO0 1:FIFO1. 通过后的报文会放入这个FIFO中*/
    CAN1->FFA1R &= ~CAN_FFA1R_FFA0;

    /* 4.7.4 设置过滤器组0 标识符寄存器FR1 */
    //CAN1->sFilterRegister[0].FR1 = 0x00000000; /* id每位都是0 */
    /* 4.7.5 设置过滤器组0 屏蔽位寄存器FR2 */
    //CAN1->sFilterRegister[0].FR2 = 0x00000000; /* 屏蔽位是0，表示不关心ID对应的位。都是0，表示接收所有消息 */

    // 2.2 配置标识符（ID=0x001）和掩码（全1=0x7FF，精确匹配）
    // 32位宽格式：高16位=标准ID（11位）+ IDE/RTR位，低16位=扩展ID（0）+ 掩码扩展位
    // 标准ID存储格式：[31:21] = ID，[20] = IDE（0=标准帧），[19:16] = RTR（0=数据帧）
    uint32_t filter_id = 0x066 << 21;  // 标准ID存储位段[31:21]
    uint32_t mask = 0x7FF << 21;       // 掩码全1，精确匹配
    CAN1->sFilterRegister[0].FR1 = filter_id;    // FR1=标识符（ID=0x001）
    CAN1->sFilterRegister[0].FR2 = mask;  // FR2=掩码（全1）
}

void Driver_Can_FilterConfig(void){
   /* 4.7 配置过滤器： 接收所有消息 */
    /* 4.7.1 进入过滤器初始化模式 */
    CAN1->FMR |= CAN_FMR_FINIT;
    
    workMode_Id();
    //workMode_Mask();

    /* 4.7.6 激活过滤器组0 */
    CAN1->FA1R |= CAN_FA1R_FACT0;
    /* 4.7.7 退出过滤器初始化模式 */
    CAN1->FMR &= ~CAN_FMR_FINIT;
}

void Driver_Can_Init(void)
{
    /**1.开启时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    RCC->APB2ENR |= (RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN);

    /**2.给引脚CanTx和CanRx配置工作模式 给can的Rx和Tx重映射到PB8和PB9  10 */
    AFIO->MAPR |= AFIO_MAPR_CAN_REMAP_1;
    AFIO->MAPR &= ~AFIO_MAPR_CAN_REMAP_0;

    /**3. 初始化GPIO:PB8(Rx):浮空输入 CNF=01 MODE=00  PB9(Tx):复用推挽 CNF=10 MODE=11 */
    GPIOB->CRH &= ~(GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8 | GPIO_CRH_CNF9_0);
    GPIOB->CRH |= (GPIO_CRH_CNF8_0 | GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1);

    /**4.初始化Can */
    /* 4.1 进入初始化模式 */
    CAN1->MCR |= CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) == 0); /* 等待进入初始化模式 */

    /* 4.2 退出睡眠模式 */
    CAN1->MCR &= ~CAN_MCR_SLEEP;
    while ((CAN1->MSR & CAN_MSR_SLAK) != 0); /* 等待退出睡眠模式 */

    /* 4.3 自动离线管理。 允许自动退出离线状态 */
    CAN1->MCR |= CAN_MCR_ABOM;

    /* 4.4 自动唤醒管理。 检测到有报文，可以从睡眠模式由硬件自动唤醒。 */
    CAN1->MCR |= CAN_MCR_AWUM;

    /** 禁止自动重传(当发送失败的时候,协议标准会自动重传)*/
    CAN1->MCR |= CAN_MCR_NART;

    /* 4.5 配置位时序寄存器 */
    /**对外不发不收，对内自发自收 */
    /* 4.5.1 静默模式 用于调试 生产环境要注释 */
    //CAN1->BTR |= CAN_BTR_SILM;
    /* 4.5.2 回环模式 用于调试 生产环境要注释*/
    //CAN1->BTR |= CAN_BTR_LBKM;
    /* 4.5.3 波特率分频器，定义Tq的长度。配置35表示36分频，则产生波特率的时钟位1MHz。Tq = 1us*/
    CAN1->BTR &= ~CAN_BTR_BRP; /* 相应的位均置0 (9:0) */
    CAN1->BTR |= 35 << 0;
    /* 4.5.4 时间段1(3*Tq)和时间段2(6*Tq) */
    CAN1->BTR &= ~CAN_BTR_TS1;
    CAN1->BTR &= ~CAN_BTR_TS2;
    CAN1->BTR |= ((3 - 1) << 16);
    CAN1->BTR |= ((6 - 1) << 20);
    /* 4.5.5 再同步跳跃宽度 2*Tq*/
    CAN1->BTR &= ~CAN_BTR_SJW;
    CAN1->BTR |= (2 << 24);

    /* 4.6 退出初始化模式 */
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) != 0); /* 等待退出初始化模式 */

    Driver_Can_FilterConfig();
}


    /**
    ?* @description: 发送消息
    ?* @param {uint16_t} stdId 标准帧id
    ?* @param {uint8_t} *data 要发送的数据
    ?* @param {uint8_t} length 发送的数据的字节数
    ?*/
void CAN_SendMsg(uint16_t stdId, uint8_t *data, uint8_t length)
{
    if (length > 8){
        printf("数据长度不能超过8个字节\r\n");
        return;
    }
    /* 1. 等待邮箱0为空 (也可以判断其他邮箱) 0：非空 1：空*/
    while ((CAN1->TSR & CAN_TSR_TME0) == 0);

    /* 2. 使用标准标识符 0:标准标识符 1：扩展标识符 */
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_IDE;

    /* 3. 0：数据帧 or 1：远程帧 */
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_RTR;

    /* 4. 设置标准标识符 */
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_STID;
    CAN1->sTxMailBox[0].TIR |= (stdId << 21);

    /* 5. 设置数据长度 */
    CAN1->sTxMailBox[0].TDTR &= ~CAN_TDT0R_DLC;
    CAN1->sTxMailBox[0].TDTR |= (length << 0);

    /* 6. 设置数据 */
    uint8_t i;
    CAN1->sTxMailBox[0].TDLR = 0; /* 低位寄存器 */
    CAN1->sTxMailBox[0].TDHR = 0; /* 高位寄存器 */
    for (i = 0; i < length; i++){
        if (i < 4){
            CAN1->sTxMailBox[0].TDLR |= (data[i] << (8 * i));
        }else{
            CAN1->sTxMailBox[0].TDHR |= (data[i] << (8 * (i - 4)));
        }
    }

    /* 7. 请求发送数据 */
    CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
}

/**
?* @description:最多有三个报文，一次性读完
?* @param {uint16_t} *stdId 读取数据的标准id
?* @param {uint8_t} *data 读取到的数据
?* @param {uint8_t} *length 读取到的数据的长度
?*/
void CAN_ReceiveMsg(RxDataStruct rxDataStruct[], uint8_t *msgCount)
{
    /* 1. 获取 FIFO0 中的报文数 */
    *msgCount = (CAN1->RF0R & CAN_RF0R_FMP0) >> 0;

    uint8_t i, j;
    for (i = 0; i < *msgCount; i++){
        RxDataStruct *msg = &rxDataStruct[i];
        /* 2. 读取标准标识符id */
        //msg->stdId = (CAN1->sFIFOMailBox[0].RIR >> 21) & 0x7FF;
        msg->stdId = ((CAN1->sFIFOMailBox[0].RIR&CAN_RI0R_STID) >> 21);
        
        /* 3. 读取数据长度 */
        //msg->length = (CAN1->sFIFOMailBox[0].RDTR >> 0) & 0x0F;
        msg->length = (CAN1->sFIFOMailBox[0].RDTR&CAN_RDT1R_DLC) & 0x0F;
        
        /* 4. 读取数据 */
        memset(msg->data, 0, sizeof((char *)msg->data));
        uint32_t low = CAN1->sFIFOMailBox[0].RDLR;
        uint32_t high = CAN1->sFIFOMailBox[0].RDHR;
        for (j = 0; j < msg->length; j++){
            if (j < 4){
                msg->data[j] = (low >> (8 * j)) & 0xFF;
            }else{
                msg->data[j] = (high >> (8 * (j - 4))) & 0xFF;
            }
        }
        /* 5. 释放 FIFO 0. 则报文数减1*/
        CAN1->RF0R |= CAN_RF0R_RFOM0;
    }
}
