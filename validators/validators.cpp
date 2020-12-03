#include "validators.h"

//::::::::::::::::::::::::::::::::::::::
//            VALIDADORES
//::::::::::::::::::::::::::::::::::::::

/**
  * Valida si numero esta en rango
  * 
  * @param int number el numero a comparar
  * @param int min minimo del rango
  * @param int max maximo del rango
  * @return bool :. el numero esta en rango
  */
bool isInRange(int number, int min, int max)
{
  return number >= min && number <= max;
}

/**
  * Valida si un string esta compuesta solo de numeros
  * 
  * @param string digits el string a comparar
  * @return bool :. el string esta compuesto solo de numeros
  */
bool isDigits(string str)
{
  return str.find_first_not_of("0123456789") == string::npos;
}

bool isFormatted(string str)
{
  return str.at(0) == '\033';
}

/**
  * Valida si in archivo se puede abrir y retorna el booleano equivalente
  * 
  * @param string filename el nombre del archivo
  * @return bool :. el archivo se puede abrir 
  */
bool isFile(string filename)
{
  bool valid = false;     //iniciamos esperando el fracaso
  fstream file(filename); //el archivo filename en buffer
  //si se puede abrir
  if ((valid = file.is_open()))
    //lo cierra para que no haya errores
    file.close();
  return valid; //retorna si el archivo se pude abrir
}

/**
  * Valida si es un alias valido
  *  
  * @param int min el tamaño minimo del alias
  * @param int max el tamaño maximo del alias
  * @return bool  :. el alias es valido 
  *
  */
bool isAlias(string alias, int min, int max)
{
  //valido solo si inicia con letra y si el tamaño es mayor o igual a min y menor o igual a max
  return isalpha(alias.at(0)) && isInRange(alias.length(), min, max);
}
