// -*- C++ -*-
// $Id$

#include "Connection.h"

namespace ADBC
{
namespace SQLite
{
//
// ~Query
//
ADBC_INLINE
Query::~Query (void)
{
  this->finalize ();
}

//
// execute_no_record
//
ADBC_INLINE
void Query::execute_no_record (const char * query)
{
  this->prepare (query);
  this->execute_no_record ();
}


//
// execute
//
ADBC_INLINE
Record & Query::execute (const char * query)
{
  this->prepare (query);
  return this->execute ();
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
// last_insert_id
//
ADBC_INLINE
long Query::last_insert_id (void)
{
  return static_cast <long> (::sqlite3_last_insert_rowid (this->parent_.conn_));
}

//
// count
//
ADBC_INLINE
size_t Query::count (void) const
{
  return ::sqlite3_changes (this->parent_.conn_);
}

}
}
