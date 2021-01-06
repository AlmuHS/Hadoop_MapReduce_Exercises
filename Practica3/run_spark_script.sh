#!/bin/bash

master_url=debian

rm -rf output_ra*
spark-submit --master spark://$master_url:7077 practica_spark.py
