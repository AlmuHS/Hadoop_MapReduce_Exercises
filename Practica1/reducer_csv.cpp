#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <iterator>
#include <locale>
#include <map>


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


std::map<std::string, float> calculate_avg(std::istream& file){
	std::map<std::string, float> avg_map;
	std::map<std::string, int> ocurrences_map;
	
 	std::string line;
 	
 	//Read file line to line
	while(std::getline(file, line)){
	
		//Split the line in key and value
		std::pair<std::string, float> key_value = get_key_value(line);
		
		//extract key and value
		std::string key = key_value.first;
		float value = key_value.second;
		
		//Check if the key exists in the map
		int exists = avg_map.count(key);
		
		//If exists, sum the new value
		if(exists){
			ocurrences_map[key]++;
			int num_ocurrences = ocurrences_map[key];
			avg_map[key] += value;
		}
		//If not exists, add the pair to the map
		else{
			avg_map.insert(key_value);
		}
	}
	
	//Calculate the average of each key
	for(std::map<std::string,float>::iterator it = avg_map.begin(); it != avg_map.end(); ++it) {
	
		//Get key and value
		std::string key = it->first;
		float value = it->second;
		
		//Find the number of ocurrences of this key in the file
		int ocurrences = ocurrences_map[key];
		
		//Calculate the average
		it->second = value/ocurrences;
	}
	
	return avg_map;
}


int main(void){
	std::map<std::string, float> avg_map;
	
	//read file from standard input
	std::istream* std_in = &std::cin;
	avg_map = calculate_avg(*std_in);
	
	//write results to standard output
	for(std::map<std::string,float>::iterator it = avg_map.begin(); it != avg_map.end(); ++it)
	{
		std::cout<<it->first<<"\t"<<it->second<<"\n";
	}
	
	return 0;
}
