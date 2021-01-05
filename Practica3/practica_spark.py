import locale
from pyspark import SparkConf, SparkContext
from pyspark.sql import SparkSession
from pyspark.sql.types import *
from pyspark.sql.functions import *

conf = (SparkConf().setMaster("local").setAppName("Practica3").set("spark.executor.memory","1g"))
sc = SparkContext(conf= conf)
spark = SparkSession(sc)

station_data = spark.read.options(delimiter=';', header='True').csv("RIA_exportacion_datos_diarios_Huelva_20140206.csv")

#Remove quotes and apply casting to integer
remove_quotes = udf(lambda x : int(str(x).replace('\"', '')), IntegerType())
station_fix_quotes = station_data.withColumn('IDESTACION',remove_quotes(station_data.IDESTACION))

'''
Task 1: 
'''
#Remove NULL values before casting radiation values
radiation_fix_empty = station_fix_quotes.filter(station_fix_quotes.RADIACION.isNotNull())

#Fix decimal separator and apply casting to float
commaToDot = udf(lambda x : float(str(x).replace(',', '.')), FloatType())
radiation_fix_decimal = radiation_fix_empty.withColumn('RADIACION',commaToDot(radiation_fix_empty.RADIACION))

#Filter the stations which match with out IDs
station_filtered = radiation_fix_decimal.filter((radiation_fix_decimal.IDESTACION > 1) & (radiation_fix_decimal.IDESTACION < 11))

#Calculate the radiation average for each station
avg_radiation = station_filtered.groupBy("SESTACION") \
                                .avg("RADIACION") \
                                .withColumnRenamed("avg(RADIACION)", "AVG_RADIACION")

avg_radiation.show()
#avg_radiation.write.parquet("./output_radiation")

'''
Task 2:
'''

# Get the station with highest average radiation
max_avg_radiation = avg_radiation.orderBy(col("AVG_RADIACION").desc()) \
                                 .limit(1)
max_avg_radiation.show()

# Filter the name of this station
station_max_rad = max_avg_radiation.select(max_avg_radiation.columns[0])
name_smaxrad = station_max_rad.first()["SESTACION"]

# Filter the data from this station
station_filtered2 = station_filtered.filter(station_filtered.SESTACION == name_smaxrad)

# Fix format problems
blank_as_zero = udf(lambda x: str(x).replace("", "0"), StringType())
rain_fix_empty = station_filtered2.withColumn("PRECIPITACION", blank_as_zero(station_filtered2.RADIACION))
rain_fix_decimal = rain_fix_empty.withColumn('PRECIPITACION',commaToDot(rain_fix_empty.PRECIPITACION))

# Extract year from date
rain_filter_year = rain_fix_decimal.withColumn('FECHA', rain_fix_decimal['FECHA'].substr(7,10)) \
                                 .withColumnRenamed("FECHA", "ANIO")        
rain_filter_year.show()

rain_year = rain_filter_year.groupBy("PRECIPITACION", "ANIO") \
                            .sum("PRECIPITACION") \
                            .withColumnRenamed("sum(PRECIPITACION)", "TOTAL_PRECIPITACION") \
                            .select("TOTAL_PRECIPITACION", "ANIO")

rain_year.show()


