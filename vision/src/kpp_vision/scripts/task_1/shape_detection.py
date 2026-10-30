import cv2
import numpy as np

IDEAL = {"triangle": 0.413, "square": 0.637, "pentagon": 0.757, "hexagon": 0.827}
SIDES = {3: "triangle", 4: "square", 5: "pentagon", 6: "hexagon"}

def segment(frame, s_max=70):
    sat = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)[:, :, 1]
    mask = cv2.inRange(sat, 0, s_max)
    return cv2.morphologyEx(mask, cv2.MORPH_OPEN, np.ones((5, 5), np.uint8))

def classify(contour):
    area = cv2.contourArea(contour)
    (_, _), radius = cv2.minEnclosingCircle(contour)
    fillness = area / (np.pi * radius ** 2)
    if fillness > 0.9:
        return "circle"
    periphery = cv2.arcLength(contour, True)
    side = len(cv2.approxPolyDP(contour, 0.02 * periphery, True))
    if side in SIDES:
        return SIDES[side]
    return min(IDEAL, key=lambda k: abs(IDEAL[k] - fillness))

def detect(frame, min_area=2000):
    contours, _ = cv2.findContours(segment(frame), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    return [(classify(c), c) for c in contours if cv2.contourArea(c) >= min_area]

def draw(frame, results):
    for label, c in results:
        x,y,_,_ = cv2.boundingRect(c)
        cv2.drawContours(frame, [c], -1, (0,0,0), 3)
        cv2.putText(frame, label, (x, max(y-10, 25)), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,0,0), 3)
    return frame