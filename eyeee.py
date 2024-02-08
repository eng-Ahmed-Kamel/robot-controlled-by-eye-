import cv2
import mediapipe as mp
import serial
import time 
arduino= serial.Serial(port='COM8', baudrate=9600 , timeout = 1)
cam = cv2.VideoCapture (0)
face_mesh=mp.solutions.face_mesh.FaceMesh(refine_landmarks=True)

def move_forward():
    arduino.write(b'F')  # Send 'F' to move forward
    time.sleep(0.005)
    arduino.flush()

def turn_right():
    arduino.write(b'R')  # Send 'B' to move backward
    time.sleep(0.005)
    arduino.flush()

def stop():
    arduino.write(b'S')  # Send 'S' to stop
    time.sleep(0.005)
    arduino.flush()

while True:
    _, frame = cam.read()
    frame = cv2.flip(frame, 1)
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    output = face_mesh.process(rgb_frame)
    landmark_points = output.multi_face_landmarks
    frame_h, frame_w, _ = frame.shape
    if landmark_points:
        landmarks = landmark_points[0].landmark
        ###################################
        right= [landmarks[374], landmarks[386]]
        for landmark in right:
            x = int(landmark.x * frame_w)
            y = int(landmark.y * frame_h)
            cv2.circle(frame, (x, y), 3, (0,0, 255))
        if(right[0].y - right[1].y)<0.009:    
            #forward function
            move_forward()
            time.sleep(1)  # Move for 2 seconds
            stop()
        ###################################
        left = [landmarks[145], landmarks[159]]
        for landmark in left:
            x = int(landmark.x * frame_w)
            y = int(landmark.y * frame_h)
            cv2.circle(frame, (x, y), 3, (0, 255, 255))
        if(left[0].y - left[1].y)<0.009: 
        #turn right function
            turn_right()
            time.sleep(1)  # Move right for 1 seconds
            stop()
        if (left[0].y - left[1].y)<0.009 and (right[0].y - right[1].y)<0.009:
        #stop function
            stop()
        ##################################           
    cv2.imshow('eye controlled robot',frame)
    cv2.waitKey(1)
