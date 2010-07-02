//
// $Id$
//

#include "Stock_i.h"
#include "ace/OS_NS_unistd.h"

PortableServer::POA_var Quoter_Stock_i::_poa ;

Quoter_Stock_i::Quoter_Stock_i (const char *symbol,
                                const char *full_name,
                                CORBA::Double price)
  : symbol_ (symbol)
  , full_name_ (full_name)
  , price_ (price)
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

Quoter::Stock::StockHistory*
Quoter_Stock_i::history ()
{
  Quoter::Stock::StockHistory_var hist = new Quoter::Stock::StockHistory ;

  // delay a little bit so we have chance to kill client
  // before this method returns
  ACE_OS::sleep(5) ;

  const unsigned hsize = 200000 ;
  hist->length(hsize) ;
  for ( unsigned int i = 0 ; i < hsize ; ++ i ) {
    hist[i] = double(i+1) ;
  }

  ACE_DEBUG ((LM_DEBUG, "Returning method in server\n"));
  return hist._retn() ;
}
