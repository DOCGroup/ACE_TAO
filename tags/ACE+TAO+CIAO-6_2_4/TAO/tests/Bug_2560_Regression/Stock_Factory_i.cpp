//
// $Id$
//

#include "Stock_Factory_i.h"

#include <iostream>

Quoter_Stock_Factory_i::Quoter_Stock_Factory_i ()
{
  PortableServer::Servant_var<Quoter_Stock_i> servant1 =
    new Quoter_Stock_i ("RHAT", "RedHat, Inc.", 210);
  PortableServer::Servant_var<Quoter_Stock_i> servant2 =
    new Quoter_Stock_i ("MSFT", "Microsoft, Inc.", 91);

  PortableServer::POA_var poa1 = servant1->_default_POA ();
  CORBA::String_var str = servant1->symbol ();
  PortableServer::ObjectId_var oid1 = PortableServer::string_to_ObjectId (str.in ());
  poa1->activate_object_with_id (oid1.in(), servant1.in ());
  CORBA::Object_var obj = poa1->id_to_reference (oid1.in ());
  ref1_ = Quoter::Stock::_narrow (obj.in ());

  PortableServer::POA_var poa2 = servant2->_default_POA ();
  str = servant2->symbol ();
  PortableServer::ObjectId_var oid2 = PortableServer::string_to_ObjectId (str.in ());
  poa2->activate_object_with_id (oid2.in(), servant2.in ());
  obj = poa2->id_to_reference (oid2.in ());
  ref2_ = Quoter::Stock::_narrow (obj.in ());
}

Quoter::Stock_ptr
Quoter_Stock_Factory_i::get_stock (const char *symbol)
{
  try {
    if (ACE_OS::strcmp (symbol, "RHAT") == 0) {
      return Quoter::Stock::_duplicate(ref1_.in ());
    } else if (ACE_OS::strcmp (symbol, "MSFT") == 0) {
      return Quoter::Stock::_duplicate(ref2_.in ());
    }
  } catch (CORBA::Exception & e) {
    std::cerr << "CORBA exception raised: " << e << std::endl;
  }
  std::cerr << "Invalid_Stock_Symbol: " << symbol << std::endl ;
  throw Quoter::Invalid_Stock_Symbol ();
}
