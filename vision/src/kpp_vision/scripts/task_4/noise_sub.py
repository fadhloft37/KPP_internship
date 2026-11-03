#!/usr/bin/env python3
import cv2
import numpy as np
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
from noise_remover import remove_noise

bridge = CvBridge()

def callback(msg):
    frame = bridge.imgmsg_to_cv2(msg, "bgr8")
    noisy, clean = remove_noise(frame)
    cv2.imshow("noisy | clean", np.hstack([noisy, clean]))
    cv2.waitKey(1)

def main():
    rospy.init_node("noise_sub")
    rospy.Subscriber("/camera/image_raw", Image, callback, queue_size=1)
    rospy.spin()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()    