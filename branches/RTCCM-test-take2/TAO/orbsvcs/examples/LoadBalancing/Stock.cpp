//
// $Id$
//

#include "Stock.h"

Stock::Stock (const char *symbol,
              const char *full_name,
              CORBA::Double price)
  :  symbol_ (symbol),
     full_name_ (full_name),
     price_ (price)
{
}

char *
Stock::symbol () throw (CORBA::SystemException)
{
  return CORBA::string_dup (this->symbol_.c_str ());
}

char *
Stock::full_name () throw (CORBA::SystemException)
{
  return CORBA::string_dup (this->full_name_.c_str ());
}

CORBA::Double
Stock::price () throw (CORBA::SystemException)
{
  return this->price_;
}
