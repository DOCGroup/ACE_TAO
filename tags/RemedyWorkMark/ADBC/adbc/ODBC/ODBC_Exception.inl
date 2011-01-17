// -*- C++ -*-
// $Id$

namespace ADBC
{
namespace ODBC
{
//
// Exception
//
ADBC_INLINE
Exception::Exception (void)
: ::ADBC::Exception ("unknown ODBC exception has occurred")
{

}

//
// Exception
//
ADBC_INLINE
Exception::Exception (const char * message)
: ::ADBC::Exception (message)
{

}

//
// ~Exception
//
ADBC_INLINE
Exception::~Exception (void)
{

}
}
}
