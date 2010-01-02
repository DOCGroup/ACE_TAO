// $Id$

namespace ADBC
{
namespace SQLite
{
//
// Record
//
ADBC_INLINE
Record::
Record (const Query & query)
: query_ (query),
  state_ (101)      /* SQLITE_DONE */
{

}

//
// ~Record
//
ADBC_INLINE
Record::~Record (void)
{

}

//
// count
//
ADBC_INLINE
size_t Record::count (void) const
{
  return -1;
}
}
}
