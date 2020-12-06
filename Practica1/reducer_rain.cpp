#include <string>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>

#include "common.hpp"

std::unordered_map<int, float> sum_rain(std::istream& file){
	std::unordered_map<int, float> year_rain;
	
 	std::string line;
 	
 	//Read file line to line
	while(std::getline(file, line)){
	
		//Split the line in key and values
		std::vector<std::string> values;
		split(line, values, '\t');
		
		//extract key and value
		int year = std::stoi(values[0]);
		float rain = std::stof(values[1]);
		
		//Check if the key exists in the unordered_map
		std::unordered_map<int, float>::iterator it_year = year_rain.find(year);
		
		//If exists, sum the new value
		if(it_year != year_rain.end()){
			it_year->second += rain;
		}
		//If not exists, add the pair to the unordered_map
		else{
			year_rain[year] = rain;
		}
	}
	
	return year_rain;
}


int main(void){	
	//set locale to spanish. Necessary to read decimal separator correctly
	std::setlocale(LC_ALL, "es_ES.UTF-8");

	//read file from standard input
	std::istream* std_in = &std::cin;
	std::unordered_map<int, float> year_rain = sum_rain(*std_in);
	
	//write results to standard output
	for (auto& map: year_rain)
	{
		int year = map.first;
		float rain = map.second;
		
		std::cout<<year<<"\t"<<rain<<"\n";
	}
	
	return 0;
}
