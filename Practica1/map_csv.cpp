#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <iostream>
#include <iterator>
#include <regex>
#include <locale>
#include <unordered_map>

#include <cstdio>

const int RADIATION = 16;
const int RAIN = 17;
const int REGION = 1;
const int CITY = 3;

using map_pair = std::unordered_map<std::string, std::pair<float, float> >;

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ')
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


map_pair read_csv(std::istream& file){
	map_pair pair_list;
	
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
		std::string rain_str = row[RAIN];
		
		radiation_str = fix_empty_values(radiation_str);
		rain_str = fix_empty_values(rain_str);
		
		float rain = std::stof(rain_str);
		float radiation = std::stof(radiation_str);
		
		pair_list[city] = std::make_pair(radiation, rain);	
	}
	
	return pair_list;
}

int main(void){
	map_pair row_filtered;
	
	//set locale to spanish. Necessary to read decimal point correctly
	std::setlocale(LC_ALL, "es_ES.UTF-8");
	
	//read file from standard input
	std::istream* std_in = &std::cin;
	row_filtered = read_csv(*std_in);
	
	//write results to standard output
	for (auto& row: row_filtered){
		std::cout<<row.first<<"\t"<<row.second.first<<"\t"<<row.second.second<<"\n";
	}
	
	return 0;
}
