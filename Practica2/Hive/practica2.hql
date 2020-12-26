/*
 * Preprocessing tasks: Load data from CSV file
 */
CREATE SCHEMA IF NOT EXISTS practica2;

DROP TABLE practica2.station_data;

CREATE TABLE practica2.station_data (IDREGION string, SREGION string, IDSTATION string, 
                                     SSTATION string, SDATE string, DAY int, TEMPMAX string, 
                                     HORMINTEMPMAX string, TEMPMIN string, HORMINTEMPMIN string, 
                                     TEMPMID string, HUMIDITYMAX string, HUMIDITYMIN string, 
                                     HUMIDITYMID string, SPWIND string, DIRWIND string, 
                                     RADIATION string, RAIN string
                                    )
                                ROW FORMAT DELIMITED 
                                FIELDS TERMINATED BY ';'
                                STORED AS TEXTFILE;

LOAD DATA LOCAL INPATH './RIA_exportacion_datos_diarios_Huelva_20140206.csv' INTO TABLE practica2.station_data;

/*
 * Task 1: Get the average radiation of certain stations
 */

--Filter data from the stations which match with the ID, fixing string and applying casting if necessary
CREATE TEMPORARY TABLE IF NOT EXISTS practica2.station_filtered(SSTATION STRING, RADIATION float, YEAR int, RAIN float);
INSERT OVERWRITE TABLE practica2.station_filtered
        SELECT REGEXP_REPLACE(SSTATION, '\"', ''), 
                CAST(REGEXP_REPLACE(RADIATION, ',', '.') as float), 
                CAST(SUBSTR(SDATE, 7, 10) as int) as YEAR, 
                CAST(REGEXP_REPLACE(RAIN, ',', '.') as float)
        FROM practica2.station_data
        WHERE CAST(REGEXP_REPLACE(IDSTATION, '\"', '') as int) BETWEEN 2 AND 10;

--Calculate average radiation for each station
CREATE TEMPORARY TABLE IF NOT EXISTS practica2.avg_rad_table(SSTATION STRING, AVG_RADIATION FLOAT);
INSERT OVERWRITE TABLE practica2.avg_rad_table
        SELECT SF.SSTATION, AVG(SF.RADIATION) as AVG_RADIATION
        FROM practica2.station_filtered as SF
        GROUP BY SF.SSTATION;

--Export data to a output file
INSERT OVERWRITE LOCAL DIRECTORY './output_radiation' ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t'
        SELECT *
        FROM practica2.avg_rad_table;

                
--Pre-task 2: Get the name of the station with maximum average radiation
SET hivevar:MAX_AVG_STA=(SELECT SSTATION
                         FROM (SELECT SSTATION,
                               RANK() OVER (ORDER BY AVG_RADIATION DESC) as rank_rad
                               FROM practica2.avg_rad_table) sort_tab
                         WHERE sort_tab.rank_rad=1);   

/*
 * Task 2: Calculate the sum of rain of the station with highest average radiation
 */

--Calculate the sum of rain of each year, from this station, Export results to file
INSERT OVERWRITE LOCAL DIRECTORY './output_rain' ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t'
        SELECT SSTATION, YEAR, SUM(RAIN) as SUM_RAIN
        FROM practica2.station_filtered
        WHERE SSTATION IN ${hivevar:MAX_AVG_STA}
        GROUP BY SSTATION,YEAR;
