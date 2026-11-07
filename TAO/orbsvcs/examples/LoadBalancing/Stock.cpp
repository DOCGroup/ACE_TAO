
#include "Stock.h"

Stock::Stock (char const *symbol,
              char const *full_name,
              CORBA::Double price)
  :  symbol_ (symbol),
     full_name_ (full_name),
     price_ (price)
{
}

char *
Stock::symbol ()
{
  return CORBA::string_dup (this->symbol_.c_str ());
}

char *
Stock::full_name ()
{
  return CORBA::string_dup (this->full_name_.c_str ());
}

CORBA::Double
Stock::price ()
{
  return this->price_;
}
