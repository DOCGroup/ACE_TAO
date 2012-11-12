//
// $Id$
//

#include "Stock_i.h"

Quoter_Stock_i::Quoter_Stock_i (const char *symbol,
                                const char *full_name,
                                CORBA::Double price)
  :  symbol_ (symbol),
     full_name_ (full_name),
     price_ (price)
{
}

char *
Quoter_Stock_i::symbol ()
{
  return CORBA::string_dup (this->symbol_.c_str ());
}

char *
Quoter_Stock_i::full_name ()
{
  return CORBA::string_dup (this->full_name_.c_str ());
}

CORBA::Double
Quoter_Stock_i::price ()
{
  return this->price_;
}

CORBA::Double
Quoter_Stock_i::get_price_and_names (CORBA::String_out symbol,
                                     CORBA::String_out full_name)
{
  symbol = CORBA::string_dup (this->symbol_.c_str ());
  full_name = CORBA::string_dup (this->full_name_.c_str ());

  return this->price_;
}



