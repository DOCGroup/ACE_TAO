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
  state_ (SQLITE_OK)
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
// done
//
ADBC_INLINE
bool Record::done (void) const
{
  return this->state_ == SQLITE_DONE;
}
}
}
