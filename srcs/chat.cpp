#include "../includes/chat.hpp"

AiAgent::AiAgent()
{
    // envFile.open(".env");
    url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent";
    junkFile = "response";
    // getline(envFile, api_key);
}

AiAgent::~AiAgent()
{
    envFile.close();
}

void AiAgent::setApi(std::string api)
{
    this->api_key = api;
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
    // i need to escape ; "" '' to prevent cmd injection and i have to try this
    std::string response;
    std::string instruction = "you are a helpful assistant named sbiksla in a irc server you keep you answers short as you can keep it funny a bit you can add some emogies to your answers : ";
    std::string payload = "{\"contents\":[{\"parts\":[{\"text\":\"" + instruction + prompt + "\"}]}]}";
    std::string exec = "curl -s -X POST \"" + url + "?key=" + api_key + "\" -H \"Content-Type: application/json\" -d '" +  payload + "'" + " > " + junkFile;
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




