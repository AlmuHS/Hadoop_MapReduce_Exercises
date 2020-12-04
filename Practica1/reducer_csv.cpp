#include <string>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <algorithm>

std::pair<std::string, float> get_key_value(std::string line){
	size_t found = line.find_first_of("\t");
	std::pair<std::string, float> key_value;
	
	if(found != std::string::npos)
	{
		std::string key = line.substr(0,found);
		std::string value = line.substr(found);
		
		key_value.first = key;
		key_value.second = std::stof(value);
	}
	
	return key_value;
}


std::unordered_map<std::string, float> calculate_avg(std::istream& file){
	std::unordered_map<std::string, float> avg_map;
	std::unordered_map<std::string, int> ocurrences_map;
	
 	std::string line;
 	
 	//Read file line to line
	while(std::getline(file, line)){
	
		//Split the line in key and value
		std::pair<std::string, float> key_value = get_key_value(line);
		
		//extract key and value
		std::string key = key_value.first;
		float value = key_value.second;
		
		//Check if the key exists in the unordered_map
		std::unordered_map<std::string, float>::iterator it_key = avg_map.find(key);
		
		//If exists, sum the new value
		if(it_key != avg_map.end()){
		
			//Increase the number of ocurrences of this key
			ocurrences_map[key]++;
			
			//Sum the new value to this key
			it_key->second += value;
		}
		//If not exists, add the pair to the unordered_map
		else{
			avg_map.insert(key_value);
		}
	}
	
	//Calculate the average of each key
	for(std::unordered_map<std::string,float>::iterator it = avg_map.begin(); it != avg_map.end(); ++it) {
	
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
	std::unordered_map<std::string, float> avg_map;
	
	//read file from standard input
	std::istream* std_in = &std::cin;
	avg_map = calculate_avg(*std_in);
	
	//write results to standard output
	for(std::unordered_map<std::string,float>::iterator it = avg_map.begin(); it != avg_map.end(); ++it)
	{
		std::string key = it->first;
		float value = it->second;
	
		//remove "" from key name
		key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
		
		//show the result
		std::cout<<key<<"\t"<<value<<"\n";
	}
	
	return 0;
}
