# This is a demo of running face recognition on a Raspberry Pi.
# This program will print out the names of anyone it recognizes to the console.

# To run this, you need a Raspberry Pi 2 (or greater) with face_recognition and
# the picamera[array] module installed.
# You can follow this installation instructions to get your RPi set up:
# https://gist.github.com/ageitgey/1ac8dbe8572f3f533df6269dab35df65

import face_recognition
import picamera
import numpy as np
import RPi.GPIO as GPIO
import time

GPIO.setwarnings(False)    # Ignore warning for now
GPIO.setmode(GPIO.BCM)   # Use physical pin numbering

GPIO.setup(17, GPIO.OUT, initial=GPIO.LOW) #known
GPIO.setup(18, GPIO.OUT, initial=GPIO.LOW) #unknown
GPIO.setup(4, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #push button

# Get a reference to the Raspberry Pi camera.
# If this fails, make sure you have a camera connected to the RPi and that you
# enabled your camera in raspi-config and rebooted first.
camera = picamera.PiCamera()
camera.resolution = (320, 240)
output = np.empty((240, 320, 3), dtype=np.uint8)

# Load a sample picture and learn how to recognize it.
print("Loading known face image(s)")
obama_image = face_recognition.load_image_file("2.jpg")
obama_face_encoding = face_recognition.face_encodings(obama_image)[0]

# Initialize some variables
face_locations = []
face_encodings = []

while True:
    if GPIO.input(4) == GPIO.HIGH:
        print("Capturing image.")
        # Grab a single frame of video from the RPi camera as a numpy array
        camera.capture(output, format="rgb")

        # Find all the faces and face encodings in the current frame of video
        face_locations = face_recognition.face_locations(output)
        print("Found {} faces in image.".format(len(face_locations)))
        face_encodings = face_recognition.face_encodings(output, face_locations)

        # Loop over each face found in the frame to see if it's someone we know.
        for face_encoding in face_encodings:
            # See if the face is a match for the known face(s)
            match = face_recognition.compare_faces([obama_face_encoding], face_encoding)
            name = "<Unknown Person>"
            GPIO.output(18, GPIO.HIGH)
            time.sleep(60)
            GPIO.output(18, GPIO.LOW)

            if match[0]:
                name = "venkatesh"
                GPIO.output(17, GPIO.HIGH)

            print("I see someone named {}!".format(name))

    else:
        print ("press button to see output")
        time.sleep(60)
