#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <iostream>
#include <iterator>
#include <regex>
#include <locale>
#include <unordered_map>

struct data{
	std::string city;
	float radiation;
};

const int RADIATION = 16;
const int RAIN = 17;
const int REGION = 1;
const int CITY = 3;
const int DATE = 4;

using vector_data = std::vector<std::pair<std::string, float> >;

void split(const std::string& str, std::vector<std::string>& cont, char delim = ' ')
{
    size_t start = 0;
    size_t end = str.find(delim);

    while (end != std::string::npos)
    {
        cont.push_back(str.substr(start, end-start));
        start = end + 1;
        end = str.find(delim, start);
    }
    cont.push_back(str.substr(start, end - start));
}

std::string fix_empty_values(std::string value){
	if(value == "" || value[0] == '\n' || value[0] == '\r'){
		value = "0";
	}
	
	return value;
}


vector_data read_csv(std::istream& file){
	vector_data data;
	
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
		
		data.emplace_back(city, radiation);	
	}
	
	return data;
}

int main(void){
	vector_data row_filtered;
	
	//set locale to spanish. Necessary to read decimal point correctly
	std::setlocale(LC_ALL, "es_ES.UTF-8");
	
	//read file from standard input
	std::istream* std_in = &std::cin;
	row_filtered = read_csv(*std_in);
	
	//write results to standard output
	for (auto& row: row_filtered){
		std::string city = row.first;
		float radiation = row.second;
	
		std::cout<<city<<"\t"<<radiation<<"\n";
	}
	
	return 0;
}
