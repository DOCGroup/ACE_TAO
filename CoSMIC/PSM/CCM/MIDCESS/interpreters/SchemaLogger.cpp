#include "SchemaLogger.h"

NSBEGIN

IMPLEMENT_DYNAMIC(SchemaLogger,CObject)

void SchemaLogger::setCommentPrefix(const char* cprefix) const	{	COMMENT_PREFIX=CString(cprefix);	}
void SchemaLogger::setErrorPrefix(const char* eprefix) const	{	ERROR_PREFIX=CString(eprefix);		}
void SchemaLogger::setWarningPrefix(const char* wprefix) const	{	WARNING_PREFIX=CString(wprefix);	}

void SchemaLogger::write(const char* s)
{
	write(CString(s));
}


void SchemaLogger::write(const CString& s)
{
	if(isinit)
	{
		if(using_ostrm)
			*ostrm_lf<<LPCTSTR(s);
		else
			*csf_lf<<s;
	}
}

void SchemaLogger::write(const std::string& s)
{
	if(isinit)
	{
		if(using_ostrm)
			*ostrm_lf<<s.c_str();
		else
			*csf_lf<<s;
	}
}

void SchemaLogger::write(const double& f)
{
	if(isinit)
	{
		if(using_ostrm)
			*ostrm_lf<<f;
		else
			*csf_lf<<f;
	}
}

void SchemaLogger::write(const int& i)
{
	if(isinit)
	{
		if(using_ostrm)
			*ostrm_lf<<i;
		else
			*csf_lf<<i;
	}
}

void SchemaLogger::timeStamp()
{
	if(isinit)
	{
		CTime ct=CTime::GetCurrentTime();
		CString ts=ct.Format("%A, %B %d, %Y, %H:%M:%S ");
		write(ts);
		write(CString("\n"));
	}
}

SchemaLogger::init(const char* lgname,bool use_ostrm)
{
	if(use_ostrm)
	{
		ostrm_lf=new std::ofstream(lgname);
		csf_lf=NULL;
		isinit=true;
	}
	else
	{
		ostrm_lf=NULL;
		csf_lf=new CStdioFile(lgname,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite | CFile::typeText);
		isinit=true;
	}
	using_ostrm=use_ostrm;
	ERROR_PREFIX="#ERROR:: ";
	COMMENT_PREFIX="// ";
	WARNING_PREFIX="#WARNING:: ";
}

SchemaLogger::SchemaLogger(const std::string& lgname,bool use_ostrm)
{
	init(lgname.c_str(),use_ostrm);
}

SchemaLogger::SchemaLogger(const CString& lgname,bool use_ostrm)
{
	init(LPCTSTR(lgname),use_ostrm);
}

SchemaLogger::SchemaLogger(bool use_ostrm)
{
	using_ostrm=use_ostrm;
	isinit=false;
	csf_lf=NULL;
	ostrm_lf=NULL;
}

SchemaLogger::~SchemaLogger()
{
	finalize();
}

bool SchemaLogger::isValid()
{
	return isinit;
}

void SchemaLogger::flush()
{
	if(isinit)
	{
		if(using_ostrm)
			ostrm_lf->flush();
		else
			csf_lf->Flush();
	}
}

void SchemaLogger::finalize()
{
	if(isinit)
	{
		flush();
		if(!using_ostrm)
			csf_lf->Close();
		isinit=false;
	}
}

SchemaLogger& SchemaLogger::notify(const char* s,ERRMSGTYPE type)
{
	switch(type)
	{
		case ERRMSG:
			write(ERROR_PREFIX);
			break;
		case WARNMSG:
			write(WARNING_PREFIX);
			break;
		default:
			write(COMMENT_PREFIX);
			break;
	}
	*this<<s<<"\n";
	return *this;
}

SchemaLogger& SchemaLogger::notify(const std::string& s,ERRMSGTYPE type)
{
	switch(type)
	{
		case ERRMSG:
			write(ERROR_PREFIX);
			break;
		case WARNMSG:
			write(WARNING_PREFIX);
			break;
		default:
			write(COMMENT_PREFIX);
			break;
	}
	*this<<s<<"\n";
	return *this;
}

SchemaLogger& SchemaLogger::notify(const CString& s,ERRMSGTYPE type)
{
	switch(type)
	{
		case ERRMSG:
			write(ERROR_PREFIX);
			break;
		case WARNMSG:
			write(WARNING_PREFIX);
			break;
		default:
			write(COMMENT_PREFIX);
			break;
	}
	*this<<s<<"\n";
	return *this;
}

SchemaLogger& SchemaLogger::notify(const int& i,ERRMSGTYPE type)
{
	switch(type)
	{
		case ERRMSG:
			write(ERROR_PREFIX);
			break;
		case WARNMSG:
			write(WARNING_PREFIX);
			break;
		default:
			write(COMMENT_PREFIX);
			break;
	}
	*this<<i<<"\n";
	return *this;
}

SchemaLogger& SchemaLogger::notify(const double& d,ERRMSGTYPE type)
{
	switch(type)
	{
		case ERRMSG:
			write(ERROR_PREFIX);
			break;
		case WARNMSG:
			write(WARNING_PREFIX);
			break;
		default:
			write(COMMENT_PREFIX);
			break;
	}
	*this<<d<<"\n";
	return *this;
}

SchemaLogger& operator<<(SchemaLogger& gl,const char* cptr)
{
	gl.write(CString(cptr));
	return gl;
}

SchemaLogger& operator<<(SchemaLogger& gl,const std::string& s)
{
	gl.write(s);
	return gl;
}

SchemaLogger& operator<<(SchemaLogger& gl,const CString& s)
{
	gl.write(s);
	return gl;
}

SchemaLogger& operator<<(SchemaLogger& gl,const int& i)
{
	gl.write(i);
	return gl;
}

SchemaLogger& operator<<(SchemaLogger& gl,const double& f)
{
	gl.write(f);
	return gl;
}

NSEND
