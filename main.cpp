#include "mbed.h"

DigitalOut mDIN_H(dp1);
DigitalOut mDIN_L(dp2);

Serial mPc(USBTX, USBRX);

//define
const int mModeNormal= 0;
const int mModeHigh   =1;
const int mModeOne   =2;
const int mModeAll    =3;
//const int mModeFadeIn =4;
const int mMaxPlay=4;

const int mBit_MAX=8;
const int mLED_MAX=4;
const int mWait_US=5;
// const int mColor_White=0;

const unsigned char mColorMap[9][3] = {
  {0xff,0xa5,0x00 }, // #ffa500 Orange
  {0xd2,0x69,0x1e }, // #d2691e chocolate
  {0x80,0x00,0x00 }, // #800000 red 
  {0x80,0x00,0x80 }, // #800080 purple
  {0x00,0x80,0x00 }, // #008000 green
  {0xff,0xff,0x00 }, // #ffff00 Yellow
  {0x00,0x00,0x80 }, // #000080 navy
  {0x00,0xff,0xff }, // #00ffff aqua
  {0x40,0x40,0x40 }, // #404040 ｗhite
  //{0x80,0x80,0x00 }, // #808000 olieve
};

//
int Is_bitPosition(int src , int Position){
    int iRet=0;
        int    iLen=mBit_MAX;
        char    buff[mBit_MAX+1];
        int     bin;
        int     i1;

        bin = src;
        buff[iLen] = '\0';
        for(int j = 0; j < iLen; j++){
            buff[j]='0';
        }
        
        for(i1 = 0; i1 < iLen; i1++){
                if(i1 != 0 && bin == 0)
                        break;
                if(bin % 2 == 0)
                        buff[(iLen-1)-i1] = '0';
                else
                        buff[(iLen-1)-i1] = '1';
                bin = bin / 2;
        }
//printf(">>>%s\n", buff );
    if(strlen(buff) >= 8 ){
      if(buff[Position]=='1'){ iRet=1; }
    }
   return iRet;
}

//
void LED_Set() {
  mDIN_H=0;
  mDIN_L=1;
  wait_ms(1);
}

//
void LED_Low_Bit() {
  mDIN_H=0;
  mDIN_L=0;
  wait_us(mWait_US);
  mDIN_H=0;
  mDIN_L=1;
  wait_us(mWait_US);
}

//
void LED_Hi_Bit() {
  mDIN_H=1;
  mDIN_L=1;
  wait_us(mWait_US);
  mDIN_H=0;
  mDIN_L=1;
  wait_us(mWait_US);
}

//
void LED_Init() {
  mDIN_H=0;
  mDIN_L=1;
  wait_ms(10); 
}

//
void LED_Color_RGB(unsigned int led_r, unsigned int led_g, unsigned int led_b) {
//printf("bit_r=%d, g=%d, b=%d \n" , led_r, led_g, led_b);
  //blue
  for (int k = 0; k < mBit_MAX; k++){
      if( Is_bitPosition(led_b , k)==1){
          LED_Hi_Bit();
      }else{
          LED_Low_Bit();
      }
  }
  //green
  for (int k = 0; k < mBit_MAX; k++){
      if( Is_bitPosition(led_g , k)==1){
          LED_Hi_Bit();
      }else{
          LED_Low_Bit();
      }
  }
  //red
  for (int k = 0; k < mBit_MAX; k++){
      if( Is_bitPosition(led_r , k)==1){
          LED_Hi_Bit();
      }else{
          LED_Low_Bit();
      }
  }
}
//
void init_proc(){
    mDIN_H =0;
    mDIN_L =1;

    LED_Init();
    wait_ms(10);
}

//
void proc_display(int iCt, int iMode){
    if(iMode==mModeNormal){
        for(int n=0;n< mLED_MAX;n++){
          LED_Color_RGB(mColorMap[iCt][0], mColorMap[iCt][1], mColorMap[iCt][2] );
          wait_ms(500);
        }
        LED_Set();  
    }else if(iMode==mModeHigh){
        //HIGH
        for(int j=0; j < 3; j++){
            for(int n=0;n< mLED_MAX;n++){
              LED_Color_RGB(0x00, 0x00, 0x00);
            }
            LED_Set();
            wait_ms(200);            
            for(int n=0;n< mLED_MAX;n++){
              LED_Color_RGB(mColorMap[iCt][0], mColorMap[iCt][1], mColorMap[iCt][2] );
              wait_ms(120);
            }
            LED_Set();
        }
    }else if(iMode==mModeOne){
            //One
            for(int n=0;n< mLED_MAX;n++){
                //set
                for(int k=0;k< mLED_MAX; k++){
                    if(n==k){
                        LED_Color_RGB(mColorMap[iCt][0], mColorMap[iCt][1], mColorMap[iCt][2] );
                    }
                    else{
                        LED_Color_RGB(0x00, 0x00, 0x00);
                    }
                }
                LED_Set();
                wait_ms(500);
            }
            for(int j=0;j< mLED_MAX; j++){
                LED_Color_RGB(mColorMap[iCt][0], mColorMap[iCt][1], mColorMap[iCt][2] );
            }
            LED_Set();
            wait_ms(1000);
    }else if(iMode ==mModeAll){
            for(int n=0;n< mLED_MAX;n++){
              LED_Color_RGB(mColorMap[iCt][0], mColorMap[iCt][1], mColorMap[iCt][2] );
            }
            LED_Set();
            wait(3);
    }
    /*
    else{
        int iFadceCt=50;
        int iFade_r=mColorMap[iCt][0];
        int iFade_g=mColorMap[iCt][1];
        int iFade_b=mColorMap[iCt][2];
        int idev_r=(int)mColorMap[iCt][0] / iFadceCt;
        int idev_g=(int)mColorMap[iCt][1] / iFadceCt;
        int idev_b=(int)mColorMap[iCt][2] / iFadceCt;
        for(int j=0; j < iFadceCt; j++){
            int iTmp_r=iFade_r -(idev_r * j);
            int iTmp_g=iFade_g -(idev_g * j);
            int iTmp_b=iFade_b -(idev_b * j);
            for(int n=0;n< mLED_MAX;n++){
                iTmp_r=iTmp_r- (idev_r * n);
                iTmp_g=iTmp_g- (idev_g * n);
                iTmp_b=iTmp_b- (idev_b * n);
                LED_Color_RGB(iTmp_r, iTmp_g, iTmp_b );
                wait_ms(5);
            }
            LED_Set();
            wait_ms(100);
        } 
        for(int n=0;n< mLED_MAX;n++){
          LED_Color_RGB(0x00, 0x00, 0x00);
        }
        LED_Set();          
    }    
    */
}  

//    
int main() {
    mPc.baud(9600 );
    mPc.printf("#Start-main\n");
    wait(3); //wait-Start
    int iWait=2;    
    init_proc();
    int iCt=0;
//    unsigned long iCtPlay=0;
    int iCtPlay=0;
    while(1) {
        int iMode=iCtPlay % mMaxPlay;
        //reset
        for(int n=0;n< mLED_MAX;n++){
          LED_Color_RGB(0x00, 0x00, 0x00);
        }
        LED_Set();
        wait_ms(1000);
        proc_display(iCt, iMode);
mPc.printf("#iCt= %d\n" ,iCt);
mPc.printf("#iCtPlay= %d ,iMode=%d \n" ,iCtPlay, iMode);
        wait(iWait);
        iCt    =iCt +1;
        iCtPlay=iCtPlay+1;
        if(iCt >= 9 ){ iCt=0; }
        if(iCtPlay >= 10000 ){ iCtPlay=0; }
    }    
}

