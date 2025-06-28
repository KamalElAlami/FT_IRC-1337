#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>


class AiAgent
{
    private :
        std::string api_key;
        std::string url;
        std::string junkFile;
        std::ifstream envFile;
    public :
        AiAgent();
        ~AiAgent();
        std::string startAgent(std::string prompt);
        std::string parseResponse(void);
        void setApi(std::string api);

};
