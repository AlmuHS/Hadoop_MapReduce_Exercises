#include <string>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>

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

std::unordered_map<std::string, float> calculate_avg(std::istream& file){
	std::unordered_map<std::string, int> ocurrences_map;
	std::unordered_map<std::string, float> avg_radiation_map;
	
 	std::string line;
 	
 	//Read file line to line
	while(std::getline(file, line)){
	
		//Split the line in key and values
		std::vector<std::string> values;
		split(line, values, '\t');
		
		//extract key and value
		std::string city = values[0];
		float radiation = std::stof(values[1]);
		
		//Check if the key exists in the unordered_map
		std::unordered_map<std::string, float>::iterator it_key = avg_radiation_map.find(city);
		
		//If exists, sum the new value
		if(it_key != avg_radiation_map.end()){
		
			//Increase the number of ocurrences of this key
			ocurrences_map[city]++;
			
			//Sum the new values to this key
			it_key->second += radiation;
		}
		//If not exists, add the pair to the unordered_map
		else{
			avg_radiation_map[city] = radiation;
		}
	}
	
	//Calculate the average of each key
	for(std::unordered_map<std::string, float>::iterator it = avg_radiation_map.begin(); it != avg_radiation_map.end(); ++it) {
	
		//Get key and value
		std::string city = it->first;
		float radiation = it->second;
		
		//Find the number of ocurrences of this key in the file
		int ocurrences = ocurrences_map[city];
		
		//Calculate the average
		it->second = radiation/ocurrences;
	}
	
	return avg_radiation_map;
}


int main(void){	
	//read file from standard input
	std::istream* std_in = &std::cin;
	std::unordered_map<std::string, float> avg_map = calculate_avg(*std_in);
	
	//write results to standard output
	for (auto& map: avg_map)
	{
		std::string city = map.first;
		float radiation = map.second;
		
		//show the result
		std::cout<<city<<"\t"<<radiation<<"\n";
	}
	
	return 0;
}
