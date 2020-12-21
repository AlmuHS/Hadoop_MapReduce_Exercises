raw = load 'RIA_exportacion_datos_diarios_Huelva_20140206.csv' using PigStorage(';') 
AS (id_provincia:int, nom_provincia:chararray, id_estacion: chararray, name_station: chararray, 
	fecha: chararray, dia: int, tempmax: chararray, hormintempmax: int, tempmin: chararray, 
	hormintempmin: int, temp_media: chararray, humedad_max: chararray, 
	humedad_min: chararray, humedad_media: chararray, vel_viento: chararray, 
	dir_viento: chararray, radiacion: chararray, precipitacion: chararray
);

--Skip header
filter_data = FILTER raw BY id_estacion != 'IDESTACION';

--Remove " from id_estacion and name_station and convert to integer and fix decimal separator
replace_data = FOREACH filter_data GENERATE 
	(int) SUBSTRING(id_estacion,1, (int)SIZE(id_estacion)-1) as id_estacion, 
	SUBSTRING(name_station,1, (int)SIZE(name_station)-1) as name_station,
	(float) REPLACE(radiacion, ',', '.') as radiacion,
	(float) REPLACE(precipitacion, ',', '.') as rain,
	(int) SUBSTRING(fecha, (int)SIZE(fecha)-4, (int)SIZE(fecha)) as year;

--Filter the stations which corresponds with ID
filter_estacion = FILTER replace_data BY id_estacion > 1 AND id_estacion < 11;
estacion_radiacion = FOREACH filter_estacion GENERATE name_station, radiacion;

--Group the rows by station name, to calculate its average
radiation_gr = GROUP estacion_radiacion BY name_station;

--Extract the radiation average of each station
avg_radiation = FOREACH radiation_gr GENERATE 
	FLATTEN(estacion_radiacion.name_station) as name_station, 
	AVG(estacion_radiacion.radiacion) as mid_radiation;

--Remove repeated rows
avg_radiation_dist = DISTINCT avg_radiation;

--Store results in a file
--STORE avg_radiation_dist INTO 'estacion_radiacion' using PigStorage('\t');

--Find the maximum average radiation value
avg_radiation_gr = GROUP avg_radiation_dist ALL;
max_avg_radiation = FOREACH avg_radiation_gr GENERATE
			MAX(avg_radiation_dist.mid_radiation) as max_rad;

--Find the station with maximum radiation, using the previous value	
max_rad_station = FILTER avg_radiation_dist BY mid_radiation == max_avg_radiation.max_rad;

--Filter the rain values of this station
filter_rain = FILTER replace_data BY name_station == max_rad_station.name_station;
station_rain = FOREACH filter_rain GENERATE name_station, rain, year;


rain_gr = GROUP station_rain BY year;

rain_year = FOREACH rain_gr GENERATE
	FLATTEN(station_rain.(name_station, year)),
	SUM(station_rain.rain);
rain_year_dist = DISTINCT rain_year;


dump rain_year_dist;
STORE rain_year_dist INTO 'estacion_sum_lluvia' using PigStorage('\t');


