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