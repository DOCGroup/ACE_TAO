
#include "stdafx.h"
#include "CommonError.h"
#include "CommonSmart.h"
#include <stdio.h>

// --------------------------- hresult_exception

hresult_exception::hresult_exception()
{
	hr = 0;
}

hresult_exception::hresult_exception(const hresult_exception &e)
{
	hr = e.hr;
}

hresult_exception::hresult_exception(HRESULT a)
{
	hr = a;
}

hresult_exception &hresult_exception::operator=(const hresult_exception &e)
{
	hr = e.hr;
	return *this;
}

hresult_exception &hresult_exception::operator=(HRESULT a)
{
	hr = a;
	return *this;
}

hresult_exception::~hresult_exception()
{
}

const char *hresult_exception::what() const
{
	static char message[80];
	sprintf(message, "HRESULT (0x%08lx) exception", hr);
	return message;
}

// --------------------------- Error Codes

LPOLESTR common_descs[] = 
{
	OLESTR("Inproper use of object"),
	OLESTR("Unknown exception"),
	OLESTR("Data conversion has failed"),
	OLESTR("Requested != count in GetAll"),
	OLESTR("[out] parameter is not empty"),
	OLESTR("Passed interface is not from the same project"),
	OLESTR("Object was not found"),
	OLESTR("File could not be opened"),
	OLESTR("VERIFY has failed"),
	OLESTR("COMTHROW test exception throwed"),
};

LPOLESTR core_descs[] = 
{
	OLESTR("Object is locked by another Core component"),
	OLESTR("Lock value in Storage is invalid"),
	OLESTR("Attribute is not found"),
	OLESTR("Meta object is not found"),
	OLESTR("Meta project is invalid"),
	OLESTR("Invalid response from Repository"),
	OLESTR("CoreProject - CoreMetaProject mismatch"),
	OLESTR("Object is deleted or unloaded (zombie)"),
	OLESTR("Invalid ValueType"),
	OLESTR("No active transaction"),
	OLESTR("Object already exists"),
	OLESTR("No Territory has been selected"),
	OLESTR("Object is not locked"),
	OLESTR("Invalid data in Repository"),
	OLESTR("Cannot delete Object, has nonempty collection"),
	OLESTR("Cannot resolve the connection string"),
	OLESTR("Invalid binary file storage")
	OLESTR("Project has no name")
};

LPOLESTR meta_descs[] = 
{
	OLESTR("Invalid ATTVAL type"),
	OLESTR("MetaProject is not open"),
	OLESTR("Invalid Path"),
	OLESTR("Invalid MetaRef")
};

LPOLESTR parser_descs[] = 
{
	OLESTR("Invalid DTD file"),
	OLESTR("Invalid GUID"),
	OLESTR("Invalid XML filename"),
	OLESTR("Invalid MGA object"),
	OLESTR("Invalid META object"),
	OLESTR("Too many passes while parsing the XML file"),
	OLESTR("Invalid long value in XML file")
};

void SetErrorInfo(LPOLESTR desc)
{
	ASSERT( desc != NULL );

	try
	{
		CComObjPtr<ICreateErrorInfo> errorinfo;
		COMTHROW( CreateErrorInfo(PutOut(errorinfo)) );
		COMTHROW( errorinfo->SetDescription(desc) );
		COMTHROW( errorinfo->SetSource(OLESTR("GME")) );

		CComObjPtr<IErrorInfo> errorinfo2;
		COMTHROW( QueryInterface(errorinfo, errorinfo2) );
		COMTHROW( SetErrorInfo(0, errorinfo2) );
	}
	catch(hresult_exception &)
	{
		// do nothing
	}
}

void SetErrorInfo(HRESULT hr, LPOLESTR desc2)
{
	ASSERT( sizeof(common_descs)/sizeof(LPOLESTR) == (E_COMMON_LAST-E_COMMON_FIRST+1) );
	ASSERT( sizeof(core_descs)/sizeof(LPOLESTR) == (E_CORE_LAST-E_CORE_FIRST+1) );
	ASSERT( sizeof(meta_descs)/sizeof(LPOLESTR) == (E_META_LAST-E_META_FIRST+1) );
	ASSERT( sizeof(parser_descs)/sizeof(LPOLESTR) == (E_PARSER_LAST-E_PARSER_FIRST+1) );

	LPOLESTR desc = NULL;
	if( E_COMMON_FIRST <= hr && hr <= E_COMMON_LAST ) desc = common_descs[hr - E_COMMON_FIRST];
	else if( E_CORE_FIRST <= hr && hr <= E_CORE_LAST ) desc = core_descs[hr - E_CORE_FIRST];
	else if( E_META_FIRST <= hr && hr <= E_META_LAST ) desc = meta_descs[hr - E_META_FIRST];
	else if( E_PARSER_FIRST <= hr && hr <= E_PARSER_LAST ) desc = parser_descs[hr - E_PARSER_FIRST];

	if( desc2 == NULL )
	{
		if( desc != NULL )
			SetErrorInfo(desc);
	}
	else
	{
		CComBSTR d(desc);
		if( desc != NULL )
			d.Append(OLESTR(" :"));

		d.Append(desc2);
		SetErrorInfo(d);
	}
}

void GetErrorInfo(BSTR *desc)
{
	ASSERT( desc != NULL );

	try
	{
		CComObjPtr<IErrorInfo> errinfo;
		COMTHROW( GetErrorInfo(0, PutOut(errinfo)) );
		ASSERT( errinfo != NULL );

		COMTHROW( errinfo->GetDescription(desc) );
	}
	catch(hresult_exception &)
	{
		// do nothing
	}
}

