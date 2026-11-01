#!/usr/bin/env python3
import cv2
import numpy as np

# ---------------------------------------------------------------- settings
# Bin numbers: 0..11 = hues, 12 = black, 13 = gray, 14 = white
BLACK = 12
GRAY = 13
WHITE = 14
N_BINS = 15

COLOR_NAMES = ["red", "orange", "yellow", "lime", "green", "mint",
               "cyan/teal", "sky blue", "blue", "purple", "magenta", "pink",
               "black", "gray", "white"]

# Color (B, G, R) used to paint each bin in the palette bar
SWATCHES = [(0, 0, 255), (0, 128, 255), (0, 255, 255), (0, 255, 128),
            (0, 255, 0), (128, 255, 0), (255, 255, 0), (255, 128, 0),
            (255, 0, 0), (255, 0, 128), (255, 0, 255), (128, 0, 255),
            (0, 0, 0), (128, 128, 128), (255, 255, 255)]
 
LOW_SATURATION = 35    # below this the pixel has no real hue (gray-ish)
LOW_BRIGHTNESS = 50    # below this the pixel is black
HIGH_BRIGHTNESS = 200  # gray-ish AND brighter than this = white

# ---------------------------------------------------------------- 1. quantize
def quantize_hsv(bgr):
    hsv = cv2.cvtColor(bgr, cv2.COLOR_BGR2HSV)
    hue, sat, val = cv2.split(hsv)
 
    shifted_hue = (hue + 7) % 180
    labels = shifted_hue // 15
 
    gray_ish = sat < LOW_SATURATION
    labels[gray_ish & (val >= LOW_BRIGHTNESS)] = GRAY
    labels[gray_ish & (val >= HIGH_BRIGHTNESS)] = WHITE
    labels[val < LOW_BRIGHTNESS] = BLACK
    return labels

# ---------------------------------------------------------------- 2. count
def count_pixels(labels):
    flat = labels.ravel()
    counts = np.bincount(flat, minlength=N_BINS)
    return counts

# ---------------------------------------------------------------- 3. tracker
class ColorTracker:
    def __init__(self):
        self.current = None      # dominant color we are currently showing
        self.seen = set()        # every color seen so far in the video
 
    def update(self, counts):
        fractions = counts / counts.sum()
 
        for i in range(N_BINS):
            if fractions[i] >= 0.001:
                self.seen.add(i)
 
        best = int(np.argmax(fractions))
        if self.current is None:
            self.current = best
        elif fractions[best] > fractions[self.current] + 0.02:
            self.current = best
 
        return self.current, fractions

# ---------------------------------------------------------------- 4. segment
def segment_color(labels, color_id):
    is_color = labels == color_id
    mask = is_color.astype(np.uint8) * 255
    return mask

# ---------------------------------------------------------------- 5. pipeline
def analyze_frame(frame, tracker, scale=0.5):
    small = cv2.resize(frame, None, fx=scale, fy=scale, interpolation=cv2.INTER_AREA)
 
    labels = quantize_hsv(small)
    counts = count_pixels(labels)
    dominant, fractions = tracker.update(counts)
 
    mask_small = segment_color(labels, dominant)
    height, width = frame.shape[:2]
    mask = cv2.resize(mask_small, (width, height), interpolation=cv2.INTER_NEAREST)
    segmented = cv2.copyTo(frame, mask)
    return dominant, fractions, mask, segmented

# ---------------------------------------------------------------- 6. display
def put_label(img, text, position):
    cv2.putText(img, text, position, cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 0), 4)
    cv2.putText(img, text, position, cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 1)
 
 
def build_display(frame, mask, segmented, dominant, fractions, tracker, ms):
    size = (426, 240)
    original = cv2.resize(frame, size)
    mask_panel = cv2.cvtColor(cv2.resize(mask, size), cv2.COLOR_GRAY2BGR)
    segmented_panel = cv2.resize(segmented, size)
 
    put_label(original, "unique colors: %d" % len(tracker.seen), (8, 22))
    put_label(original, "%.1f ms" % ms, (8, 46))
    name = COLOR_NAMES[dominant]
    percent = fractions[dominant] * 100
    put_label(segmented_panel, "%s  %.1f%%" % (name, percent), (8, 22))
 
    row = np.hstack([original, mask_panel, segmented_panel])
 
    palette = np.zeros((40, row.shape[1], 3), np.uint8)
    slot = row.shape[1] // N_BINS
    for i in tracker.seen:
        left = i * slot
        right = left + slot - 2
        cv2.rectangle(palette, (left, 0), (right, 39), SWATCHES[i], -1)
        cv2.rectangle(palette, (left, 0), (right, 39), (90, 90, 90), 1)
 
    return np.vstack([row, palette])
