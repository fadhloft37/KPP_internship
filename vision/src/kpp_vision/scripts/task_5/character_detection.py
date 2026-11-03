import cv2
import numpy as np
from ultralytics import YOLO

class HK:
    def __init__(self, weight_path, conf=0.5, imgsz=640, max_det=1, device=None):
        self.model = YOLO(weight_path)
        self.conf = conf
        self.imgsz = imgsz
        self.max_det = max_det
        self.device = device

    def detect(self, frame):
        results = self.model.predict(
            source=frame,
            conf=self.conf,
            imgsz=self.imgsz,
            max_det=self.max_det,
            device=self.device,
            verbose=False
        )

        detections = []
        for box in results[0].boxes:
            x1, y1, x2, y2 = box.xyxy[0].tolist()
            score = float(box.conf[0])
            detections.append((int(x1), int(y1), int(x2), int(y2), score))
            
        return detections

def draw(frame, detections):
    for x1, y1, x2, y2, score in detections:
        cx = (x1 + x2) // 2
        cy = (y1 + y2) // 2

        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 3)
        cv2.circle(frame, (cx, cy), 5, (0, 0, 255), -1) 

        label = "HK ({}, {}) {:.2f}".format(cx, cy, score)
        cv2.putText(frame, label, (x1, max(y1 - 10, 25)), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)

    return frame