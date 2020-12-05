all: map_csv combiner_radiation reduce_radiation reducer_rain

map_csv: map_csv.cpp
	g++ map_csv.cpp -O2 -o map_csv
	
combiner_radiation: combiner_radiation.cpp
	g++ combiner_radiation.cpp -O2 -o combiner_radiation

reduce_radiation: reduce_radiation.cpp
	g++ reduce_radiation.cpp -O2 -o reduce_radiation

reducer_rain: reducer_rain.cpp
	g++ reducer_rain.cpp -O2 -o reducer_rain

clean:
	rm map_csv combiner_radiation reduce_radiation reducer_rain
