#!/usr/bin/env python3
import cv2
import numpy as np

PROC_SCALE = 0.5
THRESH_VAL = 20
CLOSE_KSIZE = 7
MIN_AREA = 10
_KERNEL = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (CLOSE_KSIZE, CLOSE_KSIZE))

def locate_object(frame):
    # 1. RESIZE
    resized = cv2.resize(frame, None, fx=PROC_SCALE, fy=PROC_SCALE, interpolation=cv2.INTER_AREA)

    # 2. GRAYSCALE
    gray = cv2.cvtColor(resized, cv2.COLOR_BGR2GRAY)

    # 3. MASK
    _, mask = cv2.threshold(gray, THRESH_VAL, 255, cv2.THRESH_BINARY)

    # 4. CLOSING
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, _KERNEL)

    # 5. CONTOURS
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # 6. NOISE FILTERING, BOUNDING BOXES, AND SCALING BACK
    boxes = []
    inv = 1.0/PROC_SCALE
    for c in contours:
        if cv2.contourArea(c) < MIN_AREA:
            continue
        x,y,w,h = cv2.boundingRect(c)
        boxes.append((int(x*inv), int(y*inv), int(w*inv), int(h*inv)))

    return boxes

def draw(boxes, frame, fps=None):
    for x,y,w,h in boxes:
        cv2.rectangle(frame, (x,y), (x+w,y+h), (0,255,0), 3)
    info = "Objek: %d" % len(boxes)
    if fps is not None:
        info += " | FPS: %.1f" % fps
    cv2.putText(frame, info, (20,60), cv2.FONT_HERSHEY_SIMPLEX, 2.0, (0,255,255), 4, cv2.LINE_AA)

    return frame