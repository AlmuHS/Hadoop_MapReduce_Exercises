#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <locale>

#include "common.hpp"

using vector_data = std::vector<std::pair<std::string, float> >;


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
		std::string radiation_str = fix_empty_values(row[RADIATION]);
		
		float radiation = std::stof(radiation_str);
		
		std::cout<<city<<"\t"<<radiation<<"\n";
	}
}

int main(void){
	vector_data row_filtered;
	
	//set locale to spanish. Necessary to read decimal point correctly
	std::setlocale(LC_ALL, "es_ES.UTF-8");
	
	//read file from standard input
	std::istream* std_in = &std::cin;
	filter_radiation_csv(*std_in);
	
	return 0;
}
