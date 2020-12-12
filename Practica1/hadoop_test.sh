USER="almu"
HADOOP_DIR="hdfs://0.0.0.0:9000/user/$USER"

#Change to Hadoop PseudoDistributed installation
if test -d /usr/local/hadoop_std
then
	HADOOP_STD=$(grep hadoop_std ~/.bashrc | wc -l)
	if test $HADOOP_STD == 1
	then
		sed -i '/HADOOP_HOME=/s|hadoop_std|hadoop|' ~/.bashrc
		export HADOOP_HOME
	fi
fi

#hadoop fs -copyFromLocal .

hadoop fs -rm -r -f $HADOOP_DIR/output_radiation $HADOOP_DIR/output_rain

START1=$(date +%s)
hadoop jar $HADOOP_HOME/share/hadoop/tools/lib/hadoop-streaming-2.10.1.jar \
   -input $HADOOP_DIR/Practica1/RIA_exportacion_datos_diarios_Huelva_20140206.csv \
   -output $HADOOP_DIR/output_radiation \
   -mapper map_radiation \
   -reducer reduce_radiation
END1=$(date +%s)

time1=$(($END1 - $START1))

rm -rf output_radiation

hadoop fs -copyToLocal $HADOOP_DIR/output_radiation ./output_radiation


START2=$(date +%s)
hadoop jar $HADOOP_HOME/share/hadoop/tools/lib/hadoop-streaming-2.10.1.jar \
   -input $HADOOP_DIR/Practica1/RIA_exportacion_datos_diarios_Huelva_20140206.csv \
   -output $HADOOP_DIR/output_rain \
   -mapper map_rain \
   -reducer reducer_rain
END2=$(date +%s)

time2=$(($END2-$START2))

rm -rf output_rain

hadoop fs -copyToLocal $HADOOP_DIR/output_rain ./output_rain

total_time=$(($time1 + $time2))
echo "Total time: $total_time scs"
