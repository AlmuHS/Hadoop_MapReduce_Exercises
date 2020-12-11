rm -rf output_radiation output_rain

hadoop jar $HADOOP_HOME/share/hadoop/tools/lib/hadoop-streaming-2.10.1.jar \
   -input RIA_exportacion_datos_diarios_Huelva_20140206.csv \
   -output output_radiation \
   -mapper map_radiation \
   -reducer reduce_radiation
   
hadoop jar $HADOOP_HOME/share/hadoop/tools/lib/hadoop-streaming-2.10.1.jar \
   -input Practica1/RIA_exportacion_datos_diarios_Huelva_20140206.csv \
   -output output_rain \
   -mapper map_rain \
   -reducer reducer_rain
