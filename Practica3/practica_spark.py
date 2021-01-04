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
                                .withColumnRenamed("avg(RADIACION)", "AVG_RADIACION") \
                                .orderBy(col("AVG_RADIACION").desc())
avg_radiation.show()
#avg_radiation.write.parquet("./output_radiation")


#blank_as_zero = udf(lambda x: str(x).replace("", "0"), StringType())
#rain_fix_empty = station_fix_quotes.withColumn("PRECIPITACION", blank_as_zero(station_fix_quotes.RADIACION))
#rain_fix_decimal = rain_fix_empty.withColumn('PRECIPITACION',commaToDot(rain_fix_empty.PRECIPITACION))
         

                       


#
