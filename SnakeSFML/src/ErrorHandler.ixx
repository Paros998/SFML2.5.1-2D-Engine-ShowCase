#include <iostream>
#include <string.h>
#include <Windows.h>
using namespace std;

export module ErrorHandler;

export class ErrorHandler {
	public:
		static void showErrorDialog(
			string message,
			string fileName,
			string iconType
		) {
			string formattedDialogHeader = "At file: " + fileName;
			int intIconType = iconType == "warning" ? MB_ICONWARNING : MB_ICONERROR;

			MessageBoxA(
				NULL,
				message.c_str(),
				formattedDialogHeader.c_str(),
				intIconType | MB_OK
			);
		}
};

/**
 * \file ErrorHandler.ixx
 * \brief Plik modu³owy zawieraj¹cy deklaracjê klasy ErrorHandler.
 */

/**
 * \class ErrorHandler
 * \brief Klasa obs³uguj¹ca b³¹d window'sowy
 */

/**
 * \fn static void ErrorHandler::showErrorDialog()
 * \brief Metoda statyczna obs³uguj¹ca b³¹d lub ostrze¿enie.
 * Wyœwietla miejsce b³êdu oraz przekazany przez parametr komunikat.
 */