#include <string>
#include <utility>
#include <iostream>
#include <unordered_map>

#include "common.hpp"

void calculate_avg(std::istream& file){
	std::unordered_map<std::string, int> ocurrences_map;
	std::unordered_map<std::string, float> sum_radiation_map;
	
 	std::string line;
 	
 	//Read file line to line
	while(std::getline(file, line)){
	
		//Split the line in key and values
		std::pair<std::string, float> city_radiation = get_key_value(line);
		
		//extract key and value
		std::string city = city_radiation.first;
		float radiation = city_radiation.second;
		
		//Check if the key exists in the unordered_map
		std::unordered_map<std::string, float>::iterator it_key = sum_radiation_map.find(city);
		
		//If exists, sum the new value
		if(it_key != sum_radiation_map.end()){
		
			//Increase the number of ocurrences of this key
			ocurrences_map[city]++;
			
			//Sum the new values to this key
			it_key->second += radiation;
		}
		//If not exists, add the pair to the unordered_map
		else{
			sum_radiation_map.insert(city_radiation);
			
			//Increase the number of ocurrences of this key
			ocurrences_map[city] = 1;
		}
	}
	
	//Calculate the average of each key
	for(std::unordered_map<std::string, float>::iterator it = sum_radiation_map.begin(); it != sum_radiation_map.end(); ++it) {
	
		//Get key and value
		std::string city = it->first;
		float sum_radiation = it->second;
		
		//Find the number of ocurrences of this key in the file
		int ocurrences = ocurrences_map[city];
		
		//Calculate the average
		float avg_radiation = sum_radiation/ocurrences;
		std::cout<<city<<"\t"<<avg_radiation<<"\n";
	}
}


int main(void){	
	//set locale to spanish. Necessary to read decimal point correctly
	std::setlocale(LC_ALL, "es_ES.UTF-8");

	//read file from standard input
	std::istream* std_in = &std::cin;
	calculate_avg(*std_in);
	
	return 0;
}
