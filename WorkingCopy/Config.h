#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <regex> 
#include <unordered_map>




class Config
{
private:

    std::unordered_map<std::string, float> configData;

public:
    void Import();
    bool GetValue(const char* key, float* value);

};

