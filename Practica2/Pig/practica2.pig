raw = load 'RIA_exportacion_datos_diarios_Huelva_20140206.csv' using PigStorage(';') 
AS (id_region:int, name_region:chararray, id_station: chararray, name_station: chararray, 
	date: chararray, dia: int, tempmax: chararray, hormintempmax: int, tempmin: chararray, 
	hormintempmin: int, temp_mid: chararray, humidity_max: chararray, 
	humidity_min: chararray, humidity_mid: chararray, speed_wind: chararray, 
	dir_wind: chararray, radiation: chararray, rain: chararray
);

/* Preprocessing: Extract the relevant fields from the file, fixing the format
 * and casting to appropiated type
 */

--Skip header
filter_data = FILTER raw BY id_station != 'IDESTACION';

--Extract relevant data, fixing problems and applying casting to their types
replace_data = FOREACH filter_data GENERATE 
	(int) SUBSTRING(id_station,1, (int)SIZE(id_station)-1) as id_station, --Remove ""
	SUBSTRING(name_station,1, (int)SIZE(name_station)-1) as name_station, --Remove ""
	(float) REPLACE(radiation, ',', '.') as radiation, --Fix decimal separator
	(float) REPLACE(rain, ',', '.') as rain, --Fix decimal separator
	(int) SUBSTRING(date, (int)SIZE(date)-4, (int)SIZE(date)) as year; --Extract year from date

-----------------------------------------------------------------------------------------------------

/* Task 1: Get the average radiation of certain stations, filtered by ID
 * The results will be stored in a file, using format "name_station avg_radiation"
 */

--Filter the stations which corresponds with ID
filter_station = FILTER replace_data BY id_station > 1 AND id_station < 11;
station_radiation = FOREACH filter_station GENERATE name_station, radiation;

--Group the rows by station name, to calculate its average
radiation_gr = GROUP station_radiation BY name_station;

--Extract the radiation average of each station
avg_radiation = FOREACH radiation_gr GENERATE 
	FLATTEN(station_radiation.name_station) as name_station, 
	AVG(station_radiation.radiation) as mid_radiation;

--Remove repeated rows
avg_radiation_dist = DISTINCT avg_radiation;

--Store results in a file
STORE avg_radiation_dist INTO './output_radiation' using PigStorage('\t');

------------------------------------------------------------------------------------------------------

/* Task 2: From the station with highest average radiation, 
 * get the sum of the rain of each year.
 * The results will be stored in a file, with format "name_station year sum_rain"
 */

/* Task 2.1. Get the station with highest average radiation.
 * Using the data output by Task 1, find the name of the station with highest average radiation
 */

--Find the maximum average radiation value
max_avg_radiation = ORDER avg_radiation_dist BY mid_radiation DESC;
max_rad_station = LIMIT max_avg_radiation 1;

/* Task 2.2: From station with highest average radiation, filter the rain and year columns
 *  Find the rows of this station in original replace_data, and extract its rain and year values
 */

--Filter the rain values of this station
filter_rain = FILTER replace_data BY name_station == max_rad_station.name_station;
station_rain = FOREACH filter_rain GENERATE name_station, rain, year;

--/* Task 2.3: Using data filtered by Task 2.2, calculate the sum of rain for each year
-- * The results will be stored in a file, with format "name_station year sum_rain"
-- */

rain_gr = GROUP station_rain BY year;

rain_year = FOREACH rain_gr GENERATE
	FLATTEN(station_rain.(name_station, year)),
	SUM(station_rain.rain);
rain_year_dist = DISTINCT rain_year;

STORE rain_year_dist INTO './output_rain' using PigStorage('\t');


