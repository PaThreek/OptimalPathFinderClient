#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "Client.h"

static std::map<std::string, size_t> commandParametersCount{{"P", 5},
                                                            {"M", 1},
                                                            {"Q", 1}};

std::vector<std::string> parseCommand(std::string commandStr)
{
    static const std::string DELIMITER(" ");
    
    std::vector<std::string> command;
    size_t pos = 0;
    while ((pos = commandStr.find(DELIMITER)) != std::string::npos) {
        std::string token = commandStr.substr(0, pos);
        command.emplace_back(token);
        commandStr.erase(0, pos + DELIMITER.length());
    }

    if (!commandStr.empty()) {
        command.emplace_back(commandStr);
    }

    return command;
}
bool validateCommand(const std::vector<std::string>& command)
{
    if (command.empty()) {
        return false;
    }
    
    const auto& commandIt = commandParametersCount.find(command[0]);
    return  commandIt != commandParametersCount.end() && commandIt->second == command.size();
}

int convetStrToInt(std::string& strNum)
{
    return std::__cxx11::stoi(strNum);
}

int main(int argc, char const *argv[])
{
    
    std::unique_ptr<Client::Client> client = std::make_unique<Client::Client>();
    client->start();
    
    std:: cout << "Shortest Path Client usage:" << std::endl;
    std:: cout << "\"P x1 y1 x2 y2\": Request coordinates from Server for optimal path from point x1, y1 to point x2, y2." << std::endl;
    std:: cout << "\"Q\": Quit program" << std::endl;

    while(true) 
    {
        try {
            std::cout << "Enter command: ";
            std::string commandStr;
            std::getline(std::cin, commandStr);
            auto command = std::move(parseCommand(commandStr));
            
            if (!validateCommand(command)) {
                std::cout << "Invalid command." << std::endl;
                continue;
            }

            if (command[0] == "P") {
                int x1 = convetStrToInt(command[1]);
                int y1 = convetStrToInt(command[2]);
                int x2 = convetStrToInt(command[3]);
                int y2 = convetStrToInt(command[4]);

                client->requestOptimalPath(x1, y1, x2, y2);
            }
            else if (command[0] == "Q") {
                return 0;
            }
            else {
                std:: cout << "Unknown command!" << std::endl;
            }
            
        }
        catch (std::exception const & e) {
            std::cout << "An error occured during command processing. Message: " << e.what() << std::endl;
        }
        catch (...) {
            std::cout << "An error occured during command processing." << std::endl;
        }
    }

    return 0;
}