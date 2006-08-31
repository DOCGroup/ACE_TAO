#include "SchemaUtils.h"

NSBEGIN

std::string intToStdString(const long& x)
{
	char numbuf[1024];
	memset(numbuf,'\x0',1024);
	_itoa(x,numbuf,10);
	std::string s(numbuf);
	return s;
}

CString intToCString(const long& x)
{
	char numbuf[1024];
	memset(numbuf,'\x0',1024);
	_itoa(x,numbuf,10);
	CString s(numbuf);
	return s;
}

std::string doubleToStdString(const double& x)
{
	char numbuf[1024];
	memset(numbuf,'\x0',1024);
	_gcvt(x,1023,numbuf);
	std::string s(numbuf);
	return s;
}

CString doubleToCString(const double& x)
{
	char numbuf[1024];
	memset(numbuf,'\x0',1024);
	_gcvt(x,1023,numbuf);
	CString s(numbuf);
	return s;
}

CStdioFile& operator<<(CStdioFile& ofile,const char* cptr)
{
	return ofile<<CString(cptr);
}

CStdioFile& operator<<(CStdioFile& ofile,const std::string& str)
{
	ofile.WriteString(str.c_str());
	return ofile;
}

CStdioFile& operator<<(CStdioFile& ofile,const CString& str)
{
	ofile.WriteString(str);
	return ofile;
}

CStdioFile& operator<<(CStdioFile& ofile,const int& i)
{
	ofile.WriteString(intToCString(i));
	return ofile;
}

CStdioFile& operator<<(CStdioFile& ofile,const double& f)
{
	ofile.WriteString(doubleToCString(f));
	return ofile;
}

/*const char* operator+(const char* a,const char* b)
{
	std::string as(a);
	std::string bs(b);
	return (a+b).c_str();
}

const char* operator+(const char* a,std::string b)
{
	std::string as(a);
	std::string ab=(as+b);
	return ab.c_str();
}*/

std::string operator+(std::string a,std::string b)
{
	return a.append(b);
}

NSEND
