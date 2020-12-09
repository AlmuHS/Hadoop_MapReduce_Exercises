#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <locale>
#include <fstream>

#include "common.hpp"

using vector_data = std::vector<std::pair<std::string, float> >;

std::string calculate_max_radiation(std::istream& file){
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
	
	return city_max_rad.first;
}


void filter_rain_csv(std::istream& file, std::string city_max_rad){	
 	std::string line;
 	
 	//Read file line to line
	while(std::getline(file, line)){
		std::vector<std::string> row;	
		
		//split the line by columns
		split(line, row, ';');
		
		//Filter the rain data of the city which higher radiation (passed by parameter)
		std::string city = row[CITY];
			
		if(city == city_max_rad){
			//Replace empty rain values with zero
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
	
	//Read results from previous job
	std::fstream output_radiation("output_radiation/part-00000", std::fstream::in);
	std::string city_max_rad = calculate_max_radiation(output_radiation);
	
//	std::string city_max_rad;
//	output_radiation>>city_max_rad;
	
	//read csv file from standard input
	std::istream* std_in = &std::cin;
	
	//Call to mapper function, passing the city with highest radiation by parameter
	filter_rain_csv(*std_in, city_max_rad);
	
	return 0;
}
