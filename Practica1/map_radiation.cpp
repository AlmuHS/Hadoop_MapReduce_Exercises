#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <locale>
#include <algorithm>

#include "common.hpp"

using vector_pair = std::vector<std::pair<std::string, float> >;

void filter_radiation_csv(std::istream& file){	
 	std::string line;
 	
	//Skip header
 	std::getline(file, line);
 	
 	//Read file line to line
	while(std::getline(file, line)){
		std::vector<std::string> row;
		
		//split the line by columns
		split(line, row, ';');
		
		//Filter the data about radiation and rain of this city
		std::string city = row[CITY];
		std::string radiation_str = row[RADIATION];
		std::string id_str = row[ID];
		int id;
		
		//Remove "" from string
		id_str.erase(std::remove(id_str.begin(), id_str.end(), '\"'), id_str.end());	
		
		//convert ID to int
		id = std::stoi(id_str);
		
		//Ignore the empty values and filter the required cities
		if(radiation_str != "" && (id >= 2 && id <= 10)){
			std::cout<<city<<"\t"<<radiation_str<<"\n";
		}
		
	}
}

int main(void){
	//set locale to spanish. Necessary to read decimal point correctly
	std::setlocale(LC_ALL, "es_ES.UTF-8");
	
	//read file from standard input
	std::istream* std_in = &std::cin;
	filter_radiation_csv(*std_in);
	
	return 0;
}
