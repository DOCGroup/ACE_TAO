#include "Stock_Factory_i.h"

#include <cstring>

Quoter::Stock_ptr
Quoter_Stock_Factory_i::get_stock (char const *symbol)
{
  if (std::strcmp (symbol, "RHAT") == 0) {
    Quoter_Stock_i * const rhat = new Quoter_Stock_i ("RHAT", "RedHat, Inc.", 210);
    PortableServer::ServantBase_var servant_owner (rhat);
    return rhat->_this ();
  } else if (std::strcmp (symbol, "MSFT") == 0) {
    Quoter_Stock_i * const msft = new Quoter_Stock_i ("MSFT", "Microsoft, Inc.", 91);
    PortableServer::ServantBase_var servant_owner (msft);
    return msft->_this ();
  }
  throw Quoter::Invalid_Stock_Symbol ();
}
