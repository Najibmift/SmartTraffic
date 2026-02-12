from ultralytics import YOLO
import cv2
import cvzone
import math
import time
import urllib.request
import numpy as np
import pyrebase

firebaseConfig = {
  "apiKey": "",
  "authDomain": "",
  "databaseURL": "",
  "projectId": "",
  "storageBucket": "",
  "messagingSenderId": "",
  "appId": ""
}

firebase=pyrebase.initialize_app(firebaseConfig)

db=firebase.database()

url = "http://xxx.xxx.xxx.xxx/cam-hi.jpg"
cv2.namedWindow("live Cam Testing", cv2.WINDOW_AUTOSIZE)
cap = cv2.VideoCapture(url)  # For Webcam

model = YOLO("yolov8l.pt")

classNames = ["person", "bicycle", "car", "motorbike", "aeroplane", "bus", "train", "truck", "boat",
              "traffic light", "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat",
              "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella",
              "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball", "kite", "baseball bat",
              "baseball glove", "skateboard", "surfboard", "tennis racket", "bottle", "wine glass", "cup",
              "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich", "orange", "broccoli",
              "carrot", "hot dog", "pizza", "donut", "cake", "chair", "sofa", "pottedplant", "bed",
              "diningtable", "toilet", "tvmonitor", "laptop", "mouse", "remote", "keyboard", "cell phone",
              "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors",
              "teddy bear", "hair drier", "toothbrush"
              ]

prev_frame_time = 0
new_frame_time = 0

if not cap.isOpened():
    print("Failed to open the IP camera stream")
    exit()

while True:
    # Read a frame from the video stream
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    im = cv2.imdecode(imgnp, -1)

    # Counter variable for bottles
    car_counter = 0

    # Object Detection
    results = model(im, stream=True)
    for r in results:
        boxes = r.boxes

        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
            w, h = x2 - x1, y2 - y1
            cvzone.cornerRect(im, (x1, y1, w, h))
            conf = math.ceil((box.conf[0] * 100)) / 100
            cls = int(box.cls[0])

            # Check if the detected object is a bottle
            if classNames[cls] == "car" or classNames[cls] == "truck":
                car_counter += 1

                # Display bottle count on the frame
    cv2.putText(im, f'Mobil: {car_counter}', (10, 70), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)

    # Upload hasil perhitungan ke Firebase Realtime Database
    db.child("smart_traffic").update({"object": car_counter})

    # Display the frame with object detection
    cv2.imshow('live Cam Testing', im)

    # Check for user input to exit the loop
    key = cv2.waitKey(5)
    if key == ord('q'):
        break

# Release resources
cap.release()
cv2.destroyAllWindows()
