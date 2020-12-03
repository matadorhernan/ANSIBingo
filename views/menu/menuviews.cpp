#include "menuviews.h"

/**
  * Imprime una vista de menu principal
  * 
  * @param void
  * @return void
  */
void displayMenu(Menu menu)
{
  //limpia el body
  cleanFrameBody();

  //titulo
  gotoxy({6, 7});
  cout << menu.text;

  //menu
  for (int i = 0; i < menu.itemCount; i++)
  {
    gotoxy({6, 9 + i});
    cout << i + 1 << ") "
         << left << setw(32)
         << menu.items[i].text
         << colorANSI(
                ((menu.items[i].enabled) ? "[Hablitado]" : "[Deshablitado] "),
                0,
                ((menu.items[i].enabled) ? 10 : 9),
                256)
         << endl;
  }
}

/**
  * Obtiene y valida una opcion digito valida de un menu y la
  * devuelve como string
  * 
  * @param Menu menu
  * @return string opcion seleccionada
  */
string getQueryMenu(Menu menu)
{
  string selectedOption;
  bool validOption = false;

  while (!validOption)
  {

    //limpia el input
    cleanFrameInput();

    //imprimir query en consola
    gotoxy({6, 33});
    cout << menu.query;

    //obtener opcion
    getline(cin, selectedOption);

    //validar opcion
    validOption = !selectedOption.empty() &&                            //no puede estar vacia
                  isDigits(selectedOption) &&                           //tiene que ser digito
                  isInRange(stoi(selectedOption), 1, menu.itemCount) && //tiene que ser opcion en el menu
                  menu.items[stoi(selectedOption) - 1].enabled;         //tiene que ser una opcion habilitada

    //dar informacion al usuario
    if (!validOption)
    {
      gotoxy({6, 33});
      cout << colorANSI("Esta opcion puede estar deshabilitada o ser invalida, intentalo de nuevo...", 1, 9, 256);
      Sleep(3000);
    }
  }

  return selectedOption;
}
