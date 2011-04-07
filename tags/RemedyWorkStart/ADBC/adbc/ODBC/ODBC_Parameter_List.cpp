// $Id$

#include "ODBC_Parameter_List.h"

#if !defined (__ADBC_INLINE__)
#include "ODBC_Parameter_List.inl"
#endif

#include "ODBC_Query.h"

namespace ADBC
{
namespace ODBC
{
//
// update
//
void Parameter_List::update (void)
{
  // Get the number of parameters in the prepared statement.
  SQLSMALLINT param_count;

  SQL_STMT_VERIFY (::SQLNumParams (this->query_.handle (), &param_count),
                   this->query_.handle ());

  // If this is the first prepared statement on this handle
  // then we need to allocate a new parameter list.
  this->params_.size (param_count);

  for (SQLSMALLINT i = 0; i < param_count; ++ i)
  {
    this->params_[i].query_ = &this->query_;
    this->params_[i].index_ = i;
  }
}

}
}
