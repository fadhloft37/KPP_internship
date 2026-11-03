#!/usr/bin/env python3
import cv2
import numpy as np

KERNEL = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))

def remove_noise(frame):
    # 1. GRAYSCALE
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # 2. MASKING
    _, mask = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)

    # 3. OPENING(CLEANING)
    clean = cv2.morphologyEx(mask, cv2.MORPH_OPEN, KERNEL)

    return mask, clean
