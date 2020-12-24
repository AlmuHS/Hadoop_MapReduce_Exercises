USER="hive"
HADOOP_DIR="hdfs://0.0.0.0:9000/user/$USER"

#Change to Hadoop PseudoDistributed installation
if test -d /usr/local/hadoop_std
then
	HADOOP_STD=$(grep hadoop_std ~/.bashrc | wc -l)
	if test $HADOOP_STD == 1
	then
		sed -i '/HADOOP_HOME=/s|hadoop_std|hadoop|' ~/.bashrc
		export HADOOP_HOME
		export JAVA_HOME
	fi
fi

hadoop fs -copyFromLocal RIA_exportacion_datos_diarios_Huelva_20140206.csv $HADOOP_DIR
hadoop fs -rm -r -f $HADOOP_DIR/output_radiation $HADOOP_DIR/output_rain

rm -rf output_ra*

#hive script

hadoop fs -copyToLocal $HADOOP_DIR/output_radiation ./output_radiation
hadoop fs -copyToLocal $HADOOP_DIR/output_rain ./output_rain
