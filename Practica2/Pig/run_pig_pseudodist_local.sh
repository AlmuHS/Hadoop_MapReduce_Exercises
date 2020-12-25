#!/bin/bash

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

rm -rf output_ra*

pig -x local practica2.pig
