import struct as st
import serial as ser
import time,sys
import os

head=[0xef01,0x00000000,0x01]
from FingerPrint import FingerPrint
#keypad modules
from pad4pi import rpi_gpio
import time
#OTP module
import math, random
#mqtt
import paho.mqtt.client as paho
#mqtt broker 
broker="iot.eclipse.org"
#define mqtt client
client= paho.Client("client-001") #create client object client1.on_publish = on_publish #assign function to callback client1.connect(broker,port) #establish connection client1.publish("house/bulb1","on")

#global var
start_flag=False
motor_flag=False
fng_mat_flag=False
fng_umat_flag=True
# function to generate OTP

import time
import RPi.GPIO as GPIO       ## Import GPIO library
GPIO.setmode(GPIO.BCM)      ## Use board pin numbering
GPIO.setup(21, GPIO.OUT)      ## Setup GPIO Pin 40 to OUT
GPIO.output(21,False)

class Finger:
    def __init__(self):
        self.f=FingerPrint()
    def enroll(self):
        input("Put Finger and Press Enter:")
        self.f.getimg()
        self.f.genchar(1)
        r=self.f.search()
        if(r!=-1):
           print("Finger Already Taken at:"+str(r))
        input("Put Finger Again:")
        self.f.getimg()
        self.f.genchar(2)
        y=self.f.regmodel()
        if y:
          fid=input("Enter ID (0-254):")
          self.f.store(fid)
        else:
           print("Error")
    def match(self):
        global fng_mat_flag
        global fng_umat_flag
        input("Want to search:")
        self.f.getimg()
        self.f.genchar(1)
        r=self.f.search()
        if(r!=-1):
            fng_mat_flag=True
            print("Finger Found at:"+str(r))
        else:
            fng_umat_flag=False
    def delete(self):
        fid=input("Enter Start Address")
        n=input("Enter No of Fingerprints")
        self.f.delete(fid,n)
    def empty(self):
        self.f.empty()

def print_key(k):
    global start_flag
    global motor_flag
    if (k==1 and motor_flag== False):
        print ("put finger for verification")
        start_flag =True
    elif(motor_flag==True):
        print("motor is running")
        
    if(k == 2):
        print("stop motor")
        GPIO.output(21,False)
        motor_flag=False    
    if (k == 3):
        print("connecting to broker ",broker)
        client.connect(broker)#connect
        print("publishing ")
        client.publish("im/vehsecurity/SOS","SOS_ON")#publish
        time.sleep(4)
        os.system("sudo python3 gps.py")
        
        
#finger scanner settings
f=Finger()
f.empty()
print("Enroll the Finger:")
f.enroll()

        

#keypad settings        
factory = rpi_gpio.KeypadFactory()
keypad = factory.create_keypad() # makes assumptions about keypad layout and GPIO pin numbers
key = keypad.registerKeyPressHandler(print_key)
print ("Enter 1: vec_start 2: vec_stop 3: SOS and GPS")

while True:
    global start_flag
    global motor_flag
    global fng_mat_flag
    global fng_umat_flag
    if(start_flag==True):
        start_flag=False
        print("enter finger for match")
        f.match()
    if(fng_mat_flag==True):
        print("connecting to broker ",broker)
        client.connect(broker)#connect
        print("publishing know")
        client.publish("im/vehsecurity/auth","known")#publish
        time.sleep(4)
        print("motor start")
        GPIO.output(21,True)
        fng_mat_flag=False
        motor_flag=True
    if(fng_umat_flag == False):
        print("Finger print is not matching and unknown person")
        fng_umat_flag=True
        client.connect(broker)#connect
        print("publishing unknow")
        client.publish("im/vehsecurity/auth","unknown")#publish
        time.sleep(4)
        
    time.sleep(1)


#    i=input("1:Enroll\t2:Match\t 3:Delete\t4:Empty\t5:Exit\n")
#    if(i=='1'):
#        f.enroll()
#    elif(i=='2'):
#        f.match();
#    elif(i=='3'):
#        f.delete()
#    elif(i=='4'):
#        f.empty()
#    else:
#        break;
