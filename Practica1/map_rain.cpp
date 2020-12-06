#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <locale>

#include "common.hpp"

using vector_data = std::vector<std::pair<std::string, float> >;


void filter_rain_csv(std::istream& file, std::string city_max_rad){	
 	std::string line;
 	
	//Skip header
 	std::getline(file, line);
 	
 	//Read file line to line
	while(std::getline(file, line)){
		std::vector<std::string> row;
		
		//split the line by columns
		split(line, row, ';');
		
		//Filter the rain data of the city which higher radiation (passed by parameter)
		std::string city = row[CITY];
			
		if(city == city_max_rad){
			//Replace empty values with zero
			std::string rain_str = fix_empty_values(row[RAIN]);
			
			//Extract year from date
			std::string date = row[DATE];
    			std::string year = date.substr(date.size()-4);
			
			//Show values by screen
			std::cout<<year<<"\t"<<rain_str<<"\n";
		}	
	}
}

int main(void){
	//set locale to spanish. Necessary to read decimal separator correctly
	std::setlocale(LC_ALL, "es_ES.UTF-8");
	
	//read file from standard input
	std::istream* std_in = &std::cin;
	
//	std::string line, city_max_rad;
//	*std_in>>line;
//	std::vector<std::string> city_rad;
//	split(line, city_rad, '\t');
//	city_max_rad = line[0];
//	*std_in>>line;
//	
//	std::cout<<city_max_rad;
	
	filter_rain_csv(*std_in, "\"Lepe\"");
	
	return 0;
}
