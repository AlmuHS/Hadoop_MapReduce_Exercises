#include <string>
#include <vector>
#include <utility>

const int RADIATION = 16;
const int RAIN = 17;
const int CITY = 3;
const int DATE = 4;


void split(const std::string& str, std::vector<std::string>& cont, char delim = ' ')
{
    size_t start = 0;
    size_t end = str.find(delim);

    while (end != std::string::npos)
    {
        cont.push_back(str.substr(start, end-start));
        start = end + 1;
        end = str.find(delim, start);
    }
    cont.push_back(str.substr(start, end - start));
}

std::pair<std::string, float> get_key_value(std::string line){
	size_t found = line.find_first_of("\t");
	std::pair<std::string, float> key_value;
	
	if(found != std::string::npos)
	{
		std::string key = line.substr(0,found);
		std::string value = line.substr(found);
		float valuei = std::stof(value.c_str());
		
		key_value.first = key;
		key_value.second = std::stof(value);
	}
	
	return key_value;
}

std::string fix_empty_values(std::string value){
	if(value == "" || value[0] == '\n' || value[0] == '\r'){
		value = "0";
	}
	
	return value;
}
