import rospy
import cv2
from cv_bridge import CvBridge
from sensor_msgs.msg import Image

def main():
    rospy.init_node("camera_pub")
    source = rospy.get_param("~source", 0)
    fps = rospy.get_param("~fps", 30)
    loop = rospy.get_param("~loop", True)
    pub = rospy.Publisher("/camera/image_raw", Image, queue_size=1)
    bridge = CvBridge()

    cap = cv2.VideoCapture(source)
    if not cap.isOpened():
        rospy.logerr("Cannot open source")
        return

    rate = rospy.Rate(fps)
    while not rospy.is_shutdown():
        ok, frame = cap.read()
        if not ok:
            if loop and isinstance(source, str):
                cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
                continue
            rospy.logwarn("No more frame")
            break
        msg = bridge.cv2_to_imgmsg(frame, "bgr8")
        msg.header.stamp = rospy.Time.now()
        pub.publish(msg)
        rate.sleep()
    cap.release()

if __name__ == "__main__":
    main()