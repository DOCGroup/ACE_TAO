// OCML_Utility.h: OCML utility functions
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OCML_UTILITY_H__3E6A46EA_BB50_4795_AC56_4E3899E9D2AA__INCLUDED_)
#define AFX_OCML_UTILITY_H__3E6A46EA_BB50_4795_AC56_4E3899E9D2AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

// maximum size of the file name.
const size_t MAX_FILE_NAME_SIZE = 256;

// Displays save file dialog. If cancelled returns "false", otherwise "true".
// File name stored in the file_name parameter.
bool save_file(const char* title, std::string& file_name);

#endif // !defined(AFX_OCML_UTILITY_H__3E6A46EA_BB50_4795_AC56_4E3899E9D2AA__INCLUDED_)
