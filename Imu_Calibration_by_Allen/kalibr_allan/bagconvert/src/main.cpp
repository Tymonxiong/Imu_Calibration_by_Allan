#include <string>
#include <iostream>
#include <ros/ros.h>
#include <ros/topic.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/Imu.h>

#include <boost/filesystem.hpp>

// Matlab header, this is needed to save mat files
// Note that we use the FindMatlab.cmake to get this
// #include "mat.h"

using namespace std;

int main(int argc, char **argv) {

    // Debug message
    ROS_INFO("Starting up");

    // Check if there is a path to a dataset
    if(argc < 3) {
        ROS_ERROR("Error please specify a rosbag file");
        ROS_ERROR("Command Example: rosrun bagconvert bagconvert <rosbag> <topic>");
        return EXIT_FAILURE;
    }

    // Startup this node
    ros::init(argc, argv, "bagconvert");

    // Parse the input
    string pathBag = argv[1];
    string imuTopic = argv[2];

    // Get path
    boost::filesystem::path p(pathBag);
    string pathParent = p.parent_path().string();
    string pathTxt;
    if(!pathParent.empty()) {
        pathTxt = pathParent+"/"+p.stem().string()+".txt";
    } else {
        pathTxt = p.stem().string()+".txt";
    }


    // Load rosbag here, and find messages we can play
    rosbag::Bag bag;
    bag.open(pathBag, rosbag::bagmode::Read);


    // We should load the bag as a view
    // Here we go from beginning of the bag to the end of the bag
    rosbag::View view(bag);
    // Debug
    ROS_INFO("BAG Path is: %s", pathBag.c_str());
    ROS_INFO("Txt Path is: %s", pathTxt.c_str());
    ROS_INFO("Reading in rosbag file...");


    // Our data vector
    vector<double> dataIMU = vector<double>();

    // Step through the rosbag and send to algo methods
    for (const rosbag::MessageInstance& m : view) {

        // Handle IMU message
        sensor_msgs::Imu::ConstPtr s1 = m.instantiate<sensor_msgs::Imu>();
        if (s1 != NULL && m.getTopic() == imuTopic) {
            dataIMU.push_back(m.getTime().toSec());
            dataIMU.push_back(s1->linear_acceleration.x);
            dataIMU.push_back(s1->linear_acceleration.y);
            dataIMU.push_back(s1->linear_acceleration.z);
            dataIMU.push_back(s1->angular_velocity.x);
            dataIMU.push_back(s1->angular_velocity.y);
            dataIMU.push_back(s1->angular_velocity.z);
        }

    }

    // Debug message
    ROS_INFO("Done processing bag");    

    std::vector<std::vector<double>> pt1;
 
    std::vector<double> pTmp;
    for(size_t i=0; i<dataIMU.size(); i+=7) {
        pTmp.push_back(dataIMU.at(i));
        pTmp.push_back(dataIMU.at(i + 1));
        pTmp.push_back(dataIMU.at(i + 2));
        pTmp.push_back(dataIMU.at(i + 3));
        pTmp.push_back(dataIMU.at(i + 4));
        pTmp.push_back(dataIMU.at(i + 5));
        pTmp.push_back(dataIMU.at(i + 6));
        pt1.push_back(pTmp);
        pTmp.clear();        
    }
    //将bag文件转换为.txt文件
    FILE* pTxt = fopen(pathTxt.c_str(),"w");
    fprintf(pTxt,"\n");
    for(auto p: pt1) {
        fprintf(pTxt,"%lf   %lf   %lf   %lf   %lf   %lf    %lf",p[0],p[1],p[2],p[3],p[4],p[5],p[6]);
        fprintf(pTxt,"\n");
    }
    fclose(pTxt);

    ROS_INFO("Done processing IMU data");
   
    return EXIT_SUCCESS;
}


