#include "Config.h"


void Config::Import()
{
    configData = std::unordered_map<std::string, float>();

    std::ifstream config(L"config.txt");
    std::string line;
    //Splits the data at the :
    std::regex search(R"rgx(\:(.*?)\,)rgx");
    if (config.is_open()) {

        while (getline(config, line))
        {
            //validate that the line ends with a ,
            std::regex validate(R"rgx(.*,$)rgx");
            //validate that the line only has 1 :
            std::regex validate2(R"rgx(^[^:]*:[^:]*$)rgx");

            if (std::regex_match(line, validate) && std::regex_match(line, validate2)) {
                //simple try catch so we dont break everything
                try {

                    std::smatch m;
                    //split the string into smatch m
                    std::regex_search(line, m, search);

                    std::string prefix = m.prefix();
                    
                    int pos = m.size() - 1;
                    //get the last value in m
                    float val = std::stof(m[pos]);

                    //check if key already exists
                    if (configData.find(prefix) == configData.end()) {
                        //if not add it to the array
                        std::pair<std::string, float> data(prefix, val);
                        configData.insert(data);
                    }
                }
                catch (const std::exception& e) {

                }
            }

        }
        config.close();

    }
    

}

bool Config::GetValue(const char* key, float* value)
{

    std::string searchKey(key);
    //retrun false if the key is not in the table
    if (configData.find(searchKey) == configData.end()) {
        return false;
    }
    
    *value = configData[searchKey];

    return true;

}

