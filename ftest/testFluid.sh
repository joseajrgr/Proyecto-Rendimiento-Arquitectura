#!/bin/bash

ruta_fluid=$1
ruta_ftest=$2

pruebas_pasadas=0

#Esta función va a comparar si el resultado obtenido tanto en fluid como en ftest es el esperado, validando esa ejecución
ejecutar_FLUID_cpp() {
    # Ejecutar el programa fluid con los argumentos proporcionados, valor de retorno en resultado
    $ruta_fluid "$1" "$2" "$3"
    resultado=$?

    if [ "$resultado" -eq "$4" ]; then
      if [ "$resultado" -eq 0 ]; then
          # Ejecutar el programa ftest con los argumentos proporcionados, valor de retorno en equal
          $ruta_ftest "$5" "$3" "1e-4"
          equal=$?
           if [ "$equal" -eq 0 ]; then
                ((pruebas_pasadas++))
                echo "Ejecución exitosa. Resultado: $equal"
           else
                echo "Error en el resultado: $equal"
           fi
      else
          ((pruebas_pasadas++))
          echo "Ejecución exitosa. Resultado: $resultado"
      fi
    else
        echo "Error en el resultado: $resultado"
    fi
}

#SIGNIFICADO SIGUIENTES LÍNEAS DE EJECUCIÓN: <función creada> <num iteraciones> <archivo lectura fld para fluid> <archivo salida fld del fluid> <valor de retorno esperado de fluid> <archivo referencia respectivo>

#test 1, el archivo de lectura tiene 0 partículas, por lo que debe retornar -5 (error 251)
ejecutar_FLUID_cpp 1 "./ftest/files_fld/NumberParticlesZero.fld" "./ftest/outfiles_fld/out-error.fld" 251 "./ftest/reference_files/small-1.fld"
#test 2, el archivo de lectura un numberParticles en la cabecera menor que el total de partículas en el cuerpo, por lo que debe retornar -5 (error 251)
ejecutar_FLUID_cpp 1 "./ftest/files_fld/BadParticlesLess.fld" "./ftest/outfiles_fld/out-error.fld" 251 "./ftest/reference_files/small-1.fld"
#test 3, el archivo de lectura un numberParticles en la cabecera menor que el total de partículas en el cuerpo, por lo que debe retornar -5 (error 251)
ejecutar_FLUID_cpp 1 "./ftest/files_fld/BadParticlesMore.fld" "./ftest/outfiles_fld/out-error.fld" 251 "./ftest/reference_files/small-1.fld"
#test 4, el número de iteraciones es inválido, por lo que debe retornar -2 (error 254)
ejecutar_FLUID_cpp -3 "./ftest/files_fld/small.fld" "./ftest/outfiles_fld/out-error.fld" 254 "./ftest/reference_files/small-1.fld"
#test 5, el número de iteraciones tiene formato inválido, por lo que debe retornar -1 (error 255)
ejecutar_FLUID_cpp "carolina" "./ftest/files_fld/small.fld" "./ftest/outfiles_fld/out-error.fld" 255 "./ftest/reference_files/small-1.fld"
#test 6, el archivo de lectura no existe, por lo que debe retornar -3 (error 253)
ejecutar_FLUID_cpp 1 "./ftest/files_fld/messi.fld" "./ftest/outfiles_fld/out-error.fld" 253 "./ftest/reference_files/small-1.fld"
#test 7, la ruta a el archivo de salida no existe, por lo que debe retornar -4 (error 252)
ejecutar_FLUID_cpp 1 "./ftest/files_fld/small.fld" "./ftest/outfiles_fld/wrong/messi.fld" 252 "./ftest/reference_files/small-1.fld"
#test 8, ejecución de fluid con archivo de lectura small y una iteración, se espera que funcione correctamente y retorne 0
ejecutar_FLUID_cpp 1 "./ftest/files_fld/small.fld" "./ftest/outfiles_fld/outS.fld" 0 "./ftest/reference_files/small-1.fld"
#test 9, ejecución de fluid con archivo de lectura small y dos iteraciones, se espera que funcione correctamente y retorne 0
ejecutar_FLUID_cpp 2 "./ftest/files_fld/small.fld" "./ftest/outfiles_fld/outS.fld" 0 "./ftest/reference_files/small-2.fld"
#test 10, ejecución de fluid con archivo de lectura small y tres iteraciones, se espera que funcione correctamente y retorne 0
ejecutar_FLUID_cpp 3 "./ftest/files_fld/small.fld" "./ftest/outfiles_fld/outS.fld" 0 "./ftest/reference_files/small-3.fld"
#test 11, ejecución de fluid con archivo de lectura small y cuatro iteraciones, se espera que funcione correctamente y retorne 0
ejecutar_FLUID_cpp 4 "./ftest/files_fld/small.fld" "./ftest/outfiles_fld/outS.fld" 0 "./ftest/reference_files/small-4.fld"
#test 12, ejecución de fluid con archivo de lectura small y cinco iteraciones, se espera que funcione correctamente y retorne 0
ejecutar_FLUID_cpp 5 "./ftest/files_fld/small.fld" "./ftest/outfiles_fld/outS.fld" 0 "./ftest/reference_files/small-5.fld"
#test 13, ejecución de fluid con archivo de lectura large y una iteración, se espera que funcione correctamente y retorne 0
ejecutar_FLUID_cpp 1 "./ftest/files_fld/large.fld" "./ftest/outfiles_fld/outL.fld" 0 "./ftest/reference_files/large-1.fld"
#test 14, ejecución de fluid con archivo de lectura large y dos iteraciones, se espera que funcione correctamente y retorne 0
ejecutar_FLUID_cpp 2 "./ftest/files_fld/large.fld" "./ftest/outfiles_fld/outL.fld" 0 "./ftest/reference_files/large-2.fld"
#test 15, ejecución de fluid con archivo de lectura large y tres iteraciones, se espera que funcione correctamente y retorne 0
ejecutar_FLUID_cpp 3 "./ftest/files_fld/large.fld" "./ftest/outfiles_fld/outL.fld" 0 "./ftest/reference_files/large-3.fld"
#test 16, ejecución de fluid con archivo de lectura large y cuatro iteraciones, se espera que funcione correctamente y retorne 0
ejecutar_FLUID_cpp 4 "./ftest/files_fld/large.fld" "./ftest/outfiles_fld/outL.fld" 0 "./ftest/reference_files/large-4.fld"
#test 17, ejecución de fluid con archivo de lectura large y cinco iteraciones, se espera que funcione correctamente y retorne 0
ejecutar_FLUID_cpp 5 "./ftest/files_fld/large.fld" "./ftest/outfiles_fld/outL.fld" 0 "./ftest/reference_files/large-5.fld"

#Se muestran los resultados de la ejecución de estos tests
echo "Resultados de los 17 tests:"
echo "Tests pasados: $pruebas_pasadas de 17"