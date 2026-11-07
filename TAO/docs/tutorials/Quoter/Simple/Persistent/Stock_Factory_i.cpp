
#include "Stock_Factory_i.h"

#include <cstring>

Quoter_Stock_Factory_i::Quoter_Stock_Factory_i ()
  :  rhat_ ("RHAT", "RedHat, Inc.", 210),
     msft_ ("MSFT", "Microsoft, Inc.", 91)
{
}

Quoter::Stock_ptr
Quoter_Stock_Factory_i::get_stock (char const* symbol)
{
  if (std::strcmp (symbol, "RHAT") == 0) {
    return this->rhat_._this ();
  } else if (std::strcmp (symbol, "MSFT") == 0) {
    return this->msft_._this ();
  }
  throw Quoter::Invalid_Stock_Symbol ();
}
