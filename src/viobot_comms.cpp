#include <iostream>
#include <unistd.h>
#include <string.h>
#include "HTTPRequest.hpp"


int printResponse(http::Response r)
{
    std::cout << std::string{r.body.begin(), r.body.end()} << '\n'; // print the result
    return 0;
}

int main(int argc, char **argv)
{
    bool reboot = false;
    try
    {
        std::string viobotIP = "192.168.1.100";
        std::string viobotPort = "8000";
        std::string algo_type_num = "2"; // 1. stereo1, 2. stereo2, 3. mono

        // you can pass http::InternetProtocol::V6 to Request to make an IPv6 request
        http::Request configSmartRequest{"http://" + viobotIP + ":" + viobotPort + "/Config/smart"};
        // gray_image_enable: 0：不启用流获取灰度图, 1：启用流获取左目灰度图（单目, 2：启用流获取右目灰度图, 3：启用流获取双目灰度图
        const std::string configSmartBody = "{\"gray_image_enable\": 0, "
                                            "\"imu_enable\": 0, "
                                            "\"tof_enable\": 0, "
                                            "\"tof_deep_image_enable\": 0, "
                                            "\"tof_amp_image_enable\": 0}";

        const std::string configNetworkBody = "{\"ipaddr\": \"192.168.1.100\", "
                                              "\"submask\": \"192.168.0.1.0\", "
                                              "\"gateway\": \"192.168.1.1\", "
                                              "\"macaddr\": \"FF:FF:FF:FF:FF:FF\", "
                                              "\"commandPort\":8000, "
                                              "\"heartbeatPort\":6789, "
                                              "\"udpPort\":10000}";

        const std::string emptyBody = "{}";

        http::Request algorithmEnableRequest{"http://" + viobotIP + ":" + viobotPort + "/Algorithm/enable/"+algo_type_num};
        http::Request algorithmRebootRequest{"http://" + viobotIP + ":" + viobotPort + "/Algorithm/reboot/"+algo_type_num};
        http::Request algorithmResetRequest{"http://" + viobotIP + ":" + viobotPort + "/Algorithm/reset/"+algo_type_num};
        http::Request getCamParamRequest{"http://" + viobotIP + ":" + viobotPort + "/Config/lens?Camera=1"};

        if(argc < 2)
        {
            std::cout << "Commands: init / reset / reboot / network" << std::endl;
            return -1;
        }

        if(!strcmp(argv[1], "reboot"))
        {
            std::cout << "reboot algorithm" << std::endl;
            auto r3 = algorithmRebootRequest.send("PUT", emptyBody,{
                {"Content-Type", "application/json"}
            });
            printResponse(r3);
        }
        else if(!strcmp(argv[1], "reset"))
        {
            std::cout << "reset algorithm" << std::endl;
            auto r3 = algorithmResetRequest.send("PUT", emptyBody,{
                {"Content-Type", "application/json"}
            });
            printResponse(r3);
        }
        else if(!strcmp(argv[1], "init"))
        {
            // request to enable vio algorithm
            auto r2 = algorithmEnableRequest.send("PUT", emptyBody,{
                {"Content-Type", "application/json"}
            });
            printResponse(r2);
        }
        else if(!strcmp(argv[1], "network"))
        {

            auto r1 = configSmartRequest.send("PUT", configNetworkBody,{
                {"Content-Type", "application/json"}
            });
            printResponse(r1);
        }
        else if(!strcmp(argv[1], "camparam"))
        {
            // request Camera=1:左目可见光, Camera=2:右目可见光, Camera=3:TOF
            http::Request getCamParamRequest{"http://" + viobotIP + ":" + viobotPort + "/Config/lens?Camera=" + argv[2]};
            auto r1 = getCamParamRequest.send("GET", emptyBody,{
                {"Content-Type", "application/json"}
            });
            printResponse(r1);
        }
        else if(!strcmp(argv[1], "imuinter"))
        {
            // request
            http::Request getImuParamRequest{"http://" + viobotIP + ":" + viobotPort + "/Config/imuInter"};
            auto r1 = getImuParamRequest.send("GET", emptyBody,{
                {"Content-Type", "application/json"}
            });
            printResponse(r1);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }
    return 0;
}