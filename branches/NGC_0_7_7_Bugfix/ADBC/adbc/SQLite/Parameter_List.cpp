// $Id$

#include "Parameter_List.h"

#if !defined (__ADBC_INLINE__)
#include "Parameter_List.inl"
#endif

#include "Query.h"
#include "sqlite3.h"

namespace ADBC
{
namespace SQLite
{
//
// reset
//
void Parameter_List::reset (void)
{
  // Get the number of parameters in the statement.
  int new_size = ::sqlite3_bind_parameter_count (this->parent_.stmt_);

  // Resize the statement.
  int old_size = this->params_.size ();
  this->params_.size (new_size);

  if (new_size > old_size)
    for (int i = old_size; i < new_size; ++ i)
      this->params_[i].reset (this, i + 1);
}

//
// operator []
//
ADBC_INLINE
Parameter &
Parameter_List::operator [] (const ACE_CString & name)
{
  int index = ::sqlite3_bind_parameter_index (this->parent_.stmt_,
                                              name.c_str ());

  return this->params_[index - 1];
}

//
// operator []
//
ADBC_INLINE
const Parameter &
Parameter_List::operator [] (const ACE_CString & name) const
{
  int index = ::sqlite3_bind_parameter_index (this->parent_.stmt_,
                                              name.c_str ());

  return this->params_[index - 1];
}
}
}
