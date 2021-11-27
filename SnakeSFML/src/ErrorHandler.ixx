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
 * \brief Plik modu�owy zawieraj�cy deklaracj� klasy ErrorHandler.
 */

/**
 * \class ErrorHandler
 * \brief Klasa obs�uguj�ca b��d window'sowy
 */

/**
 * \fn static void ErrorHandler::showErrorDialog()
 * \brief Metoda statyczna obs�uguj�ca b��d lub ostrze�enie.
 * Wy�wietla miejsce b��du oraz przekazany przez parametr komunikat.
 */