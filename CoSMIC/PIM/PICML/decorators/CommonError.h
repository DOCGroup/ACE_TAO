#ifndef MGA_COMMONERROR_H
#define MGA_COMMONERROR_H

#include <exception>
using std::exception;

// --------------------------- ASSERT and VERIFY

#if !defined(ASSERT) && defined(ATLASSERT)
#define ASSERT ATLASSERT
#endif

// --------------------------- hresult_exception

class hresult_exception : public exception
{
public:
	hresult_exception() throw();
	hresult_exception(const hresult_exception &e) throw();
	hresult_exception(HRESULT hr) throw();
	hresult_exception &operator=(const hresult_exception &e) throw();
	hresult_exception &operator=(HRESULT hr) throw();
	~hresult_exception() throw();
	virtual const char *what() const throw();

public:
	HRESULT hr;
};

// --------------------------- Exceptions and COM

#define NOTHROW throw()

#define COMTHROW(FUNC) \
do { \
	HRESULT _hr = (FUNC); \
	if( FAILED(_hr) ) { \
		ASSERT(("COMTHROW: Throwing HRESULT exception. Press IGNORE", false)); \
		throw hresult_exception(_hr); \
	} \
} while(false)

#define VERIFYTHROW(FUNC) \
do { \
	if( !(bool)(FUNC) ) { \
		ASSERT(("VERIFYTHROW: Throwing VERIFY exception. Press IGNORE", false)); \
		throw hresult_exception(E_VERIFY); \
	} \
} while(false)

#define COMVERIFY(FUNC) \
	VERIFY(SUCCEEDED(FUNC))

#define HR_THROW(_hr) \
do { \
	ASSERT(("HR_THROW: Throwing HRESULT exception. Press IGNORE", false)); \
	throw hresult_exception(_hr); \
} while(false)

#define COMTRY try

#define COMCATCH(CLEANUP) \
	catch(hresult_exception &e) \
	{ \
		ASSERT( FAILED(e.hr) ); \
		{ CLEANUP; } \
		SetErrorInfo(e.hr); \
		return e.hr; \
	} \
	return S_OK;

#define COMRETURN(HR) \
	{ \
		HRESULT _hr = HR; \
		if( FAILED(_hr) ) \
			SetErrorInfo(_hr); \
		return _hr; \
	}

inline bool CheckOut_IsBound(BSTR b) { return b != NULL; }
inline bool CheckOut_IsBound(const VARIANT &v) { return v.vt != VT_EMPTY; }
inline bool CheckOut_IsBound(const IUnknown *p) { return p != NULL; }
inline bool CheckOut_IsBound(const int) { return false; }
inline bool CheckOut_IsBound(const short) { return false; }
inline bool CheckOut_IsBound(const ULONG) { return false; }
inline bool CheckOut_IsBound(const SAFEARRAY *p) { return p != NULL; }

#define CHECK_IN(PTR) \
{ \
	if( PTR == NULL ) \
		COMRETURN(E_INVALIDARG) \
}

#define CHECK_OUT(PTR) \
{ \
	if( PTR == NULL ) \
		COMRETURN(E_POINTER) \
	else if( CheckOut_IsBound(*PTR) ) \
		COMRETURN(E_OUT_NOT_EMPTY) \
}

#define CHECK_INOUT(PTR) \
{ \
	if( PTR == NULL ) \
		COMRETURN(E_INVALIDARG) \
}

#define MGACOLL_ITERATE(iftype, collifptr) \
{ \
	ASSERT( collifptr != NULL ); \
	long iter_count = 0; \
	COMTHROW( collifptr->get_Count(&iter_count) ); \
	ASSERT( iter_count >= 0 ); \
	CComPtr<iftype> *arrptr, *arrend, *array = new CComPtr<iftype>[iter_count]; \
	if(iter_count > 0) \
		COMTHROW( collifptr->GetAll(iter_count, &(*array)) ); \
	arrend = array+iter_count; \
	for(arrptr = array; arrptr != arrend; arrptr++)

#define MGACOLL_ITER (*arrptr)
#define MGACOLL_AT_END (arrptr == arrend)

#define MGACOLL_ITERATE_END delete []array; }



// --------------------------- Common Error Codes

void SetErrorInfo(LPOLESTR desc) NOTHROW;
void SetErrorInfo(HRESULT hr, LPOLESTR desc2 = NULL) NOTHROW;
void GetErrorInfo(BSTR *p) NOTHROW;

//		One or more arguments are invalid
//*****	E_INVALIDARG

//		Not enough storage is available to complete this operation
//*****	E_OUTOFMEMORY

//		no implementation yet
//***** E_NOTIMPL

//		Class is not properly used
#define E_INVALID_USAGE					_HRESULT_TYPEDEF_(0x80731001)

//		Unknown exception
#define	E_EXCEPTION						_HRESULT_TYPEDEF_(0x80731002)

//		conversion failed
#define E_CONVERSION					_HRESULT_TYPEDEF_(0x80731003)

//		requested != count in GetAll 
#define E_GETALL						_HRESULT_TYPEDEF_(0x80731004)

//		output parameter is not empty
#define E_OUT_NOT_EMPTY					_HRESULT_TYPEDEF_(0x80731005)

//		the passed interface is not from this module
#define E_SAMEPROJECT					_HRESULT_TYPEDEF_(0x80731006)

//		object was not found
#define E_NOTFOUND						_HRESULT_TYPEDEF_(0x80731007)

//		file open error
#define E_FILEOPEN						_HRESULT_TYPEDEF_(0x80731008)

//		VERIFY has failed
#define E_VERIFY						_HRESULT_TYPEDEF_(0x80731009)

//		COMTHROW test exception
#define E_COMTHROW_TEST					_HRESULT_TYPEDEF_(0x8073100A)

// the description table is in CommonError.cpp
#define E_COMMON_FIRST					E_INVALID_USAGE
#define E_COMMON_LAST					E_COMTHROW_TEST

// --------------------------- Core Error Codes

//		Could not gain lock because an item is already locked
#define E_LOCK_VIOLATION				_HRESULT_TYPEDEF_(0x80732001)

//		The lock value in the database is not valid
#define E_INVALID_LOCK_VALUE			_HRESULT_TYPEDEF_(0x80732002)

//		Attribute not found
#define E_ATTRID						_HRESULT_TYPEDEF_(0x80732003)

//		Meta object not found
#define E_METAID						_HRESULT_TYPEDEF_(0x80732004)

//		meta project is invalid
#define E_METAPROJECT					_HRESULT_TYPEDEF_(0x80732005)

//		invalid response from the repository
#define E_REPOSITORY					_HRESULT_TYPEDEF_(0x80732006)

//		project - metaproject mismatch
#define E_PROJECT_MISMATCH				_HRESULT_TYPEDEF_(0x80732007)

//		object has been deleted, zombie
#define E_ZOMBIE						_HRESULT_TYPEDEF_(0x80732008)

//		invalid valtype
#define E_VALTYPE						_HRESULT_TYPEDEF_(0x80732009)

//		no active transaction
#define E_TRANSACTION					_HRESULT_TYPEDEF_(0x8073200A)

//		the object already exists
#define E_OBJECTEXISTS					_HRESULT_TYPEDEF_(0x8073200B)

//		no territory selected
#define E_TERRITORY						_HRESULT_TYPEDEF_(0x8073200C)

//		not locked
#define E_NOTLOCKED						_HRESULT_TYPEDEF_(0x8073200D)

//		invalid data in repository
#define E_REPOSITORY_DATA				_HRESULT_TYPEDEF_(0x8073200E)

//		cannot delete object because of nonempty collection
#define E_NONEMPTY_COLL					_HRESULT_TYPEDEF_(0x8073200F)

//		cannot resolve the connection string
#define E_UNKNOWN_STORAGE				_HRESULT_TYPEDEF_(0x80732010)

//		binary file storage is invalid
#define E_BINFILE						_HRESULT_TYPEDEF_(0x80732011)

//		missing name
#define E_NAMEMISSING					_HRESULT_TYPEDEF_(0x80732012)

// the description table is in CommonError.cpp
#define E_CORE_FIRST					E_LOCK_VIOLATION
#define E_CORE_LAST						E_BINFILE

// --------------------------- Meta Error Codes

// invalid attval_type
#define E_ATTVALTYPE					_HRESULT_TYPEDEF_(0x80733001)

// metaproject is not open
#define E_META_NOTOPEN					_HRESULT_TYPEDEF_(0x80733002)

// invalid path
#define E_INVALID_PATH					_HRESULT_TYPEDEF_(0x80733003)

// invalid metaref
#define E_METAREF						_HRESULT_TYPEDEF_(0x80733004)

// the description table is in CommonError.cpp
#define E_META_FIRST					E_ATTVALTYPE
#define E_META_LAST						E_METAREF

// --------------------------- Parser Error Codes

// XML parser exception, WE SET THE DESCRIPTION MANUALLY
#define E_XMLPARSER						_HRESULT_TYPEDEF_(0x80734001)

// Invalid DTD file
#define E_INVALID_DTD					_HRESULT_TYPEDEF_(0x80734002)

// Invalid GUID
#define E_INVALID_GUID					_HRESULT_TYPEDEF_(0x80734003)

// Invalid XML filename
#define E_INVALID_FILENAME				_HRESULT_TYPEDEF_(0x80734004)

// Invalid MGA object
#define E_INVALID_MGA					_HRESULT_TYPEDEF_(0x80734005)

// Invalid META object
#define E_INVALID_META					_HRESULT_TYPEDEF_(0x80734006)

// Too many passes while parsing the XML file
#define E_TOOMANYPASSES					_HRESULT_TYPEDEF_(0x80734007)

// Invalid long value
#define E_INVALID_XML_LONG				_HRESULT_TYPEDEF_(0x80734008)

// the description table is in CommonError.cpp
#define E_PARSER_FIRST				E_INVALID_DTD
#define E_PARSER_LAST				E_INVALID_XML_LONG

#endif//MGA_COMMONERROR_H
