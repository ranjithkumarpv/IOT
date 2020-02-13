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
key=""
otp_flag = False
test=""#storing enter otp 
otp_ver_flag=False
opt=""#store otp
fng_mat_flag=False
fng_umat_flag=True
# function to generate OTP

import time
import RPi.GPIO as GPIO       ## Import GPIO library
GPIO.setmode(GPIO.BCM)      ## Use board pin numbering
GPIO.setup(21, GPIO.OUT)      ## Setup GPIO Pin 40 to OUT
GPIO.output(21,False)
def generateOTP() :
  
    # Declare a digits variable   
    # which stores all digits  
    digits = "0123456789"
    OTP = ""   
   # length of password can be chaged 
   # by changing value in range 
    for i in range(4) : 
        OTP += digits[math.floor(random.random() * 10)] 
  
    return OTP

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
    global otp_flag
    global test
    global otp
    global otp_ver_flag
    
    if (k == "A" and otp_flag == False):
        test=""
        otp_flag = True
        #OTP gen
        otp=str(generateOTP())
        print("otp genrated")
        #####mqtt pub
        print("connecting to broker ",broker)
        client.connect(broker)#connect
        print("publishing ")
        client.publish("im/cloudsecurity/otp",otp)#publish
        time.sleep(4)
        print("Enter 4 digit OTP")
    elif (k == "A" and otp_flag == True):
        print ("OTP already send to MQTT Topic, Please check the MQTT Client APP")
    elif (otp_flag == True and len(test)<=4):
        test+=str(k)
        if(len(test)==1):
            print("*")
        if(len(test)==2):
            print("**")
        if(len(test)==3):
            print("***")
        if(len(test)==4):
            print("****")
            if (otp == test):
                print("OTP is Matching")
                otp_ver_flag=True
                otp_flag = False
            else:
                print("Wrong OTP and Pls enter again")
                test=""
        
        
#finger scanner settings
f=Finger()
f.empty()
print("Enroll the Finger:")
f.enroll()     
        

#keypad settings        
factory = rpi_gpio.KeypadFactory()
keypad = factory.create_4_by_4_keypad() # makes assumptions about keypad layout and GPIO pin numbers
key = keypad.registerKeyPressHandler(print_key)
print ("Enter A for start the otp" )

while True:
    global otp_ver_flag
    global fng_mat_flag
    global fng_umat_flag
    if(otp_ver_flag==True):
        print("enter finger for match")
        f.match()
        otp_ver_flag=False
    if(fng_mat_flag==True):
        print("unlock door")
        GPIO.output(21,True)
        time.sleep(30)
        GPIO.output(21,False)
        fng_mat_flag=False
    if(fng_umat_flag == False):
        print("Finger print is not matching and sendfing photo")
        fng_umat_flag=True
        os.system("python3 cam_mail.py")
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
