#include "SchemaDefines.h"
#include <stdlib.h>
#include <string>

#ifndef SCHEMAUTILS__H__
#define SCHEMAUTILS__H__

NSBEGIN

std::string intToStdString(const long&);
std::string doubleToStdString(const double&);
CString intToCString(const long&);
CString doubleToCString(const double&);

CStdioFile& operator<<(CStdioFile&,const char*);
CStdioFile& operator<<(CStdioFile&,const std::string&);
CStdioFile& operator<<(CStdioFile&,const CString&);
CStdioFile& operator<<(CStdioFile&,const int&);
CStdioFile& operator<<(CStdioFile&,const double&);

//const char* operator+(const char* a,const char* b);
std::string operator+(std::string,std::string);

NSEND

#endif
