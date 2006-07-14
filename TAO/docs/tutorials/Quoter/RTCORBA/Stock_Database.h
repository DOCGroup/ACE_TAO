// $Id$

#ifndef STOCK_DATABASE_H_
#define STOCK_DATABASE_H_

// local headers
#include "Common_i.h"

// STL headers
#include <map>
#include <string>

// @@ Shanshan, please document this class using doxygen style comments.

/**
 *
 */
class Stock_Database
{
public:
  void init (RTCORBA::RTORB_ptr rt_orb);

  void term (void);

  static Stock_Database *instance (void);

  Stock::StockInfo *get_stock_info (const char *name);

  void publish_stock_info (Stock::StockNameConsumer_ptr consumer);

  void lock (void);

  void unlock (void);

private:
  Stock_Database (void);

  static Stock_Database *instance_;

  RTCORBA::RTORB_var rt_orb_;

  typedef std::map<std::string, Stock::StockInfo *> StockMap;

  StockMap stock_map_;

  RTCORBA::Mutex_var lock_;

  ACE_hthread_t _thread;

  ACE_thread_t _thread_id;

  bool active_;

  static ACE_THR_FUNC_RETURN _thread_func (void *param);
};

#endif	// !defined STOCK_DATABASE_H_

