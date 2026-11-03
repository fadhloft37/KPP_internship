#!/usr/bin/env python3
import os
import time
import cv2
import rospy
import rospkg
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
from character_detection import HK, draw

class char_sub:
    def __init__(self):
        pkg_path = rospkg.RosPack().get_path("kpp_vision")
        default_weights = os.path.join(pkg_path, "models", "task_5", "best.pt")

        weights = rospy.get_param("~weights", default_weights)
        conf = rospy.get_param("~conf", 0.5)
        imgsz = rospy.get_param("~imgsz", 640)

        self.bridge = CvBridge()
        self.detector = HK(weights, conf=conf, imgsz=imgsz)
        self.prev_time = time.time()
        self.window = "Task 5 - Does it match?"

        cv2.namedWindow(self.window, cv2.WINDOW_NORMAL)

        rospy.Subscriber("/camera/image_raw", Image, self.callback, queue_size=1, buff_size=2**24)

    def callback(self, msg):
        frame = self.bridge.imgmsg_to_cv2(msg, "bgr8")

        detections = self.detector.detect(frame)
        frame = draw(frame, detections)

        now = time.time()
        fps = 1.0 / max(now - self.prev_time, 1e-6)
        self.prev_time = now
        cv2.putText(frame, "FPS: {:.2f}".format(fps), (20, 45), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (255, 255, 0), 3)
        cv2.imshow(self.window, frame)
        cv2.waitKey(1)

if __name__ == "__main__":
    rospy.init_node("char_sub")
    char_sub()
    rospy.spin()
    cv2.destroyAllWindows()