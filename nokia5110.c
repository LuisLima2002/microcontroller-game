/*
 * nokia5110.c
 *
 *  Created on: 5 de mar de 2022
 *      Author: luise
 */

#include "nokia5110.h"


static const uint8_t ASCII[][5] = {
         {0x00, 0x00, 0x00, 0x00, 0x00} // 20
        ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
        ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
        ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
        ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
        ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
        ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
        ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
        ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
        ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
        ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
        ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
        ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
        ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
        ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
        ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
        ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
        ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
        ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
        ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
        ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
        ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
        ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
        ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
        ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
        ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
        ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
        ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
        ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
        ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
        ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
        ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
        ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
        ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
        ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
        ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
        ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
        ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
        ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
        ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
        ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
        ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
        ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
        ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
        ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
        ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
        ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
        ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
        ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
        ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
        ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
        ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
        ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
        ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
        ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
        ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
        ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
        ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
        ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
        ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
        ,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c '\'
        ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
        ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
        ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
        ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
        ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
        ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
        ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
        ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
        ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
        ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
        ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
        ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
        ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
        ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
        ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
        ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
        ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
        ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
        ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
        ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
        ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
        ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
        ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
        ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
        ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
        ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
        ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
        ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
        ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
        ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
        ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
        ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
        ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
        ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
        //  ,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
        ,{0x1f, 0x24, 0x7c, 0x24, 0x1f} // 7f UT sign
        ,{0x00, 0x06, 0x09, 0x09, 0x06} // 80 celcius
};

uint8_t Screen[84 * 48 / 8]; // Buffer stores the next image to be printed on the screen
const unsigned char Masks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // Utilizado na função Nokia5110_ClrPxl
uint32_t g_ui32SysClockFreq;

void static enableSSI0(void){
    //SSI0
    // ENABLE PERIPHERAL
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    //Configuring Pin
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);
    GPIOPinTypeSSI(GPIO_PORTA_BASE,GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4);

    ///Configuring SSI0 and Enable
    SSIDisable(SSI0_BASE);
    SSIConfigSetExpClk(SSI0_BASE,g_ui32SysClockFreq ,SSI_FRF_MOTO_MODE_0 ,SSI_MODE_MASTER,100000,8);
    while(SSIBusy(SSI0_BASE)){}
}

void lcdWrite(uint8_t dc,uint8_t message){
    if(dc==0){ //COMMAND
        while(SSIBusy(SSI0_BASE)){}
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
        //SysCtlDelay(10000);
        SSIDataPut(SSI0_BASE,message);
        while(SSIBusy(SSI0_BASE)){}
    }else{//Data
        while(SSIBusy(SSI0_BASE)){}
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
        //SysCtlDelay(10000);
        SSIDataPut(SSI0_BASE,message);
    }
}

void nokia5510init(uint32_t SysClockFreq){
/*
       3.3V          (VCC, pin 1) power
       Ground        (GND, pin 2) ground
       SSI0Fss       (SCE, pin 3) connected to PA3
       Reset         (RST, pin 4) connected to PA7
       Data/Command  (D/C, pin 5) connected to PA6
       SSI0Tx        (DN,  pin 6) connected to PA4
       SSI0Clk       (SCLK, pin 7) connected to PA2
       back light    (LED, pin 8) connected to PA6
*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlDelay(5000);
    g_ui32SysClockFreq = SysClockFreq;
    enableSSI0();
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);
    //Reset to a known state
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0x00);
    SysCtlDelay(g_ui32SysClockFreq/10); //delay >100ns
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_PIN_7);
    SSIEnable(SSI0_BASE);
    while(SSIBusy(SSI0_BASE)){}
    lcdWrite(0, 0x0C);              // Set display control to normal mode: 0x0D for inverse
    lcdWrite(0, 0x21);              //Chip active; horizontal addressing mode (V = 0); use extended instruction set (H = 1)
    lcdWrite(0, 0xBF);              // 0xB8 (for 3.3V blue SparkFun)
    lcdWrite(0, 0x04);              // Set temp coefficient
    lcdWrite(0, 0x14);              // LCD bias mode 1:48: try 0x13 or 0x14
    lcdWrite(0, 0x20);              // We must send 0x20 before modifying the display control mode
    lcdWrite(0, 0x0C);              // Set display control to normal mode: 0x0D for inverse
}

void nokia5110_SetCursor(uint8_t newX, uint8_t newY)
{
    if((newX > 84) || (newY > 5))        // Bad input
        return;

    // Multiply newX by 7 because each character is 7 columns wide
    lcdWrite(0, 0x80 | (newX));   // Setting bit 7 updates X-position
    lcdWrite(0, 0x40 | newY);     // Setting bit 6 updates Y-position
}

void nokia5110_ClrPxl(uint32_t j, uint32_t i)
{
    Screen[84 * (j >> 3) + i] &= ~Masks[j & 0x07];
}


void nokia5110_SetPxl(uint32_t j, uint32_t i)
{
    Screen[84 * (j >> 3) + i] |= Masks[j & 0x07];
}

void nokia5110_DrawFullImage(const uint8_t *ptr)
{
    int i;

    nokia5110_SetCursor(0, 0);

    for(i = 0; i < (84 * 48 / 8); i = i + 1)
        lcdWrite(1,ptr[i]);
}

void nokia5110_displayScreen(void){
    nokia5110_DrawFullImage(Screen);
}


void nokia5110_OutChar(char data)
{
    int i;
    //lcddatawrite(0x00); // Blank vertical line padding

    for(i = 0; i < 5; i = i + 1)
        lcdWrite(1,ASCII[data - 0x20][i]);

    lcdWrite(1,0x00); // Blank vertical line padding
}

void nokia5110_OutString(char *ptr)
{
    while(*ptr)
    {
        nokia5110_OutChar((unsigned char) * ptr);
        ptr = ptr + 1;
    }
}

void nokia5110_OutDec(uint16_t n)
{
    if(n < 10)
    {
        //Nokia5110_OutString("    ");
        nokia5110_OutChar(n + '0'); // n is between 0 and 9
    }

    else if(n < 100)
    {
        //Nokia5110_OutString("   ");
        nokia5110_OutChar(n/10+'0'); // tens digit
        nokia5110_OutChar(n%10+'0'); // ones digit
    }

    else if(n < 1000)
    {
        //Nokia5110_OutString("  ");
        nokia5110_OutChar(n / 100 + '0'); n = n % 100; // Hundreds digit

        nokia5110_OutChar(n / 10 + '0'); // tens digit
        nokia5110_OutChar(n % 10 + '0'); // ones digit
    }

    else if(n < 10000)
    {
        //Nokia5110_OutChar(' ');
        nokia5110_OutChar(n / 1000 + '0'); n = n % 1000; // Thousands digit

        nokia5110_OutChar(n / 100 + '0'); n = n % 100;// Hundreds digit

        nokia5110_OutChar(n / 10 + '0'); // Tens digit
        nokia5110_OutChar(n % 10 + '0'); // Ones digit
    }

    else
    {
        nokia5110_OutChar(n / 10000 + '0'); n = n % 10000;  // Ten-thousands digit

        nokia5110_OutChar(n / 1000 + '0'); n = n % 1000;    // Thousands digit

        nokia5110_OutChar(n / 100 + '0'); n = n % 100;      // Hundreds digit

        nokia5110_OutChar(n / 10 + '0');                    // Tens digit
        nokia5110_OutChar(n % 10 + '0');                    // Ones digit
    }
}

void nokia5110_Clear(void)
{
    int i;

    for(i = 0; i < (84 * 48 / 8); i = i + 1)
        lcdWrite(1,0x00);

    nokia5110_SetCursor(0, 0);
}

