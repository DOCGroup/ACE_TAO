
namespace ADBC
{
namespace SQLite
{
//
// Exception
//
ADBC_INLINE
Exception::Exception (void)
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
