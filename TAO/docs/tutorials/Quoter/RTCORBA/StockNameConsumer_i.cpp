
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

void Stock_StockNameConsumer_i::push_StockName (::Stock::StockName *the_stockname)
  throw (::CORBA::SystemException)
{
  // Verify this is the correct stock. If this is not the stock that
  // we are interested in then we need to stop and return control to
  // ORB's event loop.
  if (ACE_OS::strcmp (this->stock_name_, the_stockname->name ()) != 0)
    return;

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
        quoter->get_stock_info (the_stockname->name());

      // Display the current value of the stock.
      ACE_DEBUG ((LM_DEBUG,
                  "Current value of %s is $%d\n", 
                  the_stockname->name (),
                  stock_info->last));
    }
  catch (Stock::Invalid_Stock &e)
    {
      ACE_PRINT_EXCEPTION (e, "Stock exception: invalid stock name: ");
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Stock_StockNameConsumer_i::push_StockName: ");
    }
}

::Stock::Cookie * 
Stock_StockNameConsumer_i::cookie ()
  throw (::CORBA::SystemException)
{
  return this->cookie_;
}

void 
Stock_StockNameConsumer_i::cookie (::Stock::Cookie *cookie)
  throw (::CORBA::SystemException)
{
  this->cookie_ = cookie;
}

