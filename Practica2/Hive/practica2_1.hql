CREATE SCHEMA IF NOT EXISTS practica2;

DROP TABLE practica2.station_data;

CREATE TABLE practica2.station_data (IDPROVINCIA string, SPROVINCIA string, IDESTACION string, SESTACION string, FECHA string, DIA int, TEMPMAX string, HORMINTEMPMAX string, TEMPMIN string, HORMINTEMPMIN string, TEMPMEDIA string, HUMEDADMAX string, HUMEDADMIN string, HUMEDADMEDIA string, VELVIENTO string, DIRVIENTO string, RADIACION string, PRECIPITACION string) 
ROW FORMAT DELIMITED 
FIELDS TERMINATED BY ';'
STORED AS TEXTFILE;

LOAD DATA LOCAL INPATH './RIA_exportacion_datos_diarios_Huelva_20140206.csv' INTO TABLE practica2.station_data;
