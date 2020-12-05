#include <string>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ')
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

std::pair<std::string, float> calculate_max_radiation(std::istream& file){
	std::pair<std::string, float> max_rad_pair;
	
 	std::string line;
 	
 	float max_rad = 0;
 	std::string city_max_rad;
 	
 	//Read file line to line
	while(std::getline(file, line)){
	
		//Split the line in key and values
		std::vector<std::string> values;
		split(line, values, '\t');
		
		//extract key and value
		std::string city = values[0];
		float radiation = std::stof(values[1]);
		
		//Update max radiation
		if(radiation > max_rad){
			max_rad = radiation;
			city_max_rad = city;
		}
		
		
	}
	
	//Return a pair with the city of max radiation and its value
	max_rad_pair = std::make_pair(city_max_rad, max_rad);
	
	return max_rad_pair;
}


int main(void){	
	//read file from standard input
	std::istream* std_in = &std::cin;
	std::pair<std::string, float> max_rad_pair = calculate_max_radiation(*std_in);
	
	//write results to standard output
	std::string city = max_rad_pair.first;
	float radiation = max_rad_pair.second;
	std::cout<<city<<"\t"<<radiation<<"\n";
	
	return 0;
}
