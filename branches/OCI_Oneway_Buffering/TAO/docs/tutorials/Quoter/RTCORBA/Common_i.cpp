// $Id$

// local headers
#include "Common_i.h"
#include "Stock_Database.h"

// Implementation skeleton constructor
Stock_StockNameConsumer_i::Stock_StockNameConsumer_i (Stock::StockBroker_ptr parent,
                                                      const char *stock_name)
                                                      : parent_ (Stock::StockBroker::_duplicate (parent)),
                                                        stock_name_ (CORBA::string_dup (stock_name))
{
}

// Implementation skeleton destructor
Stock_StockNameConsumer_i::~Stock_StockNameConsumer_i (void)
{
}

void Stock_StockNameConsumer_i::push_StockName (::Stock::StockName *the_stockname)
  throw (::CORBA::SystemException)
{
  // Verify the <parent_> is valid. There is no need to continue processing
  // any information if there is not <parent_> attached to the object. The
  // <parent_> is needed to retrieve the <quoter>.
  if (CORBA::is_nil (this->parent_))
    return;

  // Verify this is the correct stock. If this is not the stock that we are
  // interested in then we need to stop and return control to the client/
  // server.
  if (ACE_OS::strcmp (this->stock_name_, the_stockname->name ()) != 0)
    return;

  // Get the quoter for the connection.
  ACE_DEBUG ((LM_DEBUG, "*** message: getting the quoter connection\n"));
  Stock::StockQuoter_var quoter = this->parent_->get_connection_quoter_info ();

  try
  {
    // Request more information from the quoter.
    ACE_DEBUG ((LM_DEBUG, "*** message: requesting the stock info from the quoter\n"));
    Stock::StockInfo_var stock_info = quoter->get_stock_info (the_stockname->name());

    // Display the current value of the stock.
    ACE_DEBUG ((LM_DEBUG, "Current value of %s is $%d\n", 
                the_stockname->name (), stock_info->last));
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

// Implementation skeleton constructor
Stock_StockQuoter_i::Stock_StockQuoter_i (void)
{
}

// Implementation skeleton destructor
Stock_StockQuoter_i::~Stock_StockQuoter_i (void)
{
}

::Stock::StockInfo *Stock_StockQuoter_i::get_stock_info (const char * stock_name)
  throw (::CORBA::SystemException, ::Stock::Invalid_Stock)
{
  ACE_DEBUG ((LM_DEBUG, "*** message: requesting stock_info from the database\n"));

  Stock::StockInfo *stock = STOCK_DATABASE->get_stock_info (stock_name);

  if (stock == 0)
    throw Stock::Invalid_Stock();
  return stock;
}
