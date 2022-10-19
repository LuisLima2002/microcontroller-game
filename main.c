#include "nokia5110.h"
#include "inc/hw_gpio.h"

uint32_t g_ui32SysClockFreq;

const uint8_t  player0[8]={0b01000001,0b01110111,0b01111111,0b00111110,0b00111110,0b00011100,0b00011100,0b00001000},obstacle[8]={0b00111100,0b01110110,0b01111011,0b01111011,0b01011111,0b01011110,0b01111100,0b00111000},shot=0b00001000;
uint8_t playerY=3,obstacleV=1,count=0;
int obstacleX[4]={75,125,100,110},shotX[4]={199,199,199,199},pont=0;
bool gameOver=0;
void IntHandler (void){
    int8_t pinRead = GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0|GPIO_PIN_1);
    if((pinRead & GPIO_PIN_0)==0x00){
    //PIN0
        //Limpa posição atual
        nokia5110_SetCursor(0,playerY);
        int i;
        for(i=0;i<8;i++){
            lcdWrite(1,0x00);
        }

        if(playerY<5){
            playerY++;
        }

        nokia5110_SetCursor(0,playerY);
        for(i=0;i<8;i++){
            lcdWrite(1,player0[i]);
        }
        //shot
        lcdWrite(1,shot);
        lcdWrite(1,shot);
        shotX[playerY-2]=8;
    }else if(((pinRead & GPIO_PIN_1)==0x00)){
        //PIN1
        //Limpa posição atual
        nokia5110_SetCursor(0,playerY);
        int i;
        for(i=0;i<8;i++){
            lcdWrite(1,0x00);
        }

        if(playerY>2){
            playerY--;
        }

        nokia5110_SetCursor(0,playerY);
        for(i=0;i<8;i++){
            lcdWrite(1,player0[i]);
        }
        //shot
        lcdWrite(1,shot);
        lcdWrite(1,shot);
        shotX[playerY-2]=8;
    }

}

void SysticklHandler(void){
    if(gameOver)    return;
    //move obstacle
    int i,j;
    for(j=0;j<4;j++){
        obstacleX[j]-=obstacleV;
       if(abs(shotX[j]- obstacleX[j])<=1+obstacleV){
                nokia5110_SetCursor(shotX[j]-obstacleV,j+2);
                for(i=0;i<15+obstacleV;i++){
                    lcdWrite(1,0x00);
                }
                obstacleX[j]= (rand() % 50)+80;
                shotX[j]=200;
                pont++;
                nokia5110_SetCursor(40,0);
                nokia5110_OutDec(pont);

        }
       //move shot
       if(shotX[j]<81){
                   nokia5110_SetCursor(shotX[j]-1,j+2);
                   lcdWrite(1,0x00);
                   lcdWrite(1,0x00);
                   nokia5110_SetCursor(shotX[j],j+2);
                   lcdWrite(1,shot);
                   lcdWrite(1,shot);
                   shotX[j]++;

        }
        if(obstacleX[j]<=8){
            //GAME OVER
            nokia5110_Clear();
            nokia5110_SetCursor(15,1);
            nokia5110_OutString("GAME OVER");
            nokia5110_SetCursor(15,4);
            nokia5110_OutString("POINTS: ");
            nokia5110_OutDec(pont);
            gameOver=1;
            return;
        }
        if(obstacleX[j]<75){
            nokia5110_SetCursor(obstacleX[j]+obstacleV,j+2);
            for(i=0;i<8;i++){
                lcdWrite(1,0x00);
            }
            nokia5110_SetCursor(obstacleX[j],j+2);
            for(i=0;i<8;i++){
                lcdWrite(1,obstacle[i]);
            }
        }
    }

    //increase difficult
    if(obstacleV<5){
        if(count==29){
            obstacleV++;
            count=0;
        }else{
            count++;
        }
    }
    IntHandler();
}

/*
 *  Função responsavel por Configurar a interrupção
 */
void
IntSetup(void){
    //Enable Ports
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);

    //Enable input pins
    while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)));
    HWREG(GPIO_PORTJ_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTJ_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTJ_BASE + GPIO_O_LOCK) = 0;
    GPIODirModeSet(GPIO_PORTJ_BASE, GPIO_PIN_1|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_1|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

int main(void)
{
    g_ui32SysClockFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
            SYSCTL_OSC_MAIN |
            SYSCTL_USE_PLL |
            SYSCTL_CFG_VCO_320),16000000);//clock at 16mhz
    nokia5510init(g_ui32SysClockFreq);
    nokia5110_Clear();
    //Setup game
    nokia5110_SetCursor(0,1);
        int i;
        for(i=0;i<83;i++){
            lcdWrite(1,0b00000100);
    }
    nokia5110_SetCursor(0,playerY);
    for(i=0;i<8;i++){
        lcdWrite(1,player0[i]);
    }
    IntSetup();
    IntMasterEnable();
    SysTickPeriodSet((g_ui32SysClockFreq/10) -1);///10fps
    SysTickIntEnable();
    SysTickEnable();
    while(!gameOver){
    }
}
