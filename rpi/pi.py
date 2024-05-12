from gpiozero import DistanceSensor
from time import sleep
import math
import serial
from ultralytics import YOLO
import cv2
import cvzone

serial_port = '/dev/ttyAMA10'  # Replace with the Arduino port
baud_rate = 9600
ser = serial.Serial(serial_port, baud_rate)
if ser.isOpen():
    print('Serial connected')
else:
    print('Connection failed')

echo_pin = 17  # GPIO pin for ECHO
trigger_pin = 18  # GPIO pin for TRIGGER

# Create an ultrasonic sensor object
ultrasonic_sensor = DistanceSensor(echo=echo_pin, trigger=trigger_pin)
infer = YOLO(r"/path/yolomodel")  # Change this to the path of the yolo model
cap = cv2.VideoCapture(0)
cap.set(3, 640)
cap.set(4, 480)

try:
    while True:
        distance = math.ceil(ultrasonic_sensor.distance * 100)
        print(f"Distance: {distance} cm")
        if 25 <= distance < 30:
            _, img = cap.read()
            results = infer(img)  # get the prediction of the model using YOLO
            for r in results:
                boxes = r.boxes
                for box in boxes:
                    x1, y1, x2, y2 = box.xyxy[0]
                    x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)  # get the values of the bounding box
                    print(x1, y1, x2, y2)
                    w, h = x2 - x1, y2 - y1  # Calculate width and height
                    print(w, h)
                    message = f'{w}x{h}'  # Send the width and height of the object detected to Arduino
                    ser.write(message.encode())
                    print('Data sent to Arduino ', message)
                    cvzone.cornerRect(img, (x1, y1, w, h))
                    conf = math.ceil(box.conf[0] * 100) / 100
                    cls = int(box.cls[0])
                    cvzone.putTextRect(img, f'Object {conf}', (max(0, x1), max(20, y1)))
            cv2.imshow('Image', img)  # Display the image with the bounding box of the object
            cv2.waitKey(5000)
        # Introduce a small delay
        # sleep(5)

except KeyboardInterrupt:
    pass
finally:
    # Release the camera and close the serial port
    cap.release()
    cv2.destroyAllWindows()
    ser.close()
    ultrasonic_sensor.close()
