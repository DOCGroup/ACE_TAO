// OCML_Utility.cpp: implementation of the OCML utility functions
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OCML_Utility.h"

#include "windows.h"
#include "commdlg.h"

#include <algorithm>

bool
save_file(const char* title, std::string& file_name)
{
	OPENFILENAME ofn;

	// allocate memory for the file name
	char *szFile = new char[MAX_PATH];
	strcpy(szFile, file_name.c_str());

	// Initialize the OPENFILENAME structure
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.lpstrFile = szFile;
	ofn.lpstrTitle = title;

	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = "All\0*.*\0"; // "All\0*.*\0Text\0*.txt\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.Flags = OFN_PATHMUSTEXIST;

	// GetSaveFileName() system call shows up the save file dialog
	bool result;
	if (result = GetSaveFileName(&ofn))
		file_name = szFile;
	delete[] szFile;
	return result;
}
