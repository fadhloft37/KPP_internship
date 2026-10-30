import rospy
import cv2
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image
from shape_detection import detect, draw

bridge = CvBridge()

def process(frame):
    results = detect(frame)
    return draw(frame, results)

def callback(msg):
    try:
        frame = bridge.imgmsg_to_cv2(msg, "bgr8")
    except CvBridgeError as e:
        rospy.logerr(e)
        return
    output_frame = process(frame)
    cv2.imshow("Shape Detection", cv2.resize(output_frame, (960, 540)))
    if cv2.waitKey(1) == 27:
        rospy.signal_shutdown("ESC pressed")

def main():
    rospy.init_node("shape_sub")
    rospy.Subscriber("/camera/image_raw", Image, callback, queue_size=1, buff_size=2**24)
    rospy.spin()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()