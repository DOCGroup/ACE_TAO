// $Id$

#include "Stock_Quoter_i.h"
#include "ace/Log_Msg.h"

Stock_Quoter_i::Stock_Quoter_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Stock_Quoter_i::~Stock_Quoter_i ()
{
}

void
Stock_Quoter_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

CORBA::Float Stock_Quoter_i::get_quote (const char* stock_id)
{
   ACE_DEBUG ((LM_DEBUG, "Stock_Quoter_i::get_quote\n"));
   CORBA::Float price = 0.0;
   if (ACE_OS::strcmp(stock_id, "BA") == 0)
   {
     price = 54.25;
   }
   else if (ACE_OS::strcmp(stock_id, "OCI") == 0)
   {
     price = 100.00;
   }
   else
   {
     ACE_DEBUG ((LM_DEBUG, "Bad ticker symbol, Stock Id: %s\n", stock_id));
     throw Bad_Ticker_Symbol(stock_id);
   }
   ACE_DEBUG ((LM_DEBUG,"Stock Id: %s, Price = %6.2f\n", stock_id, price));
   return price;
}

