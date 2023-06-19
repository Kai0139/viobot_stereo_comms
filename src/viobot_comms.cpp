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
        std::string viobotIP = "192.168.1.23";
        std::string viobotPort = "8000";
        // you can pass http::InternetProtocol::V6 to Request to make an IPv6 request
        http::Request configSmartRequest{"http://" + viobotIP + ":" + viobotPort + "/Config/smart"};
        const std::string configSmartBody = "{\"gray_image_enable\": 1, "
                                            "\"imu_enable\": 1, "
                                            "\"tof_enable\": 1, "
                                            "\"tof_deep_image_enable\": 1, "
                                            "\"tof_amp_image_enable\": 1}";

        const std::string configNetworkBody = "{\"ipaddr\": \"192.168.1.23\", "
                                              "\"submask\": \"192.168.0.1.0\", "
                                              "\"gateway\": \"192.168.1.1\", "
                                              "\"macaddr\": \"FF:FF:FF:FF:FF:FF\", "
                                              "\"commandPort\":8000, "
                                              "\"heartbeatPort\":6789, "
                                              "\"udpPort\":10000}";

        http::Request algorithmEnableRequest{"http://" + viobotIP + ":" + viobotPort + "/Smart/algorithmEnable"};
        const std::string emptyBody = "{}";
        
        http::Request algorithmRebootRequest{"http://" + viobotIP + ":" + viobotPort + "/Smart/algorithmReboot"};
        http::Request algorithmResetRequest{"http://" + viobotIP + ":" + viobotPort + "/Smart/algorithmReset"};

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
            // request to enable all sensors
            auto r1 = configSmartRequest.send("PUT", configSmartBody,{
                {"Content-Type", "application/json"}
            });
            printResponse(r1);

            // sleep for 100ms
            usleep(100000);

            // request to enable vio algorithm
            auto r2 = algorithmEnableRequest.send("PUT", emptyBody,{
                {"Content-Type", "application/json"}
            });
            printResponse(r2);
        }
        else if(!strcmp(argv[1], "network"))
        {
            // request to enable all sensors
            auto r1 = configSmartRequest.send("PUT", configNetworkBody,{
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