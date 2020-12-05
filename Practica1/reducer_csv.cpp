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

using map_pair = std::unordered_map<std::string, std::pair<float, float> >;


map_pair calculate_avg(std::istream& file){
	std::unordered_map<std::string, int> ocurrences_map;
	map_pair avg_radiation_rain;
	
 	std::string line;
 	
 	//Read file line to line
	while(std::getline(file, line)){
	
		//Split the line in key and values
		std::vector<std::string> values;
		split(line, values, '\t');
		
		//extract key and value
		std::string city = values[0];
		float radiation = std::stof(values[1]);
		float rain = std::stof(values[2]);
		
		//Check if the key exists in the unordered_map
		map_pair::iterator it_key = avg_radiation_rain.find(city);
		
		//If exists, sum the new value
		if(it_key != avg_radiation_rain.end()){
		
			//Increase the number of ocurrences of this key
			ocurrences_map[city]++;
			
			//Sum the new values to this key
			it_key->second.first += radiation;
			it_key->second.second += rain;
		}
		//If not exists, add the pair to the unordered_map
		else{
			avg_radiation_rain[city] = std::make_pair(radiation, rain);
		}
	}
	
	//Calculate the average of each key
	for(map_pair::iterator it = avg_radiation_rain.begin(); it != avg_radiation_rain.end(); ++it) {
	
		//Get key and value
		std::string city = it->first;
		float radiation = it->second.first;
		float rain = it->second.second;
		
		//Find the number of ocurrences of this key in the file
		int ocurrences = ocurrences_map[city];
		
		//Calculate the average
		it->second.first = radiation/ocurrences;
		it->second.second = rain/ocurrences;
	}
	
	return avg_radiation_rain;
}


int main(void){
	map_pair avg_map;
	
	//read file from standard input
	std::istream* std_in = &std::cin;
	avg_map = calculate_avg(*std_in);
	
	//write results to standard output
	for (auto& map: avg_map)
	{
		std::string city = map.first;
		float radiation = map.second.first;
		float rain = map.second.second;
		
		//show the result
		std::cout<<city<<"\t"<<radiation<<"\t"<<rain<<"\n";
	}
	
	return 0;
}
