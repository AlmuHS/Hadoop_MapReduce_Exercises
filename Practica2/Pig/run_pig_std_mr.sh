#!/bin/bash

USER="almu"
HADOOP_DIR="hdfs://0.0.0.0:9000/user/$USER"

#Change to Hadoop standalone installation
if test -d /usr/local/hadoop_std
then
	HADOOP_STD=$(grep hadoop_std ~/.bashrc | wc -l)
	if test $HADOOP_STD == 0
	then
		sed -i '/HADOOP_HOME=/s|hadoop|hadoop_std|' ~/.bashrc
		export HADOOP_HOME=$HADOOP_HOME
		export JAVA_HOME
	fi
fi

rm -rf output_ra*

pig -x mapreduce practica2.pig
