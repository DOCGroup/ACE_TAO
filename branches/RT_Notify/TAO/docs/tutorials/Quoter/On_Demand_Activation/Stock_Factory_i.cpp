// $Id$

#include "Stock_Factory_i.h"

Quoter_Stock_Factory_i::Quoter_Stock_Factory_i ()
{
}

Quoter::Stock_ptr
Quoter_Stock_Factory_i::get_stock (const char *symbol)
    throw (Quoter::Invalid_Stock_Symbol)
{
  if (strcmp (symbol, "RHAT") == 0) {
    Quoter_Stock_i *rhat = new Quoter_Stock_i ("RHAT", "RedHat, Inc.", 210);
    return rhat->_this ();
  } else if (strcmp (symbol, "MSFT") == 0) {
    Quoter_Stock_i *msft = new Quoter_Stock_i ("MSFT", "Microsoft, Inc.", 91);
    return msft->_this ();
  }
  throw Quoter::Invalid_Stock_Symbol ();
}
