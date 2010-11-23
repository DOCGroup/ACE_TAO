// -*- C++ -*-
// $Id$

#include "Record.h"

namespace ADBC
{
namespace SQLite
{
//
// Query
//
ADBC_INLINE
Query::Query (Connection & parent)
: parent_ (parent),
  stmt_ (0),
  params_ (*this),
  record_ (*this)
{

}

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

}
}
