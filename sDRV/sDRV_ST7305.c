#include "sDRV_ST7305.h"






/*Command Table1*/

/*No operation*/
#define COMM_NOP                                              (0x00u)

/*Software reset*/
//复位后最少等待5ms,如果如果在sleep模式下复位,则要等待120ms
#define COMM_SWRESET                                          (0x01u)

/*Read display ID */
//读回第一个参数:manufacturer ID
//第二个:version ID
//第三个:driver ID
#define COMM_RDDID                                            (0x04u)

/*Read display status*/
#define COMM_RDDST                                            (0x09u)

/*Read display Power Mode */
#define COMM_RDDPM                                            (0x0Au)

/*Sleep in */
#define COMM_SLPIN                                            (0x10u)

/*Sleep out*/
#define COMM_SLPOUT                                           (0x11u)

/*Partial on (240 duty set) */
#define COMM_PLTON                                            (0x12u)

/*Partial off (320 duty set) */
#define COMM_PLTOFF                                           (0x13u)

/*Display inversion of*/
#define COMM_INVOFF                                           (0x20u)

/*Display inversion on*/
#define COMM_INVON                                            (0x21u)

/*Display off */
#define COMM_DISPOFF                                          (0x28u)

/*Display on*/
#define COMM_DISPON                                           (0x29u)

/*Column address set */
#define COMM_CASET                                            (0x2Au)
#define MSK_CASET_XS                                          (0b00111111u)
#define MSK_CASET_XE                                          (0b00111111u)

/*Row address set  */
#define COMM_RASET                                            (0x2Bu)
#define MSK_RASET_YS                                          (0b11111111u)
#define MSK_RASET_YE                                          (0b11111111u)

/*Memory write */
#define COMM_RAMWR                                            (0x2Cu)

/*Memory read */
#define COMM_RAMRD                                            (0x2Eu)

/*Tearing effect line off */
#define COMM_TEOFF                                            (0x34u)

/*Tearing effect line on*/
#define COMM_TEON                                             (0x35u)
#define PARM1_TEON(_TEM)                                      (_TEM)

/*Memory data access control */
#define COMM_MADCTL                                           (0x36u)
//Page Address Order,Column Address Order,Page/Column Order,Data Output Order,Gate Scan Order 
#define PARM1_MADCTL(_MY,_MX,_MV,_DO,_GS)                     ((_MY << 7) | (_MX << 6) | (_MV << 5) | (_DO << 3) | (_GS) << 2)

/*Vertical scrolling start address */
#define COMM_VSCSAD                                           (0x37u)
#define PARM1_VSCSAD(_VSP8)                                   (_VSP8)
#define PARM2_VSCSAD(_VSP7TO0)                                (_VSP7TO0)

/*High power mode */
#define COMM_HPM                                              (0x38u)

/*Low power mode*/
#define COMM_LPM                                              (0x39u)

/*Data format select */
#define COMM_DTFORM                                           (0x3Au)
//Data Up Down Switch,Bytes Per Pixel Select
#define PARM1_DTFORM(_XDE,_BPS)                               ((_XDE << 4) | _BPS)

/*Write memory continue */
#define COMM_RAMWRC                                           (0x3Cu)

/*Memory read */
#define COMM_RAMRDC                                           (0x3Eu)

/*Set tear scanline */
#define COMM_TESCAN                                           (0x44u)
#define PARM1_TESCAN(_N8)                                     (_N8)
#define PARM2_TESCAN(_N7TO0)                                  (_N7TO0)

/*Read ID1 */
#define COMM_RDID1                                            (0xDAu)

/*Read ID2 */
#define COMM_RDID2                                            (0xDBu)

/*Read ID3 */
#define COMM_RDID3                                            (0xDCu)


/*Command Table2*/

/*Gate Line Setting*/
#define COMM_GATESET                                          (0xB0)
#define PARM1_GATESET(_GL)                                    (_GL)

/*First Gate Setting*/
#define COMM_FSTCOM                                           (0xB1u)
#define PARM1_FSTCOM(_FST8)                                   (_FST8)
#define PARM2_FSTCOM(_FST7TO0)                                (_FST7TO0)

/*Frame Rate Control */
#define COMM_FRCTRL                                           (0xB2u)
#define PARM1_FRCTRL(_HFRA,_LFRA)                             ((_HFRA << 4) | _LFRA)

/* Update Period Gate EQ Control in HPM */
#define COMM_GTUPEQH                                          (0xB3u)

/* Update Period Gate EQ Control in LPM*/
#define COMM_GTUPEQL                                          (0xB4u)

/*Source EQ Enable*/
#define COMM_SOUEQ                                            (0xB7u)
#define PARM1_SOUEQ(_SOUEQ)                                   (_SOUEQ | 0b00000011u)

/*Panel Setting */
#define COMM_PNLSET                                           (0xB8u)
#define PARM1_PNLSET(_DOTINV,_DPSCN,_LAY)                     ((_DOTINV << 5) | (_DPSCN << 2) | _LAY)

/*Gamma Mode Setting*/
#define COMM_GAMAMS                                           (0xB9u)
#define PARM1_GAMAMS(_GAMA)                                   (_GAMA << 5)

/*Enable Clear RAM*/
#define COMM_CLRAM                                            (0xBBu)
#define PARM1_CLRAM(_CLR)                                     ((_CLR << 7) | 0b01001111u)

/*Gate Voltage Control*/
#define COMM_GCTRL                                            (0xC0u)
#define PARM1_GCTRL(_VGHA)                                    (_VGHA)
#define PARM2_GCTRL(_VGLA)                                    (_VGLA)

/* Source High Positive Voltage Control */
#define COMM_VSHPCTRL                                         (0xC1u)
#define PARM1_VSHPCTRL(_VSHPA1)                               (_VSHPA1)
#define PARM2_VSHPCTRL(_VSHPA2)                               (_VSHPA2)
#define PARM3_VSHPCTRL(_VSHPA3)                               (_VSHPA3)
#define PARM4_VSHPCTRL(_VSHPA4)                               (_VSHPA4)

/* Source Low Positive Voltage Control */
#define COMM_VSLPCTRL                                         (0xC2u)
#define PARM1_VSLPCTRL(_VSLPA1)                               (_VSLPA1)
#define PARM2_VSLPCTRL(_VSLPA2)                               (_VSLPA2)
#define PARM3_VSLPCTRL(_VSLPA3)                               (_VSLPA3)
#define PARM4_VSLPCTRL(_VSLPA4)                               (_VSLPA4)

/* Source High Negative Voltage Control */
#define COMM_VSHNCTRL                                         (0xC4u)
#define PARM1_VSHNCTRL(_VSHNA1)                               (_VSHNA1)
#define PARM2_VSHNCTRL(_VSHNA2)                               (_VSHNA2)
#define PARM3_VSHNCTRL(_VSHNA3)                               (_VSHNA3)
#define PARM4_VSHNCTRL(_VSHNA4)                               (_VSHNA4)

/* Source Low Negative Voltage Control */
#define COMM_VSLNCTRL                                         (0xC5u)
#define PARM1_VSLNCTRL(_VSLNA1)                               (_VSLNA1)
#define PARM2_VSLNCTRL(_VSLNA2)                               (_VSLNA2)
#define PARM3_VSLNCTRL(_VSLNA3)                               (_VSLNA3)
#define PARM4_VSLNCTRL(_VSLNA4)                               (_VSLNA4)

/*Source Gamma Voltage Control*/
#define COMM_VSIKCTRL                                         (0xC8u)
#define PARM1_VSIKCTRL(_VSIP)                                 (_VSIP)
#define PARM2_VSIKCTRL(_VSKP)                                 (_VSKP)
#define PARM3_VSIKCTRL(_VSIN)                                 (_VSIN)
#define PARM4_VSIKCTRL(_VSKN)                                 (_VSKN)

/*Source Voltage Select */
#define COMM_VSHLSEL                                          (0xC9u)
#define PARM1_VSHLSEL(_VSHLSEL)                               (_VSHLSEL)

/*ID1 Setting*/
#define COMM_ID1SET                                           (0xCAu)
#define PARM1_ID1SET(_ID1)                                    (_ID1)

/*ID2 Setting*/
#define COMM_ID2SET                                           (0xCBu)
#define PARM1_ID2SET(_ID2)                                    (_ID2)

/*ID3 Setting*/
#define COMM_ID3SET                                           (0xCCu)
#define PARM1_ID3SET(_ID3)                                    (_ID3)

/* Enable Auto Power Down */
#define COMM_AUTOPWRCTRL                                      (0xD0u)
#define PARM1_AUTOPWRCTRL(_AUTOPWR)                           ((_AUTOPWR << 7) | 0b01111111u)

/*Booster Enable*/
#define COMM_BSTEN                                            (0xD1u)
#define PARM1_BSTEN(_BSTEN)                                   (_BSTEN)

/*NVM Load Control */
#define COMM_NVMLOADCTRL                                      (0xD6u)
#define PARM1_NVMLOADCTRL(_VS_EN,_ID_EN)                      (1u << 4u | (_VS_EN << 2u) | (_ID_EN << 1u) | 1u)
#define PARM2_NVMLOADCTRL(_NRDTIME,_NRDSLP)                   ((_NRDTIME << 2) | (_NRDSLP << 1))

/*OSC Setting*/
#define COMM_OSCSET                                           (0xD8u)
#define PARM1_OSCSET(_OSCEN,_OSCSW2,_OSCSW1,_OSCSW0)          ((_OSCEN << 7) | (_OSCSW2 << 5) | (_OSCSW1 << 2) | (_OSCSW0 << 1))
#define PARM2_OSCSET                                          (0b11101001u)



/*接口*/


#define CS_CLK_EN     __GPIOA_CLK_ENABLE
#define CS_PORT       GPIOA
#define CS_PIN        GPIO_PIN_4

#define RES_CLK_EN    __GPIOC_CLK_ENABLE
#define RES_PORT      GPIOC
#define RES_PIN       GPIO_PIN_5

#define DC_CLK_EN    __GPIOC_CLK_ENABLE
#define DC_PORT      GPIOC
#define DC_PIN       GPIO_PIN_4

#define TE_CLK_EN    __GPIOB_CLK_ENABLE
#define TE_PORT      GPIOB
#define TE_PIN       GPIO_PIN_0


static void portCSInit(){
    CS_CLK_EN();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin = CS_PIN;
    HAL_GPIO_Init(CS_PORT,&gpio);
    HAL_GPIO_WritePin(CS_PORT,CS_PIN,GPIO_PIN_SET);
}

static inline void portSetCS(bool Level){
    HAL_GPIO_WritePin(CS_PORT,CS_PIN,(GPIO_PinState)Level);
}

static void portRESInit(){
    RES_CLK_EN();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin = RES_PIN;
    HAL_GPIO_Init(RES_PORT,&gpio);
    HAL_GPIO_WritePin(RES_PORT,RES_PIN,GPIO_PIN_SET);
}

static void portSetRES(bool Level){
    HAL_GPIO_WritePin(RES_PORT,RES_PIN,(GPIO_PinState)Level);
}

static void portDCInit(){
    DC_CLK_EN();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin = DC_PIN;
    HAL_GPIO_Init(DC_PORT,&gpio);
    HAL_GPIO_WritePin(DC_PORT,DC_PIN,GPIO_PIN_SET);
}

static void portSetDC(bool Level){
    HAL_GPIO_WritePin(DC_PORT,DC_PIN,(GPIO_PinState)Level);
}

static void portTEInit(){
    TE_CLK_EN();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin = TE_PIN;
    HAL_GPIO_Init(TE_PORT,&gpio);
    HAL_GPIO_WritePin(TE_PORT,TE_PIN,GPIO_PIN_SET);
}

static bool portGetTE(){
    return (GPIO_PinState)HAL_GPIO_ReadPin(TE_PORT,TE_PIN);
}

extern SPI_HandleTypeDef hspi1;

void sBSP_SPI_OLED_SendByte(uint8_t byte){
    HAL_SPI_Transmit(&hspi1,&byte,1,100);
}

uint8_t sBSP_SPI_OLED_RecvByte(){
    uint8_t send_byte = 0;
    HAL_SPI_Receive (&hspi1,&send_byte,1,100);
    return send_byte;
}

void sBSP_SPI_OLED_SendBytes(uint8_t *pData,uint16_t Size){
    HAL_SPI_Transmit(&hspi1,pData,Size,1000);
    
    // HAL_SPI_Transmit_DMA(&hspi1,pData,Size);
    // while(HAL_SPI_GetState(&OLED_SPI_HANDLE) != HAL_SPI_STATE_READY);
}

static void portWriteCmd(uint8_t command){
    portSetDC(0);
    portSetCS(0);

    sBSP_SPI_OLED_SendByte(command);

    portSetCS(1);
}

static void portWriteParam(uint8_t parameter){
    portSetDC(1);
    portSetCS(0);

    sBSP_SPI_OLED_SendByte(parameter);

    portSetCS(1);
}

static void portDelay(uint32_t ms){
    HAL_Delay(ms);
}










void sDRV_ST7305_Init(){
    portCSInit();
    portRESInit();
    portDCInit();


    portSetRES(1);
    portDelay(10);
    portSetRES(0);
    portDelay(10);
    portSetRES(1);
    portDelay(10);
    
    // 7305
    portWriteCmd(COMM_NVMLOADCTRL); //NVM Load Control 
    portWriteParam(PARM1_NVMLOADCTRL(1,0)); 
    portWriteParam(PARM2_NVMLOADCTRL(0,1));

    portWriteCmd(COMM_BSTEN); //Booster Enable 
    portWriteParam(PARM1_BSTEN(1)); 

    portWriteCmd(COMM_GCTRL); //Gate Voltage Setting 
    portWriteParam(PARM1_GCTRL(0x12)); //VGH 00:8V  04:10V  08:12V   0E:15V   12:17V
    portWriteParam(PARM2_GCTRL(0x0A)); //VGL 00:-5V   04:-7V   0A:-10V

    // VLC=3.6V (12/-5)(delta Vp=0.6V)		
    portWriteCmd(0xC1); //VSHP Setting (4.8V)	
    portWriteParam(0X3C); //VSHP1 	
    portWriteParam(0X3E); //VSHP2 	
    portWriteParam(0X3C); //VSHP3 	
    portWriteParam(0X3C); //VSHP4	

    portWriteCmd(0xC2); //VSLP Setting (0.98V)	
    portWriteParam(0X23); //VSLP1 	
    portWriteParam(0X21); //VSLP2 	
    portWriteParam(0X23); //VSLP3 	
    portWriteParam(0X23); //VSLP4 	

    portWriteCmd(0xC4); //VSHN Setting (-3.6V)	
    portWriteParam(0X5A); //VSHN1	
    portWriteParam(0X5C); //VSHN2 	
    portWriteParam(0X5A); //VSHN3 	
    portWriteParam(0X5A); //VSHN4 	

    portWriteCmd(0xC5); //VSLN Setting (0.22V)	
    portWriteParam(0X37); //VSLN1 	
    portWriteParam(0X35); //VSLN2 	
    portWriteParam(0X37); //VSLN3 	
    portWriteParam(0X37); //VSLN4

    portWriteCmd(0xD8); //OSC Setting                                                                                                                                                                              
    portWriteParam(0XA6); //Enable OSC, HPM Frame Rate Max = 32hZ
    portWriteParam(0XE9); 

    /*-- HPM=32hz ; LPM=> 0x15=8Hz 0x14=4Hz 0x13=2Hz 0x12=1Hz 0x11=0.5Hz 0x10=0.25Hz---*/
    portWriteCmd(0xB2); //Frame Rate Control 
    portWriteParam(0X12); //HPM=32hz ; LPM=1hz 

    portWriteCmd(0xB3); //Update Period Gate EQ Control in HPM 
    portWriteParam(0XE5); 
    portWriteParam(0XF6); 
    portWriteParam(0X17);
    portWriteParam(0X77); 
    portWriteParam(0X77); 
    portWriteParam(0X77); 
    portWriteParam(0X77); 
    portWriteParam(0X77); 
    portWriteParam(0X77); 
    portWriteParam(0X71); 

    portWriteCmd(0xB4); //Update Period Gate EQ Control in LPM 
    portWriteParam(0X05); //LPM EQ Control 
    portWriteParam(0X46); 
    portWriteParam(0X77); 
    portWriteParam(0X77); 
    portWriteParam(0X77); 
    portWriteParam(0X77); 
    portWriteParam(0X76); 
    portWriteParam(0X45); 

    portWriteCmd(0x62); //Gate Timing Control
    portWriteParam(0X32);
    portWriteParam(0X03);
    portWriteParam(0X1F);

    portWriteCmd(0xB7); //Source EQ Enable 
    portWriteParam(0X13); 

    portWriteCmd(0xB0); //Gate Line Setting 
    portWriteParam(0X60); //384 line 

    portWriteCmd(0x11); //Sleep out 
    portDelay(10); 

    portWriteCmd(0xC9); //Source Voltage Select  
    portWriteParam(0X00); //VSHP1; VSLP1 ; VSHN1 ; VSLN1

    portWriteCmd(0x36); //Memory Data Access Control
    // portWriteParam(0X00); //Memory Data Access Control: MX=0 ; DO=0 
    portWriteParam(0X48); //MX=1 ; DO=1 
    // portWriteParam(PARM1_MADCTL(1,0,0,0,0));
    // portWriteParam(0X4c); //MX=1 ; DO=1 GS=1

    portWriteCmd(0x3A); //Data Format Select 
    portWriteParam(0X11); //10:4write for 24bit ; 11: 3write for 24bit

    portWriteCmd(0xB9); //Gamma Mode Setting 
    portWriteParam(0X20); //20: Mono 00:4GS  

    portWriteCmd(0xB8); //Panel Setting 
    portWriteParam(0x29); // Panel Setting: 0x29: 1-Dot inversion, Frame inversion, One Line Interlace

    //WRITE RAM 168*384
    portWriteCmd(0x2A); //Column Address Setting 
    portWriteParam(0X17); 
    portWriteParam(0X24); // 0X24-0X17=14 // 14*12=168

    portWriteCmd(0x2B); //Row Address Setting 
    portWriteParam(0X00); 
    portWriteParam(0XBF); // 192*2=384
    /*
    portWriteCmd(0x72); //de-stress off 
    portWriteParam(0X13);
    */
    portWriteCmd(0x35); //TE
    portWriteParam(0X00); //

    portWriteCmd(0xD0); //Auto power dowb OFF
    // portWriteParam(0X7F); //Auto power dowb OFF
    portWriteParam(0XFF); //Auto power dowb ON


    portWriteCmd(0x39); //LPM:Low Power Mode ON
    // portWriteCmd(0x38); //HPM:high Power Mode ON


    portWriteCmd(0x29); //DISPLAY ON  
    // portWriteCmd(0x28); //DISPLAY OFF  

    // portWriteCmd(0x21); //Display Inversion On 
    portWriteCmd(0x20); //Display Inversion Off 

    portWriteCmd(0xBB); // Enable Clear RAM
    portWriteParam(0x4F); // CLR=0 ; Enable Clear RAM,clear RAM to 0

}

void sDRV_ST7305_SetInvShowMode(bool is_inv){
    is_inv ? portWriteCmd(COMM_INVON) : portWriteCmd(COMM_INVOFF);
}


void sDRV_ST7305_FillImg(uint16_t x,uint16_t y,uint16_t img_height,uint16_t img_weight,const uint8_t* img){
    portWriteCmd(COMM_CASET);//Column Address Setting S61~S182
    portWriteParam(0x17);
    portWriteParam(0x24); // 0X24-0X17=14 // 14*4*3=168

    portWriteCmd(COMM_RASET);//Row Address Setting G1~G250
    portWriteParam(0x00);
    portWriteParam(0xBF); // 192*2=384

    portWriteCmd(0x2C);   //write image data

    portSetDC(1);
    portSetCS(0);
    
    sBSP_SPI_OLED_SendBytes(img,img_height * img_weight);


    portSetCS(1);
}


void sDRV_ST7305_UpdateScreen(uint8_t* gram,uint32_t len_bytes){
    portWriteCmd(COMM_CASET);//Column Address Setting S61~S182
    portWriteParam(0x17);
    portWriteParam(0x24); // 0X24-0X17=14 // 14*4*3=168

    portWriteCmd(COMM_RASET);//Row Address Setting G1~G250
    portWriteParam(0x00);
    portWriteParam(0xBF); // 192*2=384

    portWriteCmd(0x2C);   //write image data

    portSetDC(1);
    portSetCS(0);

    // portWriteCmd(parameter);
    // for(uint32_t i = 0;i < (SDRV_ST7305_H * SDRV_ST7305_W);i++){
    //     sBSP_SPI_OLED_SendByte(gram);
    // }
    
    sBSP_SPI_OLED_SendBytes(gram,len_bytes);


    portSetCS(1);
}

void sDRV_ST7305_SetAll(uint8_t data){
    portWriteCmd(0x2A);//Column Address Setting S61~S182
    portWriteParam(0x17);
    portWriteParam(0x24); // 0X24-0X17=14 // 14*4*3=168

    portWriteCmd(0x2B);//Row Address Setting G1~G250
    portWriteParam(0x00);
    portWriteParam(0xBF); // 192*2=384

    portWriteCmd(0x2C);   //write image data

    portSetDC(1);
    portSetCS(0);

    // portWriteCmd(parameter);
    for(uint32_t i = 0;i < (SDRV_ST7305_H * SDRV_ST7305_W);i++){
        sBSP_SPI_OLED_SendByte(data);
    }
    portSetCS(1);
}


