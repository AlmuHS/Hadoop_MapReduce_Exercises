#!/bin/bash

#Change to Hadoop standalone installation
if test -d /usr/local/hadoop_std
then
	HADOOP_STD=$(grep hadoop_std ~/.bashrc | wc -l)
	if test $HADOOP_STD == 0
	then
		sed -i '/HADOOP_HOME=/s|hadoop|hadoop_std|' ~/.bashrc
		export HADOOP_HOME
		export JAVA_HOME
	fi
fi

echo $HADOOP_HOME

rm -rf output_ra*

pig -x local practica2.pig
