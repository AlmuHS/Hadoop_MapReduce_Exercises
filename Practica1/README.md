# Memoria de la Practica 1
## Bibliotecas para Big Data I: Hadoop
## Autora: Almudena García Jurado-Centurión


## Indice

<!-- START TOC  -->
- [Memoria de la Practica 1](#memoria-de-la-practica-1)
  - [Bibliotecas para Big Data I: Hadoop](#bibliotecas-para-big-data-i-hadoop)
  - [Autora: Almudena García Jurado-Centurión](#autora-almudena-garc%C3%ADa-jurado-centuri%C3%B3n)
  - [Indice](#indice)
  - [Tarea a realizar](#tarea-a-realizar)
  - [Diseño de la solución](#dise%C3%B1o-de-la-soluci%C3%B3n)
    - [Primera tarea: Media de radiación de cada municipio](#primera-tarea-media-de-radiaci%C3%B3n-de-cada-municipio)
      - [Mapper](#mapper)
      - [Reducer](#reducer)
    - [Segunda tarea: Precipitación total del municipio de mayor radiación](#segunda-tarea-precipitaci%C3%B3n-total-del-municipio-de-mayor-radiaci%C3%B3n)
      - [Mapper](#mapper-1)
      - [Reducer](#reducer-1)
  - [Implementación](#implementaci%C3%B3n)
    - [Hadoop Streaming](#hadoop-streaming)
    - [Implementación en C++](#implementaci%C3%B3n-en-c)
      - [Funciones auxiliares](#funciones-auxiliares)
      - [Implementación de la primera tarea](#implementaci%C3%B3n-de-la-primera-tarea)
        - [Mapper](#mapper-2)
        - [Reducer](#reducer-2)
      - [Implementación de la segunda tarea](#implementaci%C3%B3n-de-la-segunda-tarea)
        - [Mapper](#mapper-3)
        - [Reducer](#reducer-3)
  - [Preparación del entorno Hadoop](#preparaci%C3%B3n-del-entorno-hadoop)
    - [Especificaciones del sistema](#especificaciones-del-sistema)
    - [Especificaciones hardware](#especificaciones-hardware)
      - [Instalación de Hadoop](#instalaci%C3%B3n-de-hadoop)
  - [Ejecución](#ejecuci%C3%B3n)
    - [Compilación del código](#compilaci%C3%B3n-del-c%C3%B3digo)
    - [Prueba sobre Hadoop en modo *standalone*](#prueba-sobre-hadoop-en-modo-standalone)
      - [Probando el código](#probando-el-c%C3%B3digo)
        - [Medición de tiempos](#medici%C3%B3n-de-tiempos)
  - [Configuración de Hadoop en modo *Pseudo-distributed node*](#configuraci%C3%B3n-de-hadoop-en-modo-pseudo-distributed-node)
    - [Copiando la anterior instalación de Hadoop](#copiando-la-anterior-instalaci%C3%B3n-de-hadoop)
    - [Configuración de ssh](#configuraci%C3%B3n-de-ssh)
    - [Desactivando Hosts](#desactivando-hosts)
    - [Configuración de Hadoop](#configuraci%C3%B3n-de-hadoop)
    - [Arrancando los servicios](#arrancando-los-servicios)
    - [Creando directorios en HDFS](#creando-directorios-en-hdfs)
    - [Compartiendo datos entre HDFS y nuestro sistema de ficheros local](#compartiendo-datos-entre-hdfs-y-nuestro-sistema-de-ficheros-local)
    - [Probando nuestro proyecto en Hadoop en modo *pseudo-distributed*](#probando-nuestro-proyecto-en-hadoop-en-modo-pseudo-distributed)
      - [Modificando el script](#modificando-el-script)
      - [Ejecutando el script](#ejecutando-el-script)
    - [Volviendo a Hadoop *Standalone*](#volviendo-a-hadoop-standalone)
    - [Volviendo a Hadoop *Pseudo-distributed*](#volviendo-a-hadoop-pseudo-distributed)
  - [Comparativa de tiempos](#comparativa-de-tiempos)
    - [Comparación entre modos de instalación](#comparaci%C3%B3n-entre-modos-de-instalaci%C3%B3n)
    - [Ejecución directa sobre la consola](#ejecuci%C3%B3n-directa-sobre-la-consola)
  - [Conclusiones](#conclusiones)

<!-- END TOC-->


## Tarea a realizar

La tarea a realizar consiste en el procesado de un conjunto de datos en formato csv, aplicando la metodología MapReduce sobre Hadoop.

El conjunto de datos se encuentra disponible en formato CSV para descargar desde la  siguiente dirección: http://www.juntadeandalucia.es/datosabiertos/portal/catalogo/detalle/49903.html. 

Los  datos  se  extienden desde el año 2000 hasta primeros del año 2014, mediante una instancia para cada día y cada estación de medida. No vamos a restringir el período de medida, es decir, utilizaremos todos esos años de los que disponemos información, puesto que cuanto mayor sea el intervalo de estudio, mejor resultado obtendremos. Las distintas columnas del conjunto de datos contienen información sobre la provincia (en este caso siempre es Huelva) y ubicación de la estación, fecha de la lectura, día, temperatura máxima y hora de la temperatura máxima,  temperatura  mínima  y su  hora,  temperatura  media,  humedades  máxima,  mínima  y media,  velocidad  y  dirección  del  viento,  radiación  y  precipitación.  

De  todos  estos  datos, utilizaremos para nuestro estudio el de la radicación solar media de cada día, despreciando el resto de la información, salvo la del municipio, es decir, nuestro interés está en escoger entre las siguientes opciones solamente: Almonte (Subestación 10), Lepe (2), Gibraleón(3), Moguer(4), Niebla(5), Aroche(6), Puebla de Guzman(7), El Campillo (8) o La Palma del Condado(9).

Pretendemos pues obtener:

a) Una lista de la medida de radiación para cada una de estos diez municipios, es decir, una lista de la media de radiación media, en la que se muestren dos columnas: municipio y media de la radiación media, de modo que observando esta información, tendremos una aproximación al rendimiento que daría en ese municipio,nuestro huerto solar.

b) Queremos saber también, sólo para el municipio con mayor radiación obtenido en el apartado anterior, las precipitaciones totales en cada año de los que contamos con datos en el fichero.

## Diseño de la solución

Para nuestra solución, se ha optado por el uso de dos tareas MapReduce:

- La primera tarea se dedicará a filtrar los municipios y calcular su media de radiación
- La segunda tarea, a partir de los datos de la tarea anterior, obtendrá el municipio de mayor radiación y, para este municipio, calculará el total de precipitaciones por cada año

Ambas tareas recibirán como fichero de entrada, el fichero csv descargado del enlace anterior. La segunda tarea, además, leerá el fichero con los datos de salida de la tarea anterior.

### Primera tarea: Media de radiación de cada municipio

Para esta tarea utilizaremos un *Mapper* y un *Reducer*

#### Mapper

El *mapper* de esta tarea recibirá como entrada el fichero csv llamado *RIA_exportacion_datos_diarios_Huelva_20140206.csv*. Sobre este fichero, el *mapper* filtrará las columnas *IDESTACION*, *SESTACION* y *RADIACION*. El valor de *IDESTACION* se usará como nombre del municipio a filtrar.

A medida que el *mapper* vaya leyendo las filas del fichero, se exportarán ciertas columnas para leer sus valores. En caso de que el valor de la columna *IDESTACION* se corresponda con el ID de alguno de los municipios que queremos filtrar, sacaremos su par de clave y valor formado por (*SESTACION*, *RADIACION*), para pasarlo al *reducer*.

#### Reducer

El *reducer* leerá los pares (municipio, radiación) filtrados por el *mapper* y, a partir de ellos, calculará la media de radiación de cada uno de esos municipios.

Para ello, durante la lectura de los datos del *mapper*, se sumarán los datos de radiación de cada municipio, y se contará el número de ocurrencias de dicho municipio dentro del conjunto de datos recibido.

Una vez acumulados dichos valores, se realizará una segunda iteración, dividiendo el total de radiación de cada municipio entre el número de ocurrencias del mismo.

Estos valores se exportarán en forma de clave-valor, siendo la primera columna el municipio, y la segunda su media de radiación. Estos valores se guardarán en un fichero, que será examinado por la siguiente tarea.


### Segunda tarea: Precipitación total del municipio de mayor radiación

Para esta segunda tarea, utilizaremos otro *mapper* y *reducer*

#### Mapper

El *mapper* de esta tarea recibirá por su entrada el fichero csv. Además, leerá el fichero con los resultados del *Reducer* de la tarea anterior.

Su trabajo se realizará en dos etapas:

- En la primera etapa, se recorrerá el fichero de resultados de la tarea anterior, para encontrar el municipio con mayor radiación.

- En la segunda etapa, se recorre el fichero csv recibido por la entrada y, a partir del municipio encontrado en la etapa anterior, se filtran sus columnas *FECHA* y *PRECIPITACIONES*. La fecha se descompondrá para exportar el año, y ambos datos se exportarán en forma de clave valor, como (fecha, precipitaciones), para ser leídos por el *reducer*.

#### Reducer

El *reducer* de esta tarea recibirá por su entrada el conjunto de datos clave-valor filtrado por el *mapper* y, a partir de ellos, calculará el total de precipitaciones por cada año.

Para ello, a medida que el *reducer* vaya leyendo los datos, irá sumando el valor de precipitaciones al del año que corresponda dicha fila. 

Los datos obtenidos se exportarán en forma clave valor como (año, total_precipitaciones).

## Implementación

Para la implementación de la aplicación, se ha utilizado el lenguaje de programación C++. Se ha decidido el uso de este lenguaje por su capacidad para la optimización de recursos, en especial el uso de memoria, lo cual permitirá realizar las tareas de forma mas eficiente. 

Las tareas se ejecutarán a través de Hadoop Streaming, usando su API.

Aunque Hadoop dispone de librerías para la implementación de aplicaciones en C++, estas están compiladas para 32 bits, requiriendo el uso de un compilador con opciones *multilib* para su compilación. Por desgracia, en nuestro caso, a pesar de usar `g++-multilib`, nos ha resultado imposible compilar con las librerías de Hadoop, manteniendo la compatibilidad con las del sistema.

Por esta razón, se ha optado por implementar el MapReduce sin esas librerías, y lanzar las tareas con sus ejecutables a través de la línea de comandos de Hadoop Streaming.


### Hadoop Streaming

Hadoop Streaming es una herramienta incluida en Hadoop, que permite lanzar tareas MapReduce a partir de ejecutables implementados en diversos lenguajes de programación.

Las tareas se ejecutan mediante scripts similares a este:

	hadoop jar /opt/hadoop/hadoop-2.9.2/share/hadoop/tools/lib/hadoop-streaming-2.9.2.jar \
	 -input <input> \
	 -output <output> \
	 -mapper mapper.py \
	 -combiner combiner.py \
	 -reducer reducer.py \
	 -file mapper.py \
	 -file combiner.py \
	 -file reducer.py

La implementación se realiza escribiendo un único programa para cada subtarea. Así pues, el *mapper* será un programa, el *combiner *(si lo hay) otro, y el *reducer* otro diferente.

Cada programa puede estar escrito en un lenguaje diferente, y se admiten tanto lenguajes compilados como interpretados.

La comunicación con cada programa se realiza utilizando la entrada y salida estándar del sistema. De esta manera, el *mapper* recibirá el fichero indicado en `-input` por su entrada estándar, y sacará los resultados por la salida estándar. Para exportar los datos en formato clave-valor, se mostrarán separados por tabulaciones, tipo

	clave\t\valor

El *reducer* (o *combiner*, dado el caso), recibirá estos datos por su respectiva entrada estándar, en forma de cadenas de caracteres. Esta cadena de caracteres se podrá separar para obtener su clave y valor, y de esta forma, procesarlo.

Los datos del *reducer* se volverán a exportar en forma clave-valor, separando los diferentes campos mediante tabulaciones.

En este último paso, los datos extraídos por el *reducer* se escribirán a un fichero, en el mismo formato que se haya establecido en el programa.

En caso de que no haya *reducer*, los datos del fichero serán los del último paso ejecutado: el *mapper* o el *combiner*.

**Importante: En caso de que los datos extraidos por la salida estándar no sigan un formato clave-valor, Hadoop mostrará un error y terminará la ejecución**

### Implementación en C++

La implementación en C++ se realizará utilizando la versión C++14, con las estructuras de la librería estándar (STL).

Se compondrá de 4 ficheros (*map_radiation.cpp*, *reduce_radiation.cpp*, *map_rain.cpp* y *reducer_rain.cpp*) dos por cada tarea, con el *mapper* y el *reducer* de cada una de ellas. Además, se incluirá un tercero (*common.hpp*) con algunas funciones y constantes comunes para ellas.

#### Funciones auxiliares

Nuestra aplicación requiere de algunas funciones auxiliares para realizar ciertas tareas no incluidas en el lenguaje.
Para ello, estas se implementarán en el fichero *common.hpp*.

- **Separación de cadenas por delimitadores: split**

	Dado que C++ no dispone en su librería estándar una función *split()*, esta se implementará mediante los métodos `find()` y `substr` de `std::string` (la clase para cadenas de texto de la STL).
	
	Su implementación es la siguiente:
	
		void split(const std::string& str, std::vector<std::string>& cont, char delim = ' ')
		{
		    size_t start = 0;
		    size_t end = str.find(delim);
		
		    while (end != std::string::npos)
		    {
		        cont.push_back(str.substr(start, end-start));
		        start = end + 1;
		        end = str.find(delim, start);
		    }
		    cont.push_back(str.substr(start, end - start));
		}
		
	La función recibe por referencia la cadena de caracteres a trocear, el vector donde se almacenará cada sección, y el delimitador (o separador) a utilizar para separarlos.
	
	El vector recibido se rellenará con los campos obtenidos de la cadena troceada. Para el vector, se utiliza la clase `std::vector` de la STL, que genera un vector dinámico sin requerir tamaño inicial.


- **Separacion de pares clave-valor: get_key_value()**

	Aunque esta tarea se podría realizar mediante la función anterior, se ha preferido reimplementarla mediante otra técnica algo mas eficiente.
	
	Su implementación es la siguiente:
	
		std::pair<std::string, float> get_key_value(std::string line){
			size_t found = line.find_first_of("\t");
			std::pair<std::string, float> key_value;
			
			if(found != std::string::npos)
			{
				std::string key = line.substr(0,found);
				std::string value = line.substr(found);
				float valuei = std::stof(value.c_str());
				
				key_value.first = key;
				key_value.second = std::stof(value);
			}
			
			return key_value;
		}

	En este caso, asumimos que el separador va a ser el tabulador ("\t"). Mediante el método `find_first_of()` de `std::string` encontramos la primera ocurrencia del mismo. Como cada clave solo tiene un valor, separamos la cadena por la posición de dicho delimitador.
	
	La función devuelve un par (representado por la clase `std::pair<a,b>`) formado por una cadena de caracteres (`std::string`) y un número en coma flotante (`float`). Para convertir la cadena del valor a `float` se usa la función `std::stof()` (string to float), que lo convierte automáticamente al valor correspondiente.
	

- **Reemplazo de valores vacíos: fix_empty_values()**

	El fichero csv a procesar incluye gran cantidad de campos con valores vacíos. Estos pueden generar errores al procesar los datos, por lo que se ha optado por ignorarlos en unos casos, o reemplazarlos por valores seguros en otros.
	
	En esta función, reemplazamos los valores vacíos por ceros. Dado que los campos vacíos pueden estar al final de la línea, también reemplazamos los saltos de línea en dichos casos. En nuestro fichero, los saltos de línea están representados por "\r", así que se ha añadido este a la función.
	
		std::string fix_empty_values(std::string value){
			if(value == "" || value[0] == '\n' || value[0] == '\r'){
				value = "0";
			}
			
			return value;
		}
		
#### Implementación de la primera tarea

La primera tarea se encuentra implementada en los ficheros *map_radiation.cpp* y *reduce_radiation.cpp*.

##### Mapper

Esta tarea está implementada en `map_radiation.cpp`, y se compone de una parte de preparación del entorno (en la función main()), y una función de parseo del fichero.

- **Preparación del entorno**

	Antes de empezar con el parseo del fichero, hemos de preparar el entorno para que todo funcione correctamente.
	
	En este caso, hemos de configurar la entrada estándar.
		
	- **Entrada estándar:** Como contamos previamente, en Hadoop Streaming, los ficheros se envían a través de la entrada estándar del programa. En C++, la entrada estándar es accesible mediante la clase `std::cin`. Para facilitar el procesamiento dentro de la función, vamos a convertir la entrada estándar en un flujo de datos, el cual será enviado a la función correspondiente.

		Para crear el flujo de datos, usaremos la clase `std::istream`, que crea un flujo de datos de entrada sin necesidad de un fichero. A este flujo de datos le pasaremos la dirección de memoria del descriptor de la entrada estándar (`std::cin`), la cual la pasaremos por referencia a la función de parseo del fichero, `filter_radiation()`
		  
		 El código correspondiente es este:  
		   
		  //read file from standard input
		  std::istream* std_in = &std::cin;
		  filter_radiation_csv(*std_in);
		  
- **Parseo y filtrado de los datos**

	Una vez preparado el entorno, ejecutamos nuestra función de parseo.
	
	- **Lectura de las líneas**
	
		En esta función leemos el fichero línea a línea, diviendiendo cada línea en sus campos correspondientes. 
		Para ello, usamos la función `std::getline(file, line)`, que guarda en la variable `line` la cadena de caracteres correspondiente a la siguiente línea del fichero. Esta función, llamada sucesivas veces, devolverá cada línea del fichero hasta que este finalice.
			
		En nuestro caso, el fichero csv incluye una cabecera, por lo que debemos saltarla antes de empezar. Para ello, realizamos una llamada adicional antes de comenzar el bucle de procesamiento.
		
			//Skip header
		 	std::getline(file, line);
		 	
		 	//Read file line to line
			while(std::getline(file, line)){    
			  ...  
			}
		
	- **Preprocesado de los datos**
		
		En el bucle de procesamiento, hacemos un *split* para trocear la línea en sus respectivos campos. Para ello, utilizamos la función `split()` antes mencionada, y guardamos los datos en el vector `row`.
		  
			std::vector<std::string> row;
			
			//split the line by columns
			split(line, row, ';');    
		    
		Una vez obtenidos los campos dentro del vector, accedemos a aquellos que nos interesan. Para facilitar el acceso, utilizamos varias constantes definidas en *common.hpp*, que indican los números de columna en los que se almacenan dichos campos. Para añadir legibilidad, guardamos cada campo en una variable de tipo cadena de caracteres.    
		  
		 	//Filter the data about radiation and rain of this city
			std::string city = row[CITY];
			std::string radiation_str = row[RADIATION];
			std::string id_str = row[ID];
			int id;
		  
		El campo *ID* almacena el número del identificador entre comillas. Esto da problemas al intentar convertir su valor en un número entero. Por esa razón, usamos la función `std::erase()` para eliminar dichos caracteres de la cadena. Hecho esto, convertimos su valor a entero usando la función `std::stoi()`.  
		  
			//Remove "" from string
			id_str.erase(std::remove(id_str.begin(), id_str.end(), '\"'), id_str.end());	
			
			//convert ID to int
			id = std::stoi(id_str);
	
	- **Filtrado de los datos**
	
		Finalmente, con todos los valores ya preparados, filtramos los campos que a nosotros nos interesan.
		  
		En el fichero csv hay varias filas cuyo campo *RADIACION* está vacío. Si reemplazáramos esos valores por ceros, podrían dar un resultado incorrecto al calcular la media. Por esta razón, hemos optado por ignorar las filas cuyo campo *RADIACION* esté vacío.
	
	    Además de esto, ignoraremos aquellas filas correspondientes a ciudades no requeridas por el enunciado. En este caso, aquellas cuyo ID sera inferior a 2 o superior a 10.
	
		En caso de que la fila sea aceptada, sacaremos sus valores de ciudad y radiación por la salida estándar, separados por tabulaciones. Para eso, usaremos la clase `std::cout` (la salida estándar se corresponde a la pantalla, por tanto las funciones son las mismas).
	
			//Ignore the empty values and filter the required cities
			if(radiation_str != "" && (id >= 2 && id <= 10)){
				std::cout<<city<<"\t"<<radiation_str<<"\n";
			}
	
El código completo de `map_radiation.cpp` es el siguiente:

	#include <string>
	#include <vector>
	#include <utility>
	#include <iostream>
	#include <locale>
	#include <algorithm>
	
	#include "common.hpp"
	
	void filter_radiation_csv(std::istream& file){	
	 	std::string line;
	 	
		//Skip header
	 	std::getline(file, line);
	 	
	 	//Read file line to line
		while(std::getline(file, line)){
			std::vector<std::string> row;
			
			//split the line by columns
			split(line, row, ';');
			
			//Filter the data about radiation and rain of this city
			std::string city = row[CITY];
			std::string radiation_str = row[RADIATION];
			std::string id_str = row[ID];
			int id;
			
			//Remove "" from string
			id_str.erase(std::remove(id_str.begin(), id_str.end(), '\"'), id_str.end());	
			
			//convert ID to int
			id = std::stoi(id_str);
			
			//Ignore the empty values and filter the required cities
			if(radiation_str != "" && (id >= 2 && id <= 10)){
				std::cout<<city<<"\t"<<radiation_str<<"\n";
			}
			
		}
	}
	
	int main(void){
		//read file from standard input
		std::istream* std_in = &std::cin;
		filter_radiation_csv(*std_in);
		
		return 0;
	}
	

##### Reducer

Esta tarea está implementada en `reduce_radiation.cpp`, y se compone de una parte de preparación del entorno (en la función main()), y una función de lectura de los datos y cálculo de la media.

- **Preparación del entorno**

	La preparación del entorno es muy similar a la usada en el *mapper*. En este caso, además de preparar la entrada estándar, hemos de configurar las locales para corregir el separador decimal.

	- **Separador decimal**: El fichero csv a procesar usa la coma como separador decimal. En C++, el separador decimal por defecto es el punto, por lo que fallaría al convertir las cadenas de texto a valores en coma flotante. Para resolverlo, configuramos las locales en nuestro programa, indicando que vamos a usar el idioma español con codificación UTF-8.   
	
		Las locales establecen el idioma a utilizar dentro del sistema, así como el conjunto de símbolos y caracteres especiales del mismo, entre los que se incluyen el separador decimal. De esta forma, al usar las locales españolas en nuestro programa, este reconocerá la coma como separador decimal, sin necesidad de realizar ningún cambio adicional.  
		  
		Para realizar esta tarea, utilizamos la función `std::setlocale()`, de esta manera:  
		   
		  //set locale to spanish. Necessary to read decimal point correctly
		  std::setlocale(LC_ALL, "es_ES.UTF-8");
		    
		 Esto configurará la variable de entorno `LC_ALL`, que define el idioma del programa en todas las opciones del mismo.

- **Parseo y procesado de los datos**
  
	El parseo y procesado de los datos se realizará en la función `calculate_avg()`, que recibirá el flujo de datos de la entrada estándar, y lo parseará y procesará para calcular las medias de radiación. 
      
	- **Inicialización de las estructuras**: Para calcular la media de radiación de cada ciudad, nos ayudaremos de dos tablas hash: `ocurrences_map` y `sum_radiation_map`. En estas tablas, usaremos el nombre de la ciudad como índice, para almacenar el número de ocurrencias de la misma dentro del fichero, y su suma de radiación. Estos los usaremos posteriormente para calcular la media de radiación de cada ciudad.    
	
  		Para crear estas estructuras, usaremos la clase `std::unordered_map` de la STL.
  		
  		  std::unordered_map<std::string, int> ocurrences_map;
		  std::unordered_map<std::string, float> sum_radiation_map;

	- **Lectura de los pares clave-valor**  
	
		Para la lectura de los datos seguiremos la misma estrategia que en el *mapper*, usando `std::getline()` para leer los datos línea a línea en forma de flujo de datos.  
		  
			std::string line;
			 	
			//Read file line to line
			while(std::getline(file, line)){
			  ...  
			}
		      
		      
		En este caso, cada línea incluirá un par clave-valor separado por tabulaciones. Para separarlos, usaremos la función `get_key_value()` explicada anteriormente. Esta función devuelve un par formado por una cadena de caracteres y un valor en coma flotante. Para crear el par, utilizaremos la clase `std::pair<std::string, float>`  
		  
		  //Split the line in key and values
		  std::pair<std::string, float> city_radiation = get_key_value(line);
		
		Una vez obtenido el par, guardamos los datos del par en dos variables, para facilitar su legibilidad. 
		Para acceder a los elementos del par, utilizamos los atributos `first` y `second`.
		  
		  //extract key and value
		  std::string city = city_radiation.first;
		  float radiation = city_radiation.second;  
		    
	- **Suma de los valores**  
	  
	  	Para sumar los valores, buscamos si la ciudad que queremos sumar existe en la tabla hash. En caso de existir, sumamos el valor de radiación de dicho par, al valor acumulado para dicha ciudad, y se incrementa su número de ocurrencias.
	  	
	  	En caso de no existir, se añade el par a la tabla hash, y se inicializa su número de ocurrencias a 1.
	  	
	  	Para saber si la ciudad existe en la tabla hash, utilizaremos el método `find()` de la clase `std::unordered_map`. Este método devolverá un iterador al elemento en caso de existir, o al final de la estructura en caso contrario.    
	  	 
			//Check if the key exists in the unordered_map
			std::unordered_map<std::string, float>::iterator it_key =
				sum_radiation_map.find(city);
			
			//If exists, sum the new value
			if(it_key != sum_radiation_map.end()){
			
				//Increase the number of ocurrences of this key
				ocurrences_map[city]++;
				
				//Sum the new values to this key
				it_key->second += radiation;
			}
	  	  
  	  En caso de que no exista la ciudad en la tabla, utilizaremos el método `insert()` para insertar la ciudad y su valor de radiación a partir del par generado en el paso anterior; e inicializaremos su número de ocurrencias a 1.

			//If not exists, add the pair to the unordered_map
			else{
				sum_radiation_map.insert(city_radiation);
				
				//Increase the number of ocurrences of this key
				ocurrences_map[city] = 1;
			}
			
	- **Calculo de la media**
	
		Una vez sumados los valores de radiación y el número de ocurrencias de cada ciudad, lanzamos otro bucle para calcular la medias de radiación de cada una.  
		  
		 Para ello, utilizaremos un iterador, en el cual recorreremos todos los pares de `sum_radiation_map`, y dividiremos cada valor total de radiación, por el número de ocurrencias de dicha ciudad.  
		 
		 El iterador lo generaremos dentro de un bucle for con sintaxis  `for(iterador: estructura a recorrer)`. El tipo del iterador lo deduciremos automáticamente mediante la claúsula `auto`, en la que el compilador detectará el tipo a partir del de la estructura.
		     
		 La clase `std::unordered_map` está formada por conjuntos de pares `std::pair<clave, valor>`. Por esta razón, utilizaremos el atributo `first` para acceder a la clave, y el atributo `second` para acceder a su valor.
		       
		 Los valores, los sacaremos por la entrada estándar, utilizando `std::cout`, con la clave y el valor separados mediante tabulaciones. Como clave utilizaremos la ciudad, y como valor su media de radiación.
		    
		 
			//Calculate the average of each key
			for(auto& it: sum_radiation_map){
			
				//Get key and value
				std::string city = it.first;
				float sum_radiation = it.second;
				
				//Find the number of ocurrences of this key in the file
				int ocurrences = ocurrences_map[city];
				
				//Calculate the average
				float avg_radiation = sum_radiation/ocurrences;
				std::cout<<city<<"\t"<<avg_radiation<<"\n";
			}
			
El código completo de este fichero es el siguiente;

	#include <string>
	#include <utility>
	#include <iostream>
	#include <unordered_map>
	
	#include "common.hpp"
	
	void calculate_avg(std::istream& file){
		std::unordered_map<std::string, int> ocurrences_map;
		std::unordered_map<std::string, float> sum_radiation_map;
		
	 	std::string line;
	 	
	 	//Read file line to line
		while(std::getline(file, line)){
		
			//Split the line in key and values
			std::pair<std::string, float> city_radiation = get_key_value(line);
			
			//extract key and value
			std::string city = city_radiation.first;
			float radiation = city_radiation.second;
			
			//Check if the key exists in the unordered_map
			std::unordered_map<std::string, float>::iterator it_key = sum_radiation_map.find(city);
			
			//If exists, sum the new value
			if(it_key != sum_radiation_map.end()){
			
				//Increase the number of ocurrences of this key
				ocurrences_map[city]++;
				
				//Sum the new values to this key
				it_key->second += radiation;
			}
			//If not exists, add the pair to the unordered_map
			else{
				sum_radiation_map.insert(city_radiation);
				
				//Increase the number of ocurrences of this key
				ocurrences_map[city] = 1;
			}
		}
		
		//Calculate the average of each key
		for(auto& it: sum_radiation_map){
		
			//Get key and value
			std::string city = it.first;
			float sum_radiation = it.second;
			
			//Find the number of ocurrences of this key in the file
			int ocurrences = ocurrences_map[city];
			
			//Calculate the average
			float avg_radiation = sum_radiation/ocurrences;
			std::cout<<city<<"\t"<<avg_radiation<<"\n";
		}
	}
	
	
	int main(void){	
		//set locale to spanish. Necessary to read decimal point correctly
		std::setlocale(LC_ALL, "es_ES.UTF-8");
	
		//read file from standard input
		std::istream* std_in = &std::cin;
		calculate_avg(*std_in);
		
		return 0;
	}
	
#### Implementación de la segunda tarea

La segunda tarea está implementada en las tareas `map_rain.cpp` y `reducer_rain.cpp`.

##### Mapper

El *mapper* está implementado en el fichero `map_rain.cpp`, y se compone de una parte de lectura del fichero de salida de la tarea anterior y búsqueda del municipio de mayor radiación; y otra parte de lectura del csv desde la entrada estándar y filtrado de los campos  *FECHA* y *PRECIPITACIONES* de dicho municipio.

- **Lectura de la salida de la tarea anterior**

	Aunque Hadoop Streaming permite el envío de dos ficheros en su opción `-input`, la coordinación de la lectura de ambos ficheros puede ser complicada. Por esta razón, en nuestra implementación, se ha decidido acceder a dicho fichero directamente mediante su ruta, en lugar de utilizar la entrada estándar junto al csv.
	  
	La salida de la tarea anterior se almacena en la ruta `output-radiation/part-00000`. Para acceder a este fichero, utilizaremos la clase `std::fstream`. Abrimos el fichero en modo lectura, indicando en el tercer parámetro `std::fstream::in`. 
	
	  //Read results from previous job
	  std::fstream output_radiation("output_radiation/part-00000", std::fstream::in);	  		
	  
- 	**Búsqueda de la ciudad con mayor radiación**

	Una vez creado el flujo de datos del fichero, utilizamos la función `calculate_max_radiation()` para obtener la ciudad con mayor radiación.   
	  
	  Esta función irá leyendo el fichero línea a línea, separando las claves y valores (ciudad, radiación), y comparando sus valores de radiación para obtener aquella con mayor valor. Para recorrer el fichero, volveremos a utilizar la función `std::getline(fichero, linea)`, tal y como se ha hecho en los ficheros anteriores.
	  
	  Para realizar la comparación de la ciudad actual con la de mayor radiación, utilizaremos dos pares (ciudad, radiación): uno que almacenará la ciudad actual con su radiación, y otro el de la ciudad con mayor radiación hasta el momento. 
	 	
	 	//Split the line in key and values
		std::pair<std::string,float> city_radiation = get_key_value(line);
		
		//Initialize city_max_rad to the first city of the file
	 	std::pair<std::string, float> city_max_rad = city_radiation;
			  
	Hecho esto, arrancamos el bucle de lectura para leer el resto del fichero. Creamos las variable temporales `radiation` y `map_rad` para almacenar el valor de radiación de la ciudad actual y el máximo actual, con el fin de mejorar la legibilidad. 
	
		//Read file line to line
		while(std::getline(file, line)){
			//Split the line in key and values
			std::pair<std::string,float> city_radiation = get_key_value(line);
			
			//extract key and value
			float radiation = city_radiation.second;
			float max_rad = city_max_rad.second;
			
			//Update max radiation
			if(radiation > max_rad){
				city_max_rad = city_radiation;
			}
		}

	  Una vez calculemos el mayor, devolveremos su nombre mediante el atributo `first` de `city_max_rad`.  
	    
	   	return city_max_rad.first;

- **Preparación del entorno para el *mapper* del csv**  

	Una vez calculado el mayor, preparamos el entorno para la ejecución del *mapper* propiamente dicho. 
	El *mapper* recibirá el fichero csv desde la entrada estándar, y filtrará los campos *FECHA* y *PRECIPITACIONES* de la ciudad de mayor radiación.
	
	Esto se implementa en la función `filter_rain_csv()`, que recibirá de parámetros el flujo de datos de la entrada estándar, y el nombre de la ciudad de mayor radiación.
	
	La preparación es similar a la de los ficheros anteriores:
	
		//Get the name of the city with higher radiation
		std::string city_max_rad = calculate_max_radiation(output_radiation);
		
		//set locale to spanish. Necessary to read decimal separator correctly
	    std::setlocale(LC_ALL, "es_ES.UTF-8");
		
		//read csv file from standard input
		std::istream* std_in = &std::cin;
		
		//Call to mapper function, passing the city with highest radiation by parameter
		filter_rain_csv(*std_in, city_max_rad);
		
	En este caso, la configuración de las locales se realiza después de la lectura del fichero de salida de la primera tarea, debido a que este fichero de salida usa el punto como separador decimal.
		
- **Filtrado de las precipitaciones y fechas de la ciudad con mayor radiación**  

	Una vez encontrado el nombre de la ciudad con mayor radiación, filtramos del fichero csv la cantidad de precipitaciones, y el año de cada una de ellas.     
	  
	 Para ello, vamos recorriendo el flujo de datos del fichero, y separando sus campos mediante la función `split()`. Los campos ya separados los guardaremos en un vector, de forma similar a la primera tarea:
	   
		//Read file line to line
		while(std::getline(file, line)){
			std::vector<std::string> row;	
			
			//split the line by columns
			split(line, row, ';');
			
			//Filter the rain data of the city which higher radiation (passed by parameter)
			std::string city = row[CITY]; 
	    
	 Si la ciudad obtenida es la que queremos filtrar, buscamos sus campos de año y precipitaciones.
	      
	 El campo *PRECIPITACIONES* puede estar vacío en algunos casos, lo cual nos daría problemas para procesar sus valores posteriormente. Para resolverlo, llamamos a la función `fix_empty_values()` que, en caso de que la cadena de ese campo este vacía, nos la rellena con el caracter cero.  
	      
	 Resuelto este problema, obtenemos el valor del año. Dado que el fichero csv no tiene un campo AÑO, tendremos que filtrar ese valor desde el campo *FECHA*. 
	 Para ello, asumimos que el año va a estar siempre en los últimos 4 caracteres del campo *FECHA*, y extraemos esa subcadena mediante el método `substr()` de la clase `std::string`.  
	      
		 //Extract year from date
		 std::string date = row[DATE];
	     std::string year = date.substr(date.size()-4);    
	     
	 
	 Una vez extraídos ambos datos, los sacamos por la salida estándar, usando como clave el año, y como valor las precipitaciones.  
	   
	   //Show values by screen
	   std::cout<<year<<"\t"<<rain_str<<"\n";
	   
El código completo del fichero es este:

	#include <string>
	#include <vector>
	#include <utility>
	#include <iostream>
	#include <locale>
	#include <fstream>
	
	#include "common.hpp"
	
	using vector_data = std::vector<std::pair<std::string, float> >;
	
	std::string calculate_max_radiation(std::istream& file){
	 	std::string line;
	 	
	 	//Store the current maximum
	 	std::pair<std::string, float> city_max_rad;
	 	
	 	//Read file line to line
		while(std::getline(file, line)){
			//Split the line in key and values
			std::pair<std::string,float> city_radiation = get_key_value(line);
			
			//extract key and value
			float radiation = city_radiation.second;
			float max_rad = city_max_rad.second;
			
			//Update max radiation
			if(radiation > max_rad){
				city_max_rad = city_radiation;
			}
		}
		
		//Return the name of the city with highest radiation
		return city_max_rad.first;
	}
	
	
	void filter_rain_csv(std::istream& file, std::string city_max_rad){	
	 	std::string line;
	 	
	 	//Read file line to line
		while(std::getline(file, line)){
			std::vector<std::string> row;	
			
			//split the line by columns
			split(line, row, ';');
			
			//Filter the rain data of the city which higher radiation (passed by parameter)
			std::string city = row[CITY];
				
			if(city == city_max_rad){
				//Replace empty rain values with zero
				std::string rain_str = fix_empty_values(row[RAIN]);
				
				//Extract year from date
				std::string date = row[DATE];
	    			std::string year = date.substr(date.size()-4);
				
				//Show values by screen
				std::cout<<year<<"\t"<<rain_str<<"\n";
			}	
		}
	}
	
	int main(void){
		//Read results from previous job
		std::fstream output_radiation("output_radiation/part-00000", std::fstream::in);
		
		//Get the name of the city with higher radiation
		std::string city_max_rad = calculate_max_radiation(output_radiation);
		
		//set locale to spanish. Necessary to read decimal separator correctly
		std::setlocale(LC_ALL, "es_ES.UTF-8");
		
		//read csv file from standard input
		std::istream* std_in = &std::cin;
		
		//Call to mapper function, passing the city with highest radiation by parameter
		filter_rain_csv(*std_in, city_max_rad);
		
		return 0;
	}

##### Reducer

El *reducer* se encuentra implementado en el fichero `reducer_rain.cpp`. Este recibe de su entrada estándar el conjunto de pares filtrado por el *mapper*, el cual se envía en forma de cadenas de caracteres separando la clave del valor mediante tabulaciones.

- **Preparación del entorno**

	La preparación del entorno es similar a la de los ficheros anteriores. En este fichero, volvemos a configurar las locales en español para corregir el separador decimal.
	
	En este caso, los datos de las precipitaciones por año se devolverán en una tabla hash indexada por año, en lugar de mostrarse directamente por pantalla.
	
		//set locale to spanish. Necessary to read decimal separator correctly
		std::setlocale(LC_ALL, "es_ES.UTF-8");
	
		//read file from standard input
		std::istream* std_in = &std::cin;
		std::unordered_map<int, float> year_rain = sum_rain(*std_in);
		
	Esta tabla se recorrerá en la función principal (`main()`) para mostrar sus datos en forma de clave-valor.  
    
- **Calculo de las precipitaciones totales por año**

	El cálculo de las precipitaciones totales de cada año se realizará en la función `sum_rain()`. Esta función recibirá por parámetro el descriptor del flujo de datos de la entrada estándar.
	
	Para dicho cálculo, se usará una estrategia similar a la utilizada en el cálculo de la media de radiaciones. 
	
	En este caso, en lugar de una tabla hash, utilizaremos un diccionario (`std::map`), indexado por año; para asegurar que los totales de precipitaciones se muestren ordenados según su año.
	
	En el diccionario, a medida que vayamos leyendo los pares-clave valor (año, precipitaciones), iremos sumando el valor de precipitaciones al de su respectivo año.
	
	El código de la función es el siguiente:
	
		std::map<std::string, float> sum_rain(std::istream& file){
			std::map<std::string, float> year_rain;
			
		 	std::string line;
		 	
		 	//Read file line to line
			while(std::getline(file, line)){
			
				//Split the line in key and values
				std::pair<std::string, float> values = get_key_value(line);
				
				//extract key and value
				std::string year = values.first;
				float rain = values.second;
				
				//Check if the key exists in the unordered_map
				std::map<std::string, float>::iterator it_year = year_rain.find(year);
				
				//If exists, sum the new value
				if(it_year != year_rain.end()){
					it_year->second += rain;
				}
				//If not exists, add the pair to the unordered_map
				else{
					year_rain[year] = rain;
				}
			}
			
			return year_rain;
		}
		
- **Salida de los resultados**

	La función anterior devuelve una tabla hash indexada por año, con el total de precipitaciones como valor.
	
	Pero, para que Hadoop lo muestre en el fichero de salida, es necesario sacar los pares clave-valor por la entrada estándar. Para ello, en la función `main()` añadiremos un bucle que vaya recorriendo el diccionario mostrando cada clave y valor separados por tabuladores.
	
	La estructura será similar a las utilizadas en los ficheros anteriores.
	
	   //write results to standard output
		for (auto& map: year_rain)
		{
			std::string year = map.first;
			float rain = map.second;
			
			std::cout<<year<<"\t"<<rain<<"\n";
		}
		
El código completo del fichero es el siguiente:

	#include <string>
	#include <utility>
	#include <iostream>
	#include <map>
	#include <algorithm>
	#include <vector>
	
	#include "common.hpp"
	
	std::map<std::string, float> sum_rain(std::istream& file){
		std::map<std::string, float> year_rain;
		
	 	std::string line;
	 	
	 	//Read file line to line
		while(std::getline(file, line)){
		
			//Split the line in key and values
			std::pair<std::string, float> values = get_key_value(line);
			
			//extract key and value
			std::string year = values.first;
			float rain = values.second;
			
			//Check if the key exists in the unordered_map
			std::map<std::string, float>::iterator it_year = year_rain.find(year);
			
			//If exists, sum the new value
			if(it_year != year_rain.end()){
				it_year->second += rain;
			}
			//If not exists, add the pair to the unordered_map
			else{
				year_rain[year] = rain;
			}
		}
		
		return year_rain;
	}
	
	
	int main(void){	
		//set locale to spanish. Necessary to read decimal separator correctly
		std::setlocale(LC_ALL, "es_ES.UTF-8");
	
		//read file from standard input
		std::istream* std_in = &std::cin;
		std::map<std::string, float> year_rain = sum_rain(*std_in);
		
		//write results to standard output
		for (auto& map: year_rain)
		{
			std::string year = map.first;
			float rain = map.second;
			
			std::cout<<year<<"\t"<<rain<<"\n";
		}
		
		return 0;
	}

## Preparación del entorno Hadoop

Una vez programado nuestro MapReduce en ambas tareas, realizamos la instalación y configuración de Hadoop para probar nuestro proyecto

### Especificaciones del sistema

- **Sistema Operativo:** Debian GNU/Linux 11 (bullseye)
- **Versión del lenguaje:** C++14
- **Compilador:** g++ (GCC) 10.2.0
- **Versión de Java:** OpenJDK 8 (Java 1.8.0)
- **Versión de Hadoop:** Hadoop 2.10.1
	- **Configuraciones probadas:** *Standalone* y *Pseudodistributed cluster*

### Especificaciones hardware

- **Thinkpad T440p**
	- Procesador Intel i7 4702MQ
	- 16 GB RAM 1600MHz
	- Disco duro SSD Samsung 860 EVO 500 GB

#### Instalación de Hadoop

En esta sección detallaremos los pasos seguidos para la instalación y configuración de Hadoop en nuestro sistema

- **Instalación de la JDK de Java**

	Instalamos la JDK de la versión 8 de Java con el comando
	
		sudo apt install openjdk-8-jdk
		
	Y configuramos dicha versión para que se utilice por defecto. Ejecutamos este comando:
	
		sudo update-alternatives --config java
		
	Se nos mostrará una pantalla similar a esta:
	
		almu@debian:~/Practicas_BigData/Practica1$ sudo update-alternatives --config java
		Existen 2 opciones para la alternativa java (que provee /usr/bin/java).
		
		  Selección   Ruta                                            Prioridad  Estado
		------------------------------------------------------------
		  0            /usr/lib/jvm/java-11-openjdk-amd64/bin/java      1111      modo automático
		  1            /usr/lib/jvm/java-11-openjdk-amd64/bin/java      1111      modo manual
		* 2            /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java   1081      modo manual
		
		Pulse <Intro> para mantener el valor por omisión [*] o pulse un número de selección: 
	
	
	Buscamos la opción correspondiente a Java 8, introducimos el número de su campo "Selección" y pulsamos Enter para configurarlo
		
	Repetimos el proceso con `javac`
	
		sudo update-alternatives --config javac


	Con esto ya tendremos configurada la JVM de Java 8 por defecto para nuestro sistema.

- **Descarga de Hadoop**

	Descargamos la última versión (2.10.1) de la página oficial. Podemos utilizar este servidor espejo para la descarga:
	  
	https://mirror.koddos.net/apache/hadoop/common/hadoop-2.10.1/hadoop-2.10.1.tar.gz 

	Lo descargamos y extraemos el directorio
	
		wget https://mirror.koddos.net/apache/hadoop/common/hadoop-2.10.1/hadoop-2.10.1.tar.gz
		tar -xvf hadoop-2.10.1.tar.gz
		  
	Esto nos descomprimirá el directorio con los binarios y configuraciones de Hadoop en nuestro directorio actual

- **Instalación de Hadoop**

	Para instalarlo, copiamos el directorio de Hadoop a otra ubicación fuera de nuestro directorio personal.
	En nuestro caso, vamos a copiar el directorio `/usr/local/hadoop` (reemplazaremos el nombre del directorio extraido por el de hadoop)
	
		sudo cp hadoop-2.10.1 /usr/local/hadoop
		
	Para no tener que invocar la ruta completa de Hadoop en cada ejecución, añadimos su directorio `bin/`al PATH.
	También definiremos algunas variables necesarias para el funcionamiento de Hadoop
	
	Editamos nuestro fichero `.bashrc` para añadir el directorio
	
		nano ~/.bashrc
		
	Y añadimos las siguientes líneas al final del fichero:
	
		export JAVA_HOME="/usr"
		export HADOOP_HOME="/usr/local/hadoop"
		export HADOOP_CONF_DIR=$HADOOP_HOME/etc/hadoop
		export HADOOP_MAPRED_HOME=$HADOOP_HOME
		export HADOOP_COMMON_HOME=$HADOOP_HOME
		export HADOOP_HDFS_HOME=$HADOOP_HOME
		export YARN_HOME=$HADOOP_HOME
		export PATH=$PATH:$HADOOP_HOME/bin
		
		export JAVA_HOME="/usr/lib/jvm/java-1.8.0-openjdk-amd64"
		export PATH=$PATH:$JAVA_HOME/bin

	Recargamos la configuración de nuestra terminal
	
		. .bashrc
		
	Y comprobamos si todo ha ido correctamente ejecutando
	
		hadoop version
		
	Que nos deberá mostrar algo similar a esto
	
		Hadoop 2.10.1
		Subversion https://github.com/apache/hadoop -r 1827467c9a56f133025f28557bfc2c562d78e816
		Compiled by centos on 2020-09-14T13:17Z
		Compiled with protoc 2.5.0
		From source with checksum 3114edef868f1f3824e7d0f68be03650
		This command was run using /usr/local/hadoop/share/hadoop/common/hadoop-common-2.10.1.jar
		
	En caso de que nos salga un error indicando que la variable `JAVA_HOME` no está definida
	
    	ERROR : JAVA_HOME is not set

	lo redefinimos en la configuración de hadoop, en `/usr/local/hadoop/etc/hadoop/hadoop-env.sh`.  
	Editamos el fichero
	
		sudo nano $HADOOP_CONF_DIR/hadoop-env-sh
		
	Y modificamos la línea donde se define la variable `JAVA_HOME` de esta manera
	
		export JAVA_HOME="/usr/lib/jvm/java-1.8.0-openjdk-amd64"
		
	Guardamos y cerramos
	
	Esta vez, al volver a ejecutar el comando `hadoop`, este debería funcionar sin problemas
	
	Esta instalación utilizará el modo *standalone* de Hadoop, lo cual será suficiente para comprobar el funcionamiento de nuestro proyecto

## Ejecución 

### Compilación del código

Dado que C++ es un lenguaje compilado, antes de empezar hemos de compilar los fuentes.

Para compilar el código, usaremos g++. Junto al código, se provee un Makefile para facilitar la compilación.
En el Makefile se activa la opción del compilador `-O2` para optimizar el código durante la compilación.

Para compilar el código, nos situamos en el directorio donde se encuentran los fuentes, y ejecutamos

	make

Si todo ha ido bien, aparecerá algo similar a esto

	almu@debian:~/Practicas_BigData/Practica1$ make
	g++ map_radiation.cpp -O2 -o map_radiation
	g++ reduce_radiation.cpp -O2 -o reduce_radiation
	g++ map_rain.cpp -O2 -o map_rain
	g++ reducer_rain.cpp -O2 -o reducer_rain


Si no tenemos `make` instalado, podemos instalarlo mediante el paquete `build-essential`

	sudo apt install build-essential

Y volvemos a ejecutar `make` para compilar los fuentes

Con esto deberían aparecer 4 nuevos ficheros en nuestro directorio: `map_radiation`, `reduce_radiation`, `map_rain` y `reducer_rain`


### Prueba sobre Hadoop en modo *standalone*

Para probar el código, usaremos la configuración *standalone* por defecto. 


#### Probando el código

Una vez con Hadoop instalado y los binarios ya compilados, probamos el código desde Hadoop

Para esto usaremos el script `hadoop_test_stand.sh`, que contiene las siguientes líneas

	rm -rf output_radiation output_rain

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

Hadoop requiere que los directorios de salida no existan previamente. Por esa razón, utilizamos el comando `rm` en nuestro script para borrarlos antes de cada ejecución.

Ejecutamos el script con

	sh hadoop_test_stand.sh

Si todo va bien, veremos en la consola algo parecido a esto  

		File System Counters
			FILE: Number of bytes read=12595506
			FILE: Number of bytes written=1391782
			FILE: Number of read operations=0
			FILE: Number of large read operations=0
			FILE: Number of write operations=0
		Map-Reduce Framework
			Map input records=54636
			Map output records=4842
			Map output bytes=37141
			Map output materialized bytes=46831
			Input split bytes=154
			Combine input records=0
			Combine output records=0
			Reduce input groups=15
			Reduce shuffle bytes=46831
			Reduce input records=4842
			Reduce output records=15
			Spilled Records=9684
			Shuffled Maps =1
			Failed Shuffles=0
			Merged Map outputs=1
			GC time elapsed (ms)=0
			Total committed heap usage (bytes)=499122176
		Shuffle Errors
			BAD_ID=0
			CONNECTION=0
			IO_ERROR=0
			WRONG_LENGTH=0
			WRONG_MAP=0
			WRONG_REDUCE=0
		File Input Format Counters 
			Bytes Read=6115588
		File Output Format Counters 
			Bytes Written=177
	20/12/11 21:46:24 INFO streaming.StreamJob: Output directory: output_rain
	
Y en nuestro directorio tendremos dos nuevos subdirectorios llamados `output_radiation` y `output_rain`

En el fichero `output_radiation/part-00000`, podremos ver la salida de la primera tarea:

	"Moguer"	18.3416
	"Lepe"	18.8797
	"La Puebla de Guzmán"	17.9628
	"La Palma del Condado"	18.0714
	"Gibraleón"	18.7095
	"Niebla"	18.0787
	"Aroche"	17.9192
	"El Campillo"	18.2613
	"Almonte"	18.1239
	
En el fichero `output_rain/part-00000` vemos la salida de la segunda tarea

	1999	8.6
	2000	639.8
	2001	340
	2002	570.8
	2003	877.8
	2004	445.6
	2005	332.4
	2006	613.4
	2007	439
	2008	576.2
	2009	561.2
	2010	883
	2011	479.4
	2012	430.4
	2013	342.4
	2014	62.2
	
Vemos que, en ambos casos, la salida obtenida es la esperada.

##### Medición de tiempos

Modificamos el script para añadir un cálculo de los tiempos de ejecución. Dado que los accesos a disco pueden ser lentos, ignoraremos el borrado de los directorios en el tiempo total, colocando las marcas de tiempo justo antes y justo después de la ejecución de cada tarea.

	//Remove previous output directories
	rm -rf output_radiation output_rain
	
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

Volvemos a ejecutar el script

	sh hadoot_standalone.sh
	
Tras terminar la ejecución, veremos una línea similar a esta, indicando el tiempo utilizado en su ejecución

	Total time: 5 scs

En este caso, la ejecución ha tardado 5 segundos.

## Configuración de Hadoop en modo *Pseudo-distributed node*

Una vez probada la ejecución de nuestro proyecto en modo *standalone*, vamos a intentar configurar Hadoop en modo *pseudo-distributed node*, para repetir la ejecución en un entorno mas real.

En el modo *pseudo-distributed node*, se aprovechan los núcleos del procesador para simular un cluster, distribuyendo la ejecución entre ellos.

Para la configuración, nos basaremos en este tutorial, adaptando las rutas y modificando algunas configuraciones
https://hectorsampallo.com/instalar-hadoop-2-en-debian-10/

### Copiando la anterior instalación de Hadoop

Si no queremos perder la instalación en modo *standalone*, podemos copiarla en otro directorio para preservarla.
En mi caso, la he copiado en `/usr/local/hadoop_std`

	sudo cp /usr/local/hadoop /usr/local/hadoop_std

### Configuración de ssh

Las peticiones de Hadoop se envían a través de ssh. Para ello, es necesario que ssh esté configurado para acceder a nuestro usuario sin requerir contraseña. Esto lo conseguimos configurando una clave pública asociada a nuestro usuario, y guardando la misma dentro de nuestra configuración de ssh.

Esto se realiza con los siguientes comandos:

	ssh-keygen -t rsa -P '' -f ~/.ssh/id_rsa
	cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
	chmod 0600 ~/.ssh/authorized_keys

Comprobamos que el acceso se ha configurado correctamente accediendo a nuestro usuario desde nuestra máquina

	ssh localhost

Si todo ha ido bien, se abrirá un acceso desde terminal de una forma similar a esta:

	almu@debian:~$ ssh localhost
	Linux debian 5.9.0-4-amd64 #1 SMP Debian 5.9.11-1 (2020-11-27) x86_64
	
	The programs included with the Debian GNU/Linux system are free software;
	the exact distribution terms for each program are described in the
	individual files in /usr/share/doc/*/copyright.
	
	Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent
	permitted by applicable law.
	Last login: Wed Dec  9 01:49:45 2020 from ::1
	almu@debian:~$ 

Escribimos `exit` para salir

	almu@debian:~$ exit
	cerrar sesión
	Connection to localhost closed.
	almu@debian:~$ 

Comprobamos que el acceso abierto anteriormente usaba ssh

### Desactivando Hosts

Algunas distribuciones configuran direcciones de host por defecto. Estas direcciones pueden chocar con la configuración de Hadoop, por lo que es recomendable desactivarlas.

Abrimos el fichero `/etc/hosts`, y comentamos las líneas relativas a "127.0.0.1" y "127.0.1.1"

	sudo nano /etc/hosts

Las líneas debe quedar similares a estas:

	#127.0.0.1      localhost.localdomain localhost
	#127.0.1.1      debian

### Configuración de Hadoop

- **Configuración del acceso HDFS**

	Editamos el archivo `$HADOOP_HOME/etc/hadoop/core-site.xml` para configurar la conexión
	
		sudo nano $HADOOP_HOME/etc/hadoop/core-site.xml
	
	Añadimos estas líneas al final del fichero
	
		<configuration>
		    <property>
		        <name>fs.default.name</name>
		        <value>hdfs://0.0.0.0:9000</value>
		    </property>
		</configuration>

- __Configuración de los directorios para el *namenode* y *datanode*__

	Editamos el archivo $HADOOP_HOME/etc/hadoop/hdfs-site.xml, se configuran los directorios para el namenode y datanode.
	
		<configuration>
		    <property>
		        <name>dfs.replication</name>
		        <value>1</value>
		    </property>
		    <property>
		        <name>dfs.namenode.name.dir</name>
		        <value>/usr/local/hadoop/hdfs/namenode</value>
		    </property>
		    <property>
		        <name>dfs.datanode.data.dir</name>
		        <value>/usr/local/hadoop/hdfs/datanode</value>
		    </property>
		</configuration>
	
	El namenode se ubicará en `/usr/local/hadoop/hdfs/namenode` y el datanode en  `/usr/local/hadoop/hdfs/datanode `
	
	Creamos los directorios y los asignamos a nuestro usuario
	
	  sudo mkdir -p /usr/local/hadoop/hdfs/{namenode, datanode}
	  sudo chown -R [nuestro_usuario] -R /usr/local/hadoop/hdfs/*
	
	Esto nos permitirá ejecutar Hadoop sin requerir permisos de superusuario
	
- **Directorios de log**

	También nos creamos directorios para los logs.
		
	  sudo mkdir /usr/local/hadoop/logs
			
	Y los asignamos a nuestro usuario
	
	  sudo chown -R [nuestro_usuario] -R /usr/local/hadoop/logs
	
- **Configuración de Yarn**

	Editamos el archivo de configuración `$HADOOP_HOME/etc/hadoop/`
	
	  sudo nano "$HADOOP_HOME/etc/hadoop/yarn-site.xml"
	  
	Añadimos esto al final
	
		<configuration>
		    <property>
		        <name>yarn.nodemanager.aux-services</name>
		        <value>mapreduce_shuffle</value>
		    </property>
		</configuration>
			
### Arrancando los servicios

- **Arrancando los nodos**

	Arrancamos los nodos con 
	
	  $HADOOP_HOME/sbin/start-dfs.sh
	  
	Si todo ha ido bien, veremos algo parecido a esto
	
		almu@debian:~$ $HADOOP_HOME/sbin/start-dfs.sh
		Starting namenodes on [0.0.0.0]
		0.0.0.0: starting namenode, logging to /usr/local/hadoop/logs/hadoop-almu-namenode-debian.out
		localhost: starting datanode, logging to /usr/local/hadoop/logs/hadoop-almu-datanode-debian.out
		Starting secondary namenodes [0.0.0.0]
		0.0.0.0: starting secondarynamenode, logging to /usr/local/hadoop/logs/hadoop-almu-secondarynamenode-debian.out
		
	Comprobamos que los nodos han arrancado usando el comando `jps`
	
		almu@debian:~$ jps
		19970 NameNode
		20108 DataNode
		20286 SecondaryNameNode
		20703 Jps
		
	Vemos que los nodos están en pie, y el PID asignado a sus procesos
	
- **Arrancando Yarn**

	Arrancamos Yarn con
	
		$HADOOP_HOME/sbin/start-yarn.sh
	
	Veremos algo similar a esto:
	
		almu@debian:~$ $HADOOP_HOME/sbin/start-yarn.sh
		starting yarn daemons
		starting resourcemanager, logging to /usr/local/hadoop/logs/yarn-almu-resourcemanager-debian.out
		localhost: starting nodemanager, logging to /usr/local/hadoop/logs/yarn-almu-nodemanager-debian.out

	
	Si todo ha ido bien, podremos acceder al portal de administración desde el navegador
	
		http://localhost:8088/cluster
	
	![](Screenshot_2020-12-12 All Applications.png)
	
	
### Creando directorios en HDFS

Una vez arrancados los servicios, los nodos de Hadoop están listos para funcionar.

Pero, en la configuración actual, Hadoop no es capaz de acceder a los ficheros de entrada y salida ubicados en nuestro sistema de ficheros local. Para poder acceder a estos, es necesario copiarlos dentro del HDFS.

Pero antes, debemos crear el directorio principal dentro del HDFS. Esto lo hacemos con el comando

	hadoop fs -mkdir hdfs://0.0.0.0:9000/user/[nuestro_usuario]	

Esto nos creará un directorio llamado `/user/[nuestro_usuario]` dentro del HDFS. Este será nuestro directorio raíz. 

### Compartiendo datos entre HDFS y nuestro sistema de ficheros local

- **Copiando los datos de nuestro directorio a HDFS**

	Una vez creado el directorio raíz de HDFS, copiamos los datos de nuestro directorio actual al HDFS.
	En mi caso, he optado por copiar el directorio completo
	
	Para ello usamos el comando
	
		hadoop fs -copyFromLocal [mi_directorio]
		
	En mi caso, el directorio se llama `Practica1`, así que el comando quedaría de la siguiente manera
	
		hadoop fs -copyFromLocal Practica1
	
	Esto nos creará un directorio llamado `Practica1` dentro del directorio raíz del HDFS, con los datos de nuestro directorio. La ruta completa sería:
	
		hdfs://0.0.0.0:9000/user/[nuestro_usuario]/Practica1

- **Copiando datos desde HDFS a nuestro directorio**

	Después de probar el proyecto, Hadoop guardará los datos de salida dentro del HDFS.	
	Para copiar estos datos a nuestro directorio, utilizaremos el comando:
		
	  hadoop fs -copyToLocal hdfs://0.0.0.0:9000/user/[nuestro_usuario]/[mi_directorio]/[fichero] [ruta_directorio_local]
			
	En nuestro caso, sería algo parecido a esto:
		 
	  hadoop fs -copyToLocal hdfs://0.0.0.0:9000/user/[nuestro_usuario]/Practica1/output* 
     
### Probando nuestro proyecto en Hadoop en modo *pseudo-distributed*

Una vez reconfigurado Hadoop en modo *pseudo-distributed* y creado el HDFS con los datos de nuestro proyecto, probamos a ejecutar nuestras tareas en este modo.

#### Modificando el script

Para ello, debemos modificar el script anterior, añadiendo las rutas de nuestro HDFS en los campos `-input` y `-output`. También añadimos mas marcas de tiempo, para separar el tiempo de transferencia de los ficheros del de la ejecución de la tarea.

Reemplazamos en la variable `USER` el nombre de nuestro usuario,

El script quedará similar a este (almacenado en `hadoop_test.sh`):

	USER="almu"
	HADOOP_DIR="hdfs://0.0.0.0:9000/user/$USER"
	
	hadoop fs -copyFromLocal .
	
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
	
#### 	Ejecutando el script

Ejecutamos el script con:

	sh hadoop_test.sh

Si todo ha ido bien, veremos en la consola algo similar a esto

	20/12/12 18:18:18 INFO mapreduce.Job: Counters: 35
		File System Counters
			FILE: Number of bytes read=364298
			FILE: Number of bytes written=1397913
			FILE: Number of read operations=0
			FILE: Number of large read operations=0
			FILE: Number of write operations=0
			HDFS: Number of bytes read=12231176
			HDFS: Number of bytes written=165
			HDFS: Number of read operations=13
			HDFS: Number of large read operations=0
			HDFS: Number of write operations=4
		Map-Reduce Framework
			Map input records=54636
			Map output records=4842
			Map output bytes=37141
			Map output materialized bytes=46831
			Input split bytes=141
			Combine input records=0
			Combine output records=0
			Reduce input groups=15
			Reduce shuffle bytes=46831
			Reduce input records=4842
			Reduce output records=15
			Spilled Records=9684
			Shuffled Maps =1
			Failed Shuffles=0
			Merged Map outputs=1
			GC time elapsed (ms)=5
			Total committed heap usage (bytes)=625999872
		Shuffle Errors
			BAD_ID=0
			CONNECTION=0
			IO_ERROR=0
			WRONG_LENGTH=0
			WRONG_MAP=0
			WRONG_REDUCE=0
		File Input Format Counters 
			Bytes Read=6115588
		File Output Format Counters 
			Bytes Written=165
	20/12/12 18:18:18 INFO streaming.StreamJob: Output directory: hdfs://0.0.0.0:9000/user/almu/output_rain
	Total time: 5 scs
	

Vemos que la ejecución ha tardado unos 5 segundos, un tiempo similar al de la ejecución en modo *standalone*.

Deberán aparecer en nuestro directorio dos subdirectorios: `output_radiation` y `output_rain`. En caso de existir, su contenido debería haberse actualizado con los resultados de las nuevas tareas.

Si abrimos el fichero `output_radiation/part-00000`, veremos la salida de la primera tarea:

	"Moguer"	18.3416
	"Lepe"	18.8797
	"La Puebla de Guzmán"	17.9628
	"La Palma del Condado"	18.0714
	"Gibraleón"	18.7095
	"Niebla"	18.0787
	"Aroche"	17.9192
	"El Campillo"	18.2613
	"Almonte"	18.1239
	
En el fichero `output_rain/part-00000` veremos la salida de la segunda tarea:

	1999	8.6
	2000	639.8
	2001	340
	2002	570.8
	2003	877.8
	2004	445.6
	2005	332.4
	2006	613.4
	2007	439
	2008	576.2
	2009	561.2
	2010	883
	2011	479.4
	2012	430.4
	2013	342.4
	2014	62.2

Vemos que los resultados son los mismos que en la ejecución anterior, sobre Hadoop *standalone*

### Volviendo a Hadoop *Standalone*

Si hicimos una copia de la instalación *standalone* de Hadoop antes de cambiar a la *pseudo-distributed*. podemos volver a ella redefiniendo la variable `HADOOP_HOME` en el fichero `~/.bashrc`.

La variable quedaría así:

	export HADOOP_HOME="/usr/local/hadoop_std"

Para poder realizar el cambio de forma automática, modificamos el script `hadoop_test_stand.sh` (ya modificado en el fichero proporcionado), añadiendo este bloque justo al principio

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
			export JAVA_HOME
		fi
	fi

Este bloque comprueba si disponemos de una copia *standalone* y, de existir, comprueba que la variable `HADOOP_HOME` esté configurada con dicha ruta. En caso contrario, reemplaza el contenido de la variable en el fichero `~/.bashrc`, redefiniendo la ruta para que apunte a la instalación *standalone.*

De esta forma, nos aseguramos de que el script use dicha instalación, en lugar de la *pseudo-distributed*

Si ejecutamos el script `hadoop_test_stand.sh`, este debería utilizar la instalación *standalone* de Hadoop, tomando los ficheros directamente de nuestro directorio, en lugar de utilizar el HDFS

Ejecutamos el script

	bash hadoop_test_stand.sh
	
Si todo ha ido bien, veremos al final de la consola algo similar a esto:

	20/12/12 20:46:48 INFO streaming.StreamJob: Output directory: output_rain
	Total time: 5 scs
	
### Volviendo a Hadoop *Pseudo-distributed*

Para volver al modo pseudodistribuido, aplicamos la misma modificación, en sentido inverso, en el script `hadoop_test.sh`

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

Si ejecutamos el script `hadoop_test.sh`, este se ejecutará con la instalación pseudodistribuida

	bash hadoop_test.sh

Si todo ha ido bien, veremos algo como esto

	20/12/12 20:49:55 INFO streaming.StreamJob: Output directory: hdfs://0.0.0.0:9000/user/almu/output_rain
	Total time: 6 scs


## Comparativa de tiempos

### Comparación entre modos de instalación

Tras realizar varias ejecuciones, tanto con Hadoop *Standalone* como con el modo pseudodistribuido, vemos que los tiempos de ejecución en ambos modos son muy similares, rondando los 6 segundos de media. Aunque, en algunas ocasiones, la instalación *standalone* obtiene tiempos mas reducidos que la pseudodistribuida.

Esto podría deberse a los tiempos de transferencia que requiere el HDFS, al utilizar una conexión mediante SSH; y que el modo *standalone* no requiere. Pese a todo, la diferencia de tiempos no es demasiado acentuada, siendo de apenas un par de segundos en el caso mas desfavorable.

Buscando información sobre esta cuestión, vemos que el problema ya está documentado en un artículo de 2016
http://www.beyond-storage.com/bigdata-investigation-10-using-hadoop-streaming-on-hadoop-cluster-in-pseudo-distributed-mode/

### Ejecución directa sobre la consola

Probando a ejecutar el proyecto directamente desde la consola, conectando los binarios mediante tuberías, vemos que los tiempos se reducen considerablemente respecto a los requeridos por Hadoop

	almu@debian:~/Practicas_BigData/Practica1$ time ./map_radiation < RIA_exportacion_datos_diarios_Huelva_20140206.csv | ./reduce_radiation > output_radiation.txt 
	
	real	0m0,205s
	user	0m0,235s
	sys	0m0,099s  
	
	almu@debian:~/Practicas_BigData/Practica1$ time ./map_rain < RIA_exportacion_datos_diarios_Huelva_20140206.csv | ./reducer_rain > output_rain.txt 
	
	real	0m0,146s
	user	0m0,155s
	sys	0m0,009s
	

Sumando los tiempos de ambas ejecuciones (el campo `real`), vemos que estos suman menos de un segundo entre los dos. Esto nos hace pensar que nuestra implementación no sea auténticamente paralelizable, o bien que sea la propia ejecución de Hadoop Streaming (un fichero jar de Java), el que produzca ese incremento de tiempos.

Aún así, el conjunto de datos es relativamente pequeño, por lo que no se podría considerar un buen entorno de pruebas para una ejecución paralela. Es posible que, a medida que los conjuntos de datos fueran aumentando, los tiempos entre ambos tipos de ejecución se fueran reduciendo.


## Conclusiones

Tras implementar este problema mediante una estrategia MapReduce, se puede afirmar que los tiempos se reducen respecto a una implementación tradicional iterativa. Un fichero de 50000 filas, que con otro tipo de planteamiento podría tardar cerca de 1 minuto en procesarse, con esta implementación se reduce a menos de 10 segundos.

Sin embargo, no se aprecian diferencias entre los dos modos de ejecución de Hadoop, manteniendo tiempos similares, que a su vez son mas altos que la ejecución directa sobre línea de comandos. Tampoco se aprecian diferencias en los resultados de las tareas. 

La configuración del entorno de Hadoop puede ser complicada. Algunas funcionalidades, como la del ResourceManager, no se han conseguido configurar. Y la instalación completamente distribuida, pese a varios intentos, tampoco ha llegado a funcionar. Pero, pese a todo, se ha logrado configurar y probar nuestro proyecto tanto en un entorno *standalone*, como en uno pseudodistribuido. Y configurar un HDFS de forma exitosa. La ejecución del proyecto ha sido exitosa en ambas ocasiones, obteniendo el resultado esperando en unos tiempos razonables.

La implementación mediante Hadoop Streaming, utilizando la entrada y salida estándar como medio de compartición de datos, podría haber ralentizado ligeramente la ejecución respecto a una implementación común utilizando las librerías de Hadoop dentro del propio proyecto. Aunque, comparando tiempos de otras implementaciones similares, la diferencia es bastante leve, no llegando a generar un problema grave. 

Con esto concluimos que Hadoop es una buena herramienta para realizar tareas de procesamiento en entornos distribuidos, aunque su configuración y puesta en marcha puede ser bastante problemática en ciertas ocasiones; y su eficiencia depende en bastante medida del tipo de implementación utilizada.


