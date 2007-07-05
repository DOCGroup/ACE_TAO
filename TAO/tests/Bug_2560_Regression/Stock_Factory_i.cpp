//
// $Id$
//

#include "Stock_Factory_i.h"

#include <iostream>

Quoter_Stock_Factory_i::Quoter_Stock_Factory_i ()
{
  servant1_ = new Quoter_Stock_i("RHAT", "RedHat, Inc.", 210);
  servant2_ = new Quoter_Stock_i("MSFT", "Microsoft, Inc.", 91);

  PortableServer::POA_ptr poa1 = servant1_->_default_POA() ;
  PortableServer::ObjectId_var oid1 = PortableServer::string_to_ObjectId( servant1_->symbol() ) ;
  poa1->activate_object_with_id ( oid1.in(), servant1_ ) ;
  ref1_ = Quoter::Stock::_narrow( poa1->id_to_reference(oid1.in()) ) ;

  PortableServer::POA_ptr poa2 = servant2_->_default_POA() ;
  PortableServer::ObjectId_var oid2 = PortableServer::string_to_ObjectId( servant2_->symbol() ) ;
  poa2->activate_object_with_id ( oid2.in(), servant2_ ) ;
  ref2_ = Quoter::Stock::_narrow( poa2->id_to_reference(oid2.in()) ) ;
}

Quoter::Stock_ptr
Quoter_Stock_Factory_i::get_stock (const char *symbol)
{
  try {
    if (ACE_OS::strcmp (symbol, "RHAT") == 0) {
      return Quoter::Stock::_duplicate(ref1_);
    } else if (ACE_OS::strcmp (symbol, "MSFT") == 0) {
      return Quoter::Stock::_duplicate(ref2_);
    }
  } catch (CORBA::Exception & e) {
    std::cerr << "CORBA exception raised: " << e << std::endl;
  }
  std::cerr << "Invalid_Stock_Symbol: " << symbol << std::endl ;
  throw Quoter::Invalid_Stock_Symbol ();
}
