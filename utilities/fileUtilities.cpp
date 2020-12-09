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

    fc.lineCount = 0; //defualt de lineas leidas

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
    string str;                    //auxiliar linea a escribir;
    fstream file;                  //buffer
    file.open(filename, ios::app); //el archivo filename en buffer

    if (file.is_open())
    {
        for (int i = 0; i < content.lineCount; i++)
            file << content.content[i] << endl;
        file.close();
    }
}

/**
  * Lee el archivo de puntuajes usando readFile() << @see fileUtilities.h >> y
  * separa los datos usando splitString()  << @see utilities.h >> 
  * 
  * @param string filename el nombre del archivo
  * @return ScoreVector los datos del filename estructurados en ScoreVector 
  * 
  */
ScoreVector readScoreData(string filename)
{
    //score vector para cargar los datos en un struct
    ScoreVector scoreVector;

    //cargar los datos de filename
    StringVector scoreData = readFile(filename);

    //decir cuantas lineas e inicializar score
    scoreVector.itemCount = scoreData.lineCount;
    scoreVector.score = new Score[scoreData.lineCount];

    //pora cada linea leida en el txt
    for (int i = 0; i < scoreData.lineCount; i++)
    {
        //separar por _
        StringVector scoreToken = splitString("_", scoreData.content[i]);

        //cargar al struct de retorno
        scoreVector.score[i].name = scoreToken.content[0];
        scoreVector.score[i].gameType = scoreToken.content[1];
        scoreVector.score[i].score = scoreToken.content[2];
        scoreVector.score[i].timestamp = scoreToken.content[3];
    }

    //retornar los datos
    return scoreVector;
}

/**
  * Escribe en el archivo de puntuajes usando writeFile() << @see fileUtilities.h >>,
  * guarda los datos estructurados score separandolos por _
  * 
  * @param string filename el nombre del archivo
  * @return ScoreVector los datos del filename estructurados en ScoreVector 
  * 
  */
void writeScoreData(string filename, Score score)
{
    //el vector de escritura
    StringVector scoreData;

    //cuantas lineas a concatenar
    scoreData.lineCount = 1;

    //la linea a concatenar
    scoreData.content = new string[1];

    //guarda en la lina a concatenar los datos de score separando con _
    scoreData.content[0] = score.name + "_" + score.gameType + "_" + score.score + "_" + score.timestamp + "_";

    //escribe en filename
    writeFile(filename, scoreData);
}
