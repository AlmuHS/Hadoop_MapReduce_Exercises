rm -rf output_radiation output_rain

#Change to Hadoop Standalone installation
#Check if exists any Hadoop Standalone installation
if test -d /usr/local/hadoop_std
then
	#If yes, check if the HADOOP_HOME is set to Hadoop Standalone path
	HADOOP_STD=$(grep hadoop_std ~/.bashrc | wc -l)
	
	#If not, replace the HADOOP_HOME path in .bashrc
	if test $HADOOP_STD == 0
	then
		#Replace HADOOP_HOME path to Hadoop Standalone installation
		sed -i '/HADOOP_HOME=/s|hadoop|hadoop_std|' ~/.bashrc
		
		#Update variable to get the new path
		export HADOOP_HOME
	fi
fi

#Run the tasks
START1=$(date +%s)
hadoop jar $HADOOP_HOME/share/hadoop/tools/lib/hadoop-streaming-2.10.1.jar \
   -input RIA_exportacion_datos_diarios_Huelva_20140206.csv \
   -output output_radiation \
   -mapper map_radiation \
   -reducer reduce_radiation
   
hadoop jar $HADOOP_HOME/share/hadoop/tools/lib/hadoop-streaming-2.10.1.jar \
   -input RIA_exportacion_datos_diarios_Huelva_20140206.csv \
   -output output_rain \
   -mapper map_rain \
   -reducer reducer_rain
END1=$(date +%s)
   
time1=$(($END1 - $START1))

echo "Total time: $time1 scs"
