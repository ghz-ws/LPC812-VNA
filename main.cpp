#include "mbed.h"

Serial pc(P0_4,P0_0);
SPI spi(P0_6, P0_7, P0_16);    //mosi, miso, sclk
DigitalOut cs1(P0_17);
DigitalIn drdy1(P0_13);
DigitalOut cs2(P0_8);
DigitalIn drdy2(P0_9);
DigitalOut sw1(P0_12);
DigitalOut sw2(P0_14);
DigitalOut sw3(P0_15);
DigitalOut sw4(P0_1);

//ADC
const uint8_t rst=0b0110;
const uint8_t wreg=0b0100;
const uint8_t start=0b1000;
uint16_t adc1_read(uint8_t ch);
uint16_t adc2_read(uint8_t ch);

//vals
uint16_t val[8];
uint8_t sel;

int main(){
    //init
    pc.baud(115200);
    cs1=1;
    spi.format(8,1);

    //adc init
    cs1=0;
    cs2=0;
    spi.write(rst);
    cs1=1;
    cs2=1;
    wait(0.01);

    cs1=0;
    cs2=0;
    spi.write((wreg<<4)+(0x01<<2)+0);
    spi.write((0b110<<5)+(0b10<<3)+(0b1<<2));       //2ksps, turbo mode, cc mode
    cs1=1;
    cs2=1;
    wait(0.01);
    
    while (true){
        sel=pc.getc();

        switch(sel){
            case 0x31:  //port1 1
                sw1=0;
                sw2=1;
                sw3=1;
                sw4=1;
                break;
            case 0x32:  //port2 2
                sw1=1;
                sw2=0;
                sw3=1;
                sw4=1;
                break;
            case 0x33:  //port3 3
                sw1=1;
                sw2=1;
                sw3=0;
                sw4=1;
                break;
            case 0x34:  //port4 4
                sw1=1;
                sw2=1;
                sw3=1;
                sw4=0;
                break;
            default:
                sw1=1;
                sw2=1;
                sw3=1;
                sw4=1;
        }
        wait(0.01);

        val[0]=adc1_read(1);
        val[1]=adc1_read(2);
        val[2]=adc1_read(3);
        val[3]=adc1_read(4);
        val[4]=adc2_read(1);
        val[5]=adc2_read(2);
        val[6]=adc2_read(3);
        val[7]=adc2_read(4);
        
        pc.printf("%05d%05d%05d%05d%05d%05d%05d%05d",val[0],val[1],val[2],val[3],val[4],val[5],val[6],val[7]);
    }
}

//adc1 read func.
uint16_t adc1_read(uint8_t ch){
    uint8_t mux;
    uint8_t buf[2];
    int16_t val;
    if(ch==1){
        mux=0b1000;
    }else if(ch==2){
        mux=0b1001;
    }else if(ch==3){
        mux=0b1010;
    }else if(ch==4){
        mux=0b1011;
    }
    cs1=0;
    spi.write((wreg<<4)+0b0000);
    spi.write((mux<<4)+1);
    cs1=1;
    cs1=0;
    spi.write(start);
    cs1=1;
    while(true){
        if(drdy1==0) break;
    }
    cs1=0;
    buf[1]=spi.write(0x00);
    buf[0]=spi.write(0x00);
    cs1=1;
    val=(buf[1]<<8)+buf[0];
    if(val<0){
        val=0;
    }
    return (uint16_t)val;
}

//adc1 read func.
uint16_t adc2_read(uint8_t ch){
    uint8_t mux;
    uint8_t buf[2];
    int16_t val;
    if(ch==1){
        mux=0b1000;
    }else if(ch==2){
        mux=0b1001;
    }else if(ch==3){
        mux=0b1010;
    }else if(ch==4){
        mux=0b1011;
    }
    cs2=0;
    spi.write((wreg<<4)+0b0000);
    spi.write((mux<<4)+1);
    cs2=1;
    cs2=0;
    spi.write(start);
    cs2=1;
    while(true){
        if(drdy2==0) break;
    }
    cs2=0;
    buf[1]=spi.write(0x00);
    buf[0]=spi.write(0x00);
    cs2=1;
    val=(buf[1]<<8)+buf[0];
    if(val<0){
        val=0;
    }
    return (uint16_t)val;
}