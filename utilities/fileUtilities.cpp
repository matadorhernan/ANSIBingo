#include "utilities.h"

/**
  * Lee toda la informacion en filename y la devuelve como un vector de string
  * siendo cada string una linea en el archivo filename.
  * 
  * @param string filename el nombre del archivo
  * @return FileContent contenido bajo estructura compuesta @see "structHeader.h" FileContent
  * 
  */
StringVector readFile(string filename)
{
    StringVector fc;
    string str;             //auxiliar y el contenido a retornar;
    fstream file(filename); //el archivo filename en buffer

    //si se puede abrir
    if (file.is_open())
    {
        //el primer pase cuenta las lineas
        int i = 0;
        while (getline(file, str))
            i++; //cuenta las lineas de filename
        fc.lineCount = i;

        //reset de failbit para segundo pase, para que lo lea bien
        file.clear();  //clear de errores
        file.seekg(0); //set del puntero a la pocision 0 de file

        //definicion dinamica de content de tamaño i
        fc.content = new string[i];

        //guardar contenido en segundo pase
        i = 0;
        while (getline(file, str))
        {
            fc.content[i] = str; //guarda cada linea str como cadena de string en content
            i++;
        }

        //lo cierra para que no haya errores
        file.close();
    }
    return fc;
}

/**
  * Guarda un arhivo, linea por linea usando fstream
  * 
  * @param string filename el nombre del archivo
  * @param FileContent contenido bajo estructura compuesta @see "structHeader.h" FileContent
  * @return void 
  * 
  */
void writeFile(string filename, StringVector content)
{
    string str;             //auxiliar linea a escribir;
    fstream file(filename); //el archivo filename en buffer
    if (file.is_open())
    {
        for (int i = 0; i < content.lineCount; i++)
            file << content.content[i] << endl;
        file.close();
    }
}

void writeScoreData(string filename, Player player)
{
}
