#include <string>
#include <utility>
#include <iostream>

#include "common.hpp"

std::pair<std::string, float> calculate_max_radiation(std::istream& file){
	std::pair<std::string, float> max_rad_pair;
	
 	std::string line;
 	
 	float max_rad = 0;
 	std::pair<std::string, float> city_max_rad;
 	std::pair<std::string, float> city_radiation;
 	
 	//Read file line to line
	while(std::getline(file, line)){
	
		//Split the line in key and values
		city_radiation = get_key_value(line);
		
		//extract key and value
		float radiation = city_radiation.second;
		
		//Update max radiation
		if(radiation > max_rad){
			max_rad = radiation;
			city_max_rad = city_radiation;
		}
		
	}
	
	//Return a pair with the city of max radiation and its value
	max_rad_pair = city_max_rad;
	
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
