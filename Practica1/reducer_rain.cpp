#include <string>
#include <utility>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>

#include "common.hpp"

std::map<std::string, float> sum_rain(std::istream& file){
	std::map<std::string, float> year_rain;
	
 	std::string line;
 	
 	//Read file line to line
	while(std::getline(file, line)){
	
		//Split the line in key and values
		std::pair<std::string, float> values = get_key_value(line);
		
		//extract key and value
		std::string year = values.first;
		float rain = values.second;
		
		//Check if the key exists in the unordered_map
		std::map<std::string, float>::iterator it_year = year_rain.find(year);
		
		//If not exists, add the pair to the unordered_map
		if(it_year == year_rain.end()){
			year_rain[year] = rain;
			
		}
		//If exists, sum the new value
		else{
			it_year->second += rain;
		}
	}
	
	return year_rain;
}


int main(void){	
	//set locale to spanish. Necessary to read decimal separator correctly
	std::setlocale(LC_ALL, "es_ES.UTF-8");

	//read file from standard input
	std::istream* std_in = &std::cin;
	std::map<std::string, float> year_rain = sum_rain(*std_in);
	
	//write results to standard output
	for (auto& map: year_rain)
	{
		std::string year = map.first;
		float rain = map.second;
		
		std::cout<<year<<"\t"<<rain<<"\n";
	}
	
	return 0;
}
