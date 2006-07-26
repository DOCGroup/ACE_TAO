// $Id$

/**
 * @file Stock_Database.h
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 */

#ifndef STOCK_DATABASE_H_
#define STOCK_DATABASE_H_

// ACE headers
#include "ace/Task.h"

// local headers
#include "Common_i.h"

// STL headers
#include <map>

/**
 * @class Stock_Database
 * @brief This class is used to install, update and publish the information of 
 * all the stocks. It uses the singleton design pattern.
 */
class Stock_Database: public ACE_Task_Base
{
public:
  /// Constructor.
  Stock_Database (void);

  /// Destructor.
  //Stock_Database (void);

  /**
   * Create a StockInfo object stored in the database with the given name.
   *
   * @param name The name of the stock.
   * @return A StockInfo object.
   */
  Stock::StockInfo *get_stock_info (const char *name);

  /**
   * This function is called by the Stock Distributor server's thread function 
   * to notify the Stock Broker client the state change of the stock it interested in. 
   *
   * @param consumer The StockNameConsumer object reference.
   */
  void publish_stock_info (Stock::StockNameConsumer_ptr consumer);
  
  /**
   * This function is used to calculate the new high, low and last values
   * for each stock in the stock database randomly. 
   */
  virtual int svc (void);

private:
  /// Stock map.
  typedef std::map<std::string, Stock::StockInfo_var> StockMap;
  StockMap stock_map_;
};

typedef ACE_Singleton<Stock_Database, ACE_Thread_Mutex> Stock_Database_Singleton;
#define STOCK_DATABASE Stock_Database_Singleton::instance()

#endif	// !defined STOCK_DATABASE_H_
