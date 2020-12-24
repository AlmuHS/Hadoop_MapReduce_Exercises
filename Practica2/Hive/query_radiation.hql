CREATE SCHEMA IF NOT EXISTS practica2;

CREATE TABLE practica2.station_data (IDPROVINCIA string, SPROVINCIA string, IDESTACION string, SESTACION string, FECHA string, DIA int, TEMPMAX string, HORMINTEMPMAX string, TEMPMIN string, HORMINTEMPMIN string, TEMPMEDIA string, HUMEDADMAX string, HUMEDADMIN string, HUMEDADMEDIA string, VELVIENTO string, DIRVIENTO string, RADIACION string, PRECIPITACION string) 
ROW FORMAT DELIMITED 
FIELDS TERMINATED BY ';'
STORED AS TEXTFILE;

LOAD DATA LOCAL INPATH './RIA_exportacion_datos_diarios_Huelva_20140206.csv' INTO TABLE practica2.station_data;

CREATE TEMPORARY TABLE IF NOT EXISTS practica2.station_filtered(SESTACION STRING, RADIACION float, ANIO int, PRECIPITACION float);
INSERT OVERWRITE TABLE practica2.station_filtered
        SELECT REGEXP_REPLACE(SESTACION, '\"', ''), 
                CAST(REGEXP_REPLACE(RADIACION, ',', '.') as float), 
                CAST(SUBSTR(FECHA, 7, 10) as int) as ANIO, 
                CAST(REGEXP_REPLACE(PRECIPITACION, ',', '.') as float)
        FROM practica2.station_data
        WHERE CAST(REGEXP_REPLACE(IDESTACION, '\"', '') as int) BETWEEN 2 AND 10;

CREATE TEMPORARY TABLE IF NOT EXISTS practica2.avg_rad_table(SESTACION STRING, AVG_RADIACION FLOAT);
INSERT OVERWRITE TABLE practica2.avg_rad_table
        SELECT SF.SESTACION, AVG(SF.RADIACION) as AVG_RADIACION
        FROM practica2.station_filtered as SF
        GROUP BY SF.SESTACION;

INSERT OVERWRITE LOCAL DIRECTORY './output_radiation' ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t'
        SELECT *
        FROM practica2.avg_rad_table;
                

SET hivevar:MAX_AVG_STA=(SELECT SESTACION
                 FROM practica2.avg_rad_table as AVG_R
                 WHERE AVG_RADIACION IN (
                                            SELECT MAX(AVG_RADIACION)
                                            FROM practica2.avg_rad_table
                                         ));


INSERT OVERWRITE LOCAL DIRECTORY './output_rain' ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t'
        SELECT SESTACION, ANIO, SUM(PRECIPITACION) as SUM_PREC
        FROM practica2.station_filtered
        WHERE SESTACION IN ${hivevar:MAX_AVG_STA}
        GROUP BY SESTACION, ANIO;
