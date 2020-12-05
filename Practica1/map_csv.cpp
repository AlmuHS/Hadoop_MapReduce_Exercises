#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <iostream>
#include <iterator>
#include <regex>
#include <locale>

const int RADIATION = 16;
const int REGION = 1;
const int CITY = 3;

using vector_pair = std::vector<std::pair<std::string, float> >;

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

vector_pair read_csv(std::istream& file){
	vector_pair pair_list;
	
 	std::string line;
 	
	//Skip header
 	std::getline(file, line);
 	
 	//Read file line to line
	while(std::getline(file, line)){
		std::vector<std::string> row;
		
		//replace empty fields with a blank character, to avoid errors in the split
		line = std::regex_replace(line, std::regex(";;"), "; ;");
		
		//split the line by columns
		split(line, row, ';');
		
		//Filter the radiation, together the city, of each city of Huelva. Ignore the rows with empty values
		if(row[CITY] != " " && row[RADIATION] != " "){
			pair_list.emplace_back(row[CITY], std::stof(row[RADIATION]));
		}
	}
	
	return pair_list;
}

int main(void){
	vector_pair row_filtered;
	
	//set locale to spanish. Necessary to read decimal point correctly
	std::setlocale(LC_ALL, "es_ES.UTF-8");
	
	//read file from standard input
	std::istream* std_in = &std::cin;
	row_filtered = read_csv(*std_in);
	
	//write results to standard output
	for (auto& row: row_filtered){
		std::cout<<row.first<<"\t"<<row.second<<"\n";
	}
	
	return 0;
}
