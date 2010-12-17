// $Id$

namespace ADBC
{
namespace ODBC
{
//
// Query
//
ADBC_INLINE
Query::Query (Connection & conn)
: parent_ (conn),
  handle_ (SQL_NULL_HSTMT),
  cursor_open_ (false),
  is_prepared_ (false),
  params_ (*this),
  record_ (*this)
{
  this->init ();
}

//
// parameters
//
ADBC_INLINE
Parameter_List & Query::parameters (void)
{
  return this->params_;
}

//
// parameters
//
ADBC_INLINE
const Parameter_List & Query::parameters (void) const
{
  return this->params_;
}

//
// handle
//
ADBC_INLINE
HSTMT Query::handle (void) const
{
  return this->handle_;
}

}
}
