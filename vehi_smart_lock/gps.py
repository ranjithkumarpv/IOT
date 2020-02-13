import serial               #import serial pacakge
from time import sleep
import sys                  #import system package
import paho.mqtt.client as paho

def GPS_Info():
    #mqtt broker 
    broker="iot.eclipse.org"
    #define mqtt client
    client= paho.Client("client-001")
    client.connect(broker)#connect
    print("publishing ")
    
    global NMEA_buff
    global lat_in_degrees
    global long_in_degrees
    nmea_time = []
    nmea_latitude = []
    nmea_longitude = []
    nmea_time = NMEA_buff[0]                    #extract time from GPGGA string
    nmea_latitude = NMEA_buff[1]                #extract latitude from GPGGA string
    nmea_longitude = NMEA_buff[3]               #extract longitude from GPGGA string
    
    print("NMEA Time: ", nmea_time,'\n')
    print ("NMEA Latitude:", nmea_latitude,"NMEA Longitude:", nmea_longitude,'\n')
    client.publish("im/vehsecurity/gps",str(nmea_latitude)+str(nmea_longitude))#publish
    
    
    
gpgga_info = "$GPGGA,"
ser = serial.Serial ("/dev/ttyS0")              #Open port with baud rate
GPGGA_buffer = 0
NMEA_buff = 0
lat_in_degrees = 0
long_in_degrees = 0

try:
    while True:
        received_data = (str)(ser.readline())                   #read NMEA string received
        GPGGA_data_available = received_data.find(gpgga_info)   #check for NMEA GPGGA string                 
        if (GPGGA_data_available>0):
            GPGGA_buffer = received_data.split("$GPGGA,",1)[1]  #store data coming after "$GPGGA," string 
            NMEA_buff = (GPGGA_buffer.split(','))               #store comma separated data in buffer
            GPS_Info()                                          #get time, latitude, longitude
            #press ctrl+c to plot on map and exit 
            print("------------------------------------------------------------\n")
            sys.exit(0)
                        
except KeyboardInterrupt:
    sys.exit(0)
