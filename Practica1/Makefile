all: map_radiation reduce_radiation map_rain reducer_rain

map_radiation: map_radiation.cpp
	g++ map_radiation.cpp -O2 -o map_radiation

reduce_radiation: reduce_radiation.cpp
	g++ reduce_radiation.cpp -O2 -o reduce_radiation

map_rain: map_rain.cpp
	g++ map_rain.cpp -O2 -o map_rain

reducer_rain: reducer_rain.cpp
	g++ reducer_rain.cpp -O2 -o reducer_rain

clean:
	rm map_radiation reduce_radiation map_rain reducer_rain
