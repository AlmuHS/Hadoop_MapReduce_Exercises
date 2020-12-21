raw = load 'RIA_exportacion_datos_diarios_Huelva_20140206.csv' using PigStorage(';') 
AS (id_provincia:int, nom_provincia:chararray, id_estacion: chararray, nom_estacion: chararray, 
	fecha: chararray, dia: int, tempmax: chararray, hormintempmax: int, tempmin: chararray, 
	hormintempmin: int, temp_media: chararray, humedad_max: chararray, 
	humedad_min: chararray, humedad_media: chararray, vel_viento: chararray, 
	dir_viento: chararray, radiacion: chararray, precipitacion: chararray
);

--Skip header
filter_data = FILTER raw BY id_estacion != 'IDESTACION';

--Remove " from id_estacion and nom_estacion and convert to integer and fix decimal separator
replace_data = FOREACH filter_data GENERATE 
	(int) SUBSTRING(id_estacion,1, (int)SIZE(id_estacion)-1) as id_estacion, 
	SUBSTRING(nom_estacion,1, (int)SIZE(nom_estacion)-1) as nom_estacion,
	(float) REPLACE(radiacion, ',', '.') as radiacion,
	(float) REPLACE(precipitacion, ',', '.') as rain;

--Filter the stations which corresponds with ID
filter_estacion = FILTER replace_data BY id_estacion > 1 AND id_estacion < 11;
estacion_radiacion = FOREACH filter_estacion GENERATE nom_estacion, radiacion;

--Group the rows by station name, to calculate its average
radiation_gr = GROUP estacion_radiacion BY nom_estacion;

--Extract the radiation average of each station
avg_radiation = FOREACH radiation_gr GENERATE 
	FLATTEN(estacion_radiacion.nom_estacion) as nom_estacion, 
	AVG(estacion_radiacion.radiacion) as mid_radiation;

dump avg_radiation;
STORE avg_radiation INTO 'estacion_radiacion' using PigStorage(';');
