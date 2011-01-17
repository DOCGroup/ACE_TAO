// $Id$

// local headers
#include "StockNameConsumer_i.h"
#include "Broker_i.h"

// Implementation skeleton constructor
Stock_StockNameConsumer_i::Stock_StockNameConsumer_i (Stock_StockBroker_i &context,
                                                      const char *stock_name)
  : context_ (context),
    stock_name_ (stock_name)
{
}

// Implementation skeleton destructor
Stock_StockNameConsumer_i::~Stock_StockNameConsumer_i (void)
{
}

void Stock_StockNameConsumer_i::push_StockName (::Stock::StockNames *the_stockname)
{
  for (CORBA::ULong i = 0;
       i < the_stockname->names ().length ();
       ++i)
    {
      if (ACE_OS::strcmp (this->stock_name_.in (),
                          the_stockname->names ()[i]) == 0)
        {
          // Get the quoter for the connection from the <context_>.
          ACE_DEBUG ((LM_DEBUG,
                      "*** message: getting the quoter connection\n"));
          Stock::StockQuoter_var quoter =
            this->context_.get_connection_quoter_info ();

          try
            {
              // Request more information from the StockQuoter.
              ACE_DEBUG ((LM_DEBUG,
                          "*** message: requesting the stock info from the quoter\n"));
              Stock::StockInfo_var stock_info =
                quoter->get_stock_info (the_stockname->names()[i]);

              // Display the current value of the stock.
              ACE_DEBUG ((LM_DEBUG,
                          "Current value of %s is $%d\n",
                          stock_info->name.in (),
                          stock_info->last));
            }
          catch (Stock::Invalid_Stock &e)
            {
              e._tao_print_exception ("Stock exception: invalid stock name: ");
            }
          catch (CORBA::Exception &ex)
            {
      ex._tao_print_exception ("Stock_StockNameConsumer_i::push_StockName: ");
            }
        }
    }
}

::Stock::Cookie *
Stock_StockNameConsumer_i::cookie_ ()
{
  return this->cookie__.in ();
}

void
Stock_StockNameConsumer_i::cookie_ (::Stock::Cookie *cookie)
{
  this->cookie__ = cookie;
  cookie->_add_ref ();
}

