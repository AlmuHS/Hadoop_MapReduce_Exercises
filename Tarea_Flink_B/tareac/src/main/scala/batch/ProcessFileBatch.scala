/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.myorg.tareac

import org.apache.flink.api.scala._
//import org.apache.flink.streaming.api.scala.{DataStream, StreamExecutionEnvironment}

//import org.apache.flink.table.api._
//import org.apache.flink.table.sources._
//import org.apache.flink.table.api.bridge.scala._
//import org.apache.flink.api.common.typeinfo.Types

//import org.apache.flink.core.fs.FileSystem 

//import org.apache.flink.api.scala.ExecutionEnvironment
//import org.apache.flink.table.api.bridge.scala.BatchTableEnvironment

//import org.apache.flink.table.descriptors.Csv

/**
 * Skeleton for a Flink Batch Job.
 *
 * For a tutorial how to write a Flink batch application, check the
 * tutorials and examples on the <a href="https://flink.apache.org/docs/stable/">Flink Website</a>.
 *
 * To package your application into a JAR file for execution,
 * change the main class in the POM.xml file to this class (simply search for 'mainClass')
 * and run 'mvn clean package' on the command line.
 */
object BatchJob {

  def main(args: Array[String]) {
    // set up the batch execution environment
    val env = ExecutionEnvironment.getExecutionEnvironment
    //val fbTableEnv = BatchTableEnvironment.create(fbEnv)

    val inputPath = "file:////home/almu/Practicas_BigData/Tarea_Flink_B/RIA_exportacion_datos_diarios_Huelva_20140206.csv"
    val input = env.readCsvFile[(String, String, String, String, String, String,
                                 String, String, String, String, String, String,
                                 String, String, String, String, String, String)](inputPath, fieldDelimiter=";", ignoreFirstLine=true)


    

//    val schema = new Schema()
//        .field("IDPROVINCIA", Types.STRING)
//        .field("SPROVINCIA", Types.STRING)
//        .field("IDESTACION", Types.STRING)
//        .field("SESTACION", Types.STRING)
//        .field("FECHA", Types.STRING)
//        .field("DIA", Types.STRING)
//        .field("TEMPMAX", Types.STRING)
//        .field("HORMINTEMPMAX", Types.STRING)
//        .field("TEMPMIN", Types.STRING)
//        .field("HORMINTEMPMIN", Types.STRING)
//        .field("TEMPMEDIA", Types.STRING)
//        .field("HUMEDADMAX", Types.STRING)
//        .field("HUMEDADMIN", Types.STRING)
//        .field("HUMEDADMEDIA", Types.STRING)
//        .field("VELVIENTO", Types.STRING)
//        .field("DIRVIENTO", Types.STRING)
//        .field("RADIACION", Types.STRING)
//        .field("PRECIPITACION", Types.STRING)

//    fbTableEnv.connect(new FileSystem().path(inputPath))
//        .withFormat(new Csv().fieldDelimiter(';').deriveSchema())
//        .withSchema(schema)
//        .createTemporaryTable("StationData")

    //val station_data = fbTableEnv.from("StationData")
    
    val casted_data = input.map((data: (String, String, String, String,
                                        String, String, String, String, String,
                                        String, String, String, String,
                                        String, String, String, String, String)) => 
                                     {    
                                          (data._3.replace("\"", "").toInt, //idestacion
                                          data._4.replace("\"", ""), //nombre estacion
                                          data._5.substring(6,10).toInt, //anio
                                          data._17.replace(",", ".").replace("", "0").toDouble, //radiacion
                                          data._18.replace(",", ".").replace("", "0").toDouble) //precipitacion
                                      }
                                 )
                                 
     casted_data.print()
    
     val station_filtered = casted_data.filter((fixed_data:(Int, String, Int, Double, Double)) => {fixed_data._3 > 1 && fixed_data._3 < 11})
    
     station_filtered.print()
//    
//     val station_group = station_filtered.groupBy(3)
     
     //station_group.print()
    
    /*
     * Here, you can start creating your execution plan for Flink.
     *
     * Start with getting some data from the environment, like
     *  env.readTextFile(textPath);
     *
     * then, transform the resulting DataSet[String] using operations
     * like
     *   .filter()
     *   .flatMap()
     *   .join()
     *   .group()
     *
     * and many more.
     * Have a look at the programming guide:
     *
     * https://flink.apache.org/docs/latest/apis/batch/index.html
     *
     * and the examples
     *
     * https://flink.apache.org/docs/latest/apis/batch/examples.html
     *
     */


    // execute program
    env.execute("Flink Batch CSV Scala Processing")
  }
}
