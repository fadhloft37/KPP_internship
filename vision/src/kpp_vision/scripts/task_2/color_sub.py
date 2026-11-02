#!/usr/bin/env python3
import time
import cv2
import rospy
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
from color_detection import ColorTracker, analyze_frame, build_display, COLOR_NAMES

bridge = CvBridge()
tracker = ColorTracker()

def callback(msg):
    t0 = time.perf_counter()
 
    frame = bridge.imgmsg_to_cv2(msg, "bgr8")
    dominant, fractions, mask, segmented = analyze_frame(frame, tracker)
 
    ms = (time.perf_counter() - t0) * 1000.0
    cv2.imshow("Task 2 - How Colorful?", build_display(frame, mask, segmented, dominant, fractions, tracker, ms))
    cv2.waitKey(1)

def report():
    names = [COLOR_NAMES[i] for i in sorted(tracker.seen)]
    rospy.loginfo("Unique colors detected (%d): %s", len(names), ", ".join(names))
    cv2.destroyAllWindows()

def main():
    rospy.init_node("task2_how_colorful")
    rospy.Subscriber("/camera/image_raw", Image, callback, queue_size=1, buff_size=2 ** 24)
    rospy.on_shutdown(report)
    rospy.spin()
 
 
if __name__ == "__main__":
    main()