#include "../includes/chat.hpp"

AiAgent::AiAgent()
{
    // envFile.open(".env");
    url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent";
    junkFile = "/tmp/response";
    // getline(envFile, api_key);
}

AiAgent::~AiAgent()
{
    envFile.close();
}

void AiAgent::setApi(std::string api)
{
    api_key = api;
}
std::string AiAgent::parseResponse(void)
{
    std::string buffer;
    int flag = 0;
    std::ifstream json(junkFile.c_str());
    if (!json)
        throw std::runtime_error("There is no such file\n");
    while (getline(json, buffer))
    {
        if (buffer.find("\"text\":") != std::string::npos)
        {
            flag = 1;
            break ;
        }
    }
    if (!flag)
    {
        json.close();
        std::remove(junkFile.c_str());
        throw std::runtime_error("Something is wrong with you api\n");
    }
    json.close();
    std::remove(junkFile.c_str());
    return (buffer.substr(21, (buffer.size() - 22)));
}

std::string AiAgent::startAgent(std::string prompt)
{
    std::string response;
    std::string payload = "{\"contents\":[{\"parts\":[{\"text\":\"" + prompt + "\"}]}]}";
    std::string exec = "curl -s -X POST \"" + url + "?key=" + api_key + "\" -H \"Content-Type: application/json\" -d '" + payload + "'" + " > " + junkFile;
    system(exec.c_str());
    try {
        response = parseResponse();
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }
    return (response);
}




// int main()
// {
//     std::string api_key = "AIzaSyCCXUW015gm08ac2YuWxu-SXjCC980u1t4";
//     std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent";
//     std::string msg = "explain linear regression";
//     std::string json = "{\"contents\":[{\"parts\":[{\"text\":\"" + msg + "\"}]}]}";

//     // 2. Build curl command
//     std::string cmd = "curl -s -X POST \"" + url + "?key=" + api_key + "\" -H \"Content-Type: application/json\" -d '" + json + "'" + ">> reponse";

//     // 3. Execute command
//     system(cmd.c_str());

// }