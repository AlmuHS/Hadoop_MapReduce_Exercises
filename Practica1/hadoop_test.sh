HADOOP_DIR="hdfs://0.0.0.0:9000/user/almu"

hadoop fs -rm -r -f $HADOOP_DIR/output_radiation $HADOOP_DIR/output_rain

hadoop jar /usr/local/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.10.1.jar \
   -input $HADOOP_DIR/Practica1/RIA_exportacion_datos_diarios_Huelva_20140206.csv \
   -output $HADOOP_DIR/output_radiation \
   -mapper map_radiation \
   -reducer reduce_radiation

rm -rf output_radiation

hadoop fs -copyToLocal $HADOOP_DIR/output_radiation ./output_radiation
   
hadoop jar /usr/local/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.10.1.jar \
   -input $HADOOP_DIR/Practica1/RIA_exportacion_datos_diarios_Huelva_20140206.csv \
   -output $HADOOP_DIR/output_rain \
   -mapper map_rain \
   -reducer reducer_rain

rm -rf output_rain

hadoop fs -copyToLocal $HADOOP_DIR/output_rain ./output_rain
