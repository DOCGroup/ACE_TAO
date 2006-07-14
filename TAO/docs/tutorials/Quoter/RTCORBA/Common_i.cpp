// $Id$

// local headers
#include "Common_i.h"
#include "Stock_PriorityMapping.h"
#include "Stock_Database.h"

// ACE headers
#include <ace/streams.h>
#include <ace/OS_NS_unistd.h>

// TAO headers
#include <tao/CORBA.h>
#include <tao/AnyTypeCode/Typecode.h>
#include <tao/RTCORBA/RTCORBA.h>
#include <tao/RTCORBA/Thread_Pool.h>
#include <tao/RTPortableServer/RTPortableServer.h>

// STL headers
#include <strstream>
#include <ctime>
#include <cstring>

// Implementation skeleton constructor
Stock_Trigger_i::Stock_Trigger_i (void)
{
}

// Implementation skeleton destructor
Stock_Trigger_i::~Stock_Trigger_i (void)
{
}

void Stock_Trigger_i::start (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
}

void Stock_Trigger_i::stop (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
}

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
  // @@ Shanshan Yikes!  You are using a pointer to store the parent_
  // but don't deallocate it here!  Please use a _var.
}

void Stock_StockNameConsumer_i::push_StockName (
    ::Stock::StockName * the_stockname
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  // Verify the <parent_> is valid. There is no need to continue processing
  // any information if there is not <parent_> attached to the object. The
  // <parent_> is needed to retrieve the <quoter>.
  if (CORBA::is_nil (this->parent_))
    return;

  // Verify this is the correct stock. If this is not the stock that we are
  // interested in then we need to stop and return control to the client/
  // server.
  // @@ Shanshan - Please use ACE_OS:strcmp
  if (strcmp (this->stock_name_, the_stockname->name ()) != 0)
    return;

  // Get the quoter for the connection.
  ACE_DEBUG ((LM_DEBUG, "*** message: getting the quoter connection\n"));
  Stock::StockQuoter_var quoter = this->parent_->get_connection_quoter_info ();

  try {
    // Request more information from the quoter.
    ACE_DEBUG ((LM_DEBUG, "*** message: requesting the stock info from the quoter\n"));
    Stock::StockInfo_var stock_info = quoter->get_stock_info (the_stockname->name());

    // @@ Shanshan - Please use ACE_DEBUG instead of cout.
    // Display the current value of the stock.
    cout << "Current value of " << the_stockname->name () << " is $" << stock_info->last << endl;
  }
  // @@ Shanshan - I am not sure it is a good idea to swallow the exceptions.
  catch (Stock::Invalid_Stock &) {
    cerr << "Stock exception: invalid stock name (" << the_stockname->name () << ")" << endl;
  }
  catch (CORBA::Exception &ex)	{
    // @@ Shanshan - This won't print out what your expect, please use ACE_PRINT_EXCEPTION
    cerr << "CORBA exception: " << ex << endl;
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

::Stock::StockInfo * Stock_StockQuoter_i::get_stock_info (
    const char * stock_name
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Stock::Invalid_Stock
  ))
{
  ACE_DEBUG ((LM_DEBUG, "*** message: requesting stock_info from the database\n"));
  // @@ Shanshan - Be careful about exception safety!  Either use an
  // ACE type/ACE_GUARD macro or use try/catch blocks to make sure the
  // mutex gets unlocked.
  Stock_Database::instance ()->lock ();
  Stock::StockInfo * stock = Stock_Database::instance ()->get_stock_info (stock_name);
  Stock_Database::instance ()->unlock ();

  if (stock == 0)
    throw Stock::Invalid_Stock();
  return stock;
}
