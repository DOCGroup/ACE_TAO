//
// $Id$
//

#include "Stock_Factory_i.h"

Quoter_Stock_Factory_i::Quoter_Stock_Factory_i (PortableServer::POA_ptr poa)
  :  stock_factory_poa_ (PortableServer::POA::_duplicate (poa))
{
}

Quoter::Stock_ptr
Quoter_Stock_Factory_i::get_stock (const char *symbol)
{
  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (symbol);

  try {
    CORBA::Object_var tmp =
      this->stock_factory_poa_->id_to_reference (oid.in ());
    return Quoter::Stock::_narrow (tmp.in ());
  }
  catch (PortableServer::POA::ObjectNotActive &) {
    throw Quoter::Invalid_Stock_Symbol ();
  }
}
