import serial
import time
import numpy
import csv

sg=serial.Serial("COM8",115200)
vna=serial.Serial("COM23",115200)
Start_Freq=1  ##MHz unit
Stop_Freq=300   ##MHz unit
Step_Freq=1     ##MHz unit
Wait=0.001      ##sec
LO=[-5,-5]      ##dBm unit. 50ohm loaded.
Port='1'        ##Stimulus port. 1/2/3/4
Freq=[100,100]  ##ch3&4, MHz unit
Pha=[0,0]       ##ch3&4, deg. unit
Ampl=[-10,-10]  ##ch3&4, dBm unit. 50ohm loaded.

step=int((Stop_Freq-Start_Freq)/Step_Freq)
data=numpy.zeros((step,9))
for i in range(step):
    freq=Start_Freq+i*Step_Freq
    buf=f'{freq*1000000:09}'+'000'+f'{LO[0]:+03}'+f'{freq*1000000:09}'+'000'+f'{LO[1]:+03}'+f'{Freq[0]*1000000:09}'+f'{Pha[0]:03}'+f'{Ampl[0]:+03}'+f'{Freq[1]*1000000:09}'+f'{Pha[1]:03}'+f'{Ampl[1]:+03}'
    sg.write(buf.encode())
    time.sleep(Wait)
    vna.write(Port.encode())
    buf=vna.read(5)
    pha2=int(buf)
    buf=vna.read(5)
    mag2=int(buf)
    buf=vna.read(5)
    pha1=int(buf)
    buf=vna.read(5)
    mag1=int(buf)
    buf=vna.read(5)
    pha4=int(buf)
    buf=vna.read(5)
    mag4=int(buf)
    buf=vna.read(5)
    pha3=int(buf)
    buf=vna.read(5)
    mag3=int(buf)
    print('Freq=',int(freq*1000000),'Hz')
    data[i][0]=int(freq*1000)
    data[i][1]=int(mag1)
    data[i][2]=int(pha1)
    data[i][3]=int(mag2)
    data[i][4]=int(pha2)
    data[i][5]=int(mag3)
    data[i][6]=int(pha3)
    data[i][7]=int(mag4)
    data[i][8]=int(pha4)
    
with open('data.csv','w',newline="") as f:
    writer=csv.writer(f)
    writer.writerows(data)