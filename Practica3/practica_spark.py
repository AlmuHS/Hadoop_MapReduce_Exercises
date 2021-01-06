from pyspark import SparkConf, SparkContext
from pyspark.sql import SparkSession
from pyspark.sql.types import *
from pyspark.sql.functions import *

'''
Configuracion: connect with spark local instance
'''

conf = (SparkConf().setMaster("local").setAppName("Practica3").set("spark.executor.memory","1g"))
sc = SparkContext(conf= conf)
spark = SparkSession(sc)


'''
Preprocessing stage: Read data from file, and fix format problems
'''

# Read data from file
station_data = spark.read.options(delimiter=';', header='True') \
                    .csv("RIA_exportacion_datos_diarios_Huelva_20140206.csv")

#Remove quotes and apply casting to integer
remove_quotes = udf(lambda x : int(str(x).replace('\"', '')), IntegerType())
station_fix_quotes = station_data.withColumn('IDESTACION',remove_quotes(station_data.IDESTACION))

#Remove NULL values before casting values
fix_empty = station_fix_quotes.fillna("0", subset=['PRECIPITACION', 'RADIACION'])

#Fix decimal separator and apply casting to float
commaToDot = udf(lambda x : float(str(x).replace(',', '.')), FloatType())
radiation_fix_decimal = fix_empty.withColumn('RADIACION',commaToDot(fix_empty.RADIACION))
rain_fix_decimal = radiation_fix_decimal.withColumn('PRECIPITACION',commaToDot(radiation_fix_decimal.PRECIPITACION))

'''
Task 1: Calculate the average radiation of a certain set of stations
'''

#Filter the stations which match with our station IDs
station_filtered = rain_fix_decimal.filter((rain_fix_decimal.IDESTACION > 1) & (rain_fix_decimal.IDESTACION < 11))

#Calculate the radiation average for each station
avg_radiation = station_filtered.groupBy("SESTACION") \
                                .avg("RADIACION") \
                                .withColumnRenamed("avg(RADIACION)", "AVG_RADIACION")

# Group all partitions and save data to file
avg_radiation.repartition(1).write.csv("output_radiation", sep='\t')

'''
Task 2: From the station with highest average radiation, calculate the sum of rains for each year
'''

# Get the station with highest average radiation
max_avg_radiation = avg_radiation.orderBy(col("AVG_RADIACION").desc()) \
                                 .limit(1)

# Filter the name of this station
station_max_rad = max_avg_radiation.select(max_avg_radiation.columns[0])

# Store the name in a variable
name_smaxrad = station_max_rad.first()["SESTACION"]

# Filter the data of this station, comparing its name with the previous variable
station_filtered2 = station_filtered.filter(station_filtered.SESTACION == name_smaxrad)

# Extract year from date
rain_filter_year = station_filtered2.withColumn('FECHA', station_filtered2['FECHA'].substr(7,10)) \
                                 .withColumnRenamed("FECHA", "ANIO")        

# For this station, sum the rain for each year
rain_year = rain_filter_year.groupBy("SESTACION", "ANIO") \
                            .sum("PRECIPITACION") \
                            .withColumnRenamed("sum(PRECIPITACION)", "TOTAL_PRECIPITACION") \
                            .select("SESTACION", "ANIO", "TOTAL_PRECIPITACION") \
                            .orderBy(col("ANIO").asc())

# Group all partitions and save data to file
rain_year.repartition(1).write.csv("output_rain", sep='\t')


