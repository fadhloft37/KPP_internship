#include <iostream>
#include <string>
#include <sstream>

#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char** argv){
    // ROS node initialization
    ros::init(argc, argv, "user_node");
    ros::NodeHandle nh;

    // ROS publishers for each topics
    ros::Publisher mode_pub = nh.advertise<std_msgs::String>("mode_topic", 10);
    ros::Publisher preset_pub = nh.advertise<std_msgs::String>("preset_topic", 10);
    ros::Publisher key_pub = nh.advertise<std_msgs::String>("key_topic", 10);
    ros::Publisher custom_pub = nh.advertise<std_msgs::String>("custom_topic", 10);

    // Main loop
    while(ros::ok()){
        std::string input;
        std::cout << "Masukkan key (tekan 'H' untuk panduan): ";
        std::getline(std::cin, input);

        std_msgs::String msg;
        msg.data = input;

        if(input == "M" || input == "m"){   // Mode_topic
            mode_pub.publish(msg);
        } else if(input == " "){    // reset
            key_pub.publish(msg);
        } else if(input == "H" || input == "h"){
            std::cout << "Panduan:\n";
            std::cout << "  M: Ganti mode (manual/preset)\n";
            std::cout << "  Space: Reset posisi robot\n";
            std::cout << "  Manual mode keys:\n";
            std::cout << "    D/A: Gerakkan joint 1\n";
            std::cout << "    W/S: Gerakkan joint 2\n";
            std::cout << "    F/J: Gerakkan joint 3\n";
            std::cout << "    L/K: Gerakkan joint 4\n";
            std::cout << "    P/O: Gerakkan joint 5\n";
            std::cout << "    Q/E: Gerakkan gripper\n";
            std::cout << "  Preset mode keys:\n";
            std::cout << "    1: Pindah ke posisi 'home'\n";
            std::cout << "    2: Pindah ke posisi 'ready'\n";
            std::cout << "    3: Pose segitiga 'triangle'\n";
            std::cout << "    4: Custom IK (masukkan target XYZ)\n";
        }else if(input == "1" || input == "2" || input == "3" || input == "4"){ // Preset_topic
            msg.data = (input == "1") ? "home" : (input == "2") ? "ready" : (input == "3") ? "triangle" : "custom";
            preset_pub.publish(msg);

            if(input == "4"){
                double x,y,z;
                std::cout << "Masukkan posisi target (x y z): ";

                std::string inputTarget;
                std::getline(std::cin, inputTarget);
                std::istringstream streamTarget(inputTarget);

                if(!(streamTarget >> x >> y >> z)){
                    std::cout << "Format target salah. Gunakan format: X Y Z";
                    continue;
                }

                std_msgs::String msgTarget;
                std::ostringstream output;
                output << x << " " << y << " " << z;

                msgTarget.data = output.str();
                custom_pub.publish(msgTarget);
            }
        } else{     // Key_topic
            key_pub.publish(msg);
        }
    }
}