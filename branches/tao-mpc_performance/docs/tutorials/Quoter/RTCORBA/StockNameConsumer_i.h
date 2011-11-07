// $Id$

/**
 * @file StockNameConsumer_i.h
 *
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Douglas C. Schmidt <schmidt@dre.vanderbilt.edu>
 */

#ifndef STOCKNAMECONSUMER_I_H_
#define STOCKNAMECONSUMER_I_H_

// local headers
#include "CommonS.h"
#include "BrokerS.h"

// TAO headers
#include "tao/RTCORBA/RTCORBA.h"

// STL headers
#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration
class Stock_StockBroker_i;

/**
 * @class Stock_StockNameConsumer_i
 * @brief This class is used as a callback interface used by the
 * distributor to notify brokers of updates. This class is ALSO used as a
 * signal handler to catch SIGINT and properly unsubscribe from the
 * distributor.
 */
class  Stock_StockNameConsumer_i :
  public virtual POA_Stock::StockNameConsumer
{
public:
  /**
   * Constructor.
   *
   * @param parent The parent StockBroker object reference of the StockNameConsumer object.
   * @param stock_name The name of the stock that the parent StockBroker object need to listen to.
   */
  Stock_StockNameConsumer_i (Stock_StockBroker_i  &context,
                             const char *stock_name);

  /// Destructor.
  virtual ~Stock_StockNameConsumer_i (void);

  /**
   * Push an event to the consumer.
   *
   * @param the_stockname The name of the stock that will be provided to the context StockBroker object.
   */
  virtual void push_StockName (::Stock::StockNames *the_stockname);

  /**
   * Get the cookie attribute.
   *
   * @return The cookie attribute.
   */
  virtual ::Stock::Cookie * cookie_ ();

  /**
   * Set the cookie attribute.
   *
   * @param cookie The value of cookie attribute that need to be set.
   */
  virtual void cookie_ (::Stock::Cookie *cookie);

private:
  /// Context of the StockNameConsumer object.
  Stock_StockBroker_i &context_;

  /// Name of the stock to listen to.
  CORBA::String_var stock_name_;

  Stock::Cookie_var cookie__;
};

#endif /*  STOCKNAMECONSUMER_I_H_ */
