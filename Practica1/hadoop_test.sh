hadoop jar /usr/local/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.10.1.jar \
   -input RIA_exportacion_datos_diarios_Huelva_20140206.csv \
   -output output \
   -numReduceTasks 1 \
   -mapper map_radiation \
   -combiner combiner_radiation \
   -reducer reduce_radiation
