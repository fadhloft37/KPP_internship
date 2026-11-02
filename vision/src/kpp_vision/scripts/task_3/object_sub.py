#!/usr/bin/env python3
import time
import cv2
import rospy
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
from object_detection import locate_object, draw

bridge = CvBridge()
last_time = time.time()
fps = 0.0

def callback(msg):
    global last_time, fps

    # ROS Image message to OpenCV image
    frame = bridge.imgmsg_to_cv2(msg, "bgr8")

    # Locate objects
    boxes = locate_object(frame)

    # FPS calculation
    now = time.time()
    inst = 1.0 / max(now - last_time, 1e-6)
    fps = inst if fps == 0.0 else 0.9 * fps + 0.1 *inst
    last_time = now

    output = draw(boxes, frame, fps)
    cv2.imshow("Task 3 - Object location", output)
    cv2.waitKey(1)

def main():
    rospy.init_node("object_sub")
    rospy.Subscriber("/camera/image_raw", Image, callback, queue_size=1, buff_size=2**26)
    rospy.spin()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()