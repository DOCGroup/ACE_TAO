// $Id$

// local headers
#include "Stock_Database.h"

// @@ Shanshan, ACE headers should appear before the other include.

// ACE headers
#include <ace/OS_NS_unistd.h>

//
// Stock_Database
//
Stock_Database::Stock_Database (void)
  : active_ (false)
{
  const char *stock_names [] = {"MSFT", "INTEL", "IBM"};

  Stock::StockInfo * stock_info;

  for (int i = 0; i < sizeof (stock_names) / sizeof (const char *); i ++)
    {
      // @@ Shanshan - Why are we storing pointers to dynamically allocated memory here?
      // This memory is being leaked, as it is never deleted.  I am
      // not sure there is any advantage, as you are deep copying
      // anyway when you return a value in get_stock_info.
      stock_info = new Stock::StockInfo;

      stock_info->name = stock_names[i];
      stock_info->high = 70 + (ACE_OS::rand () % 30);
      stock_info->last = 65 + (ACE_OS::rand () % 15);
      stock_info->low = 60 + (ACE_OS::rand () % 20);

      this->stock_map_.insert (std::make_pair (std::string (stock_names[i]), stock_info));
    }
}

//
// ~Stock_Database
//
Stock_Database::~Stock_Database (void)
{
}

//
// instance_
//
Stock_Database *Stock_Database::instance_ = 0;

//
// instance
//
Stock_Database *Stock_Database::instance (void)
{
  // @@ Shanshan - Please consider using the double checked locking
  // optimization here, or use the ACE_SINGLETON macro (Please see me
  // for an example) to create the singleton methods.
  if (Stock_Database::instance_ == 0)
    Stock_Database::instance_ = new Stock_Database;
  return Stock_Database::instance_;
}

//
// get_stock_info
//
Stock::StockInfo * Stock_Database::get_stock_info (const char *name)
{
  // Locate the <stock_name> in the database.
  ACE_DEBUG ((LM_DEBUG, "*** message: searching the stock_map_ for the stock_name\n"));
  StockMap::iterator iter = this->stock_map_.find (std::string (name));

  if (iter == this->stock_map_.end())
    return 0;

  // Make a deep copy of the stock info. We do not want to clien
  // to delete our copy of the stock_info.
  Stock::StockInfo * stock_info = new Stock::StockInfo;
  *stock_info = *(iter->second);

  ACE_DEBUG ((LM_DEBUG, "*** message: returning stock_info to the client\n"));
  return stock_info;
}

//
// ini
//
void Stock_Database::init (RTCORBA::RTORB_ptr rt_orb)
{
  this->rt_orb_ = RTCORBA::RTORB::_duplicate (rt_orb);
  this->lock_ = this->rt_orb_->create_mutex ();

  if (!this->active_) {
    this->active_ = true;
    // Create the thread in suspended mode.
    // @@ (WO) Shanshan, please use the ACE_Task constuct here
    // instead of using the ACE_Thread functions directly. Please see
    // page 250 in the APG.
    ACE_Thread::spawn (Stock_Database::_thread_func, this, THR_NEW_LWP |
      THR_JOINABLE, &this->_thread_id, &this->_thread);
  }
}

//
// publish_stock_info
//
void Stock_Database::publish_stock_info (Stock::StockNameConsumer_ptr consumer)
{
  // @@ Shanshan, please consider using a var type here to manage your memory.
  OBV_Stock::StockName * stock_name = new OBV_Stock::StockName;

  // @@ Shanshan, please use ++iter instead of iter++, as ++iter is
  // slightly faster as it does not make a copy.
  for (StockMap::iterator iter = this->stock_map_.begin ();
    iter != this->stock_map_.end(); iter ++)
  {
    stock_name->name (iter->first.c_str());
    consumer->push_StockName (stock_name);
  }
}

//
// lock
//
void Stock_Database::lock (void)
{
  this->lock_->lock ();
}

//
// unlock
//
void Stock_Database::unlock (void)
{
  this->lock_->unlock ();
}

//
// term
//
void Stock_Database::term (void)
{
  this->rt_orb_->destroy_mutex (this->lock_);
}

//
// _thread_func
//
ACE_THR_FUNC_RETURN Stock_Database::_thread_func(void *param)
{
  // Extract the <Stock_StockDistributor_i> object pointer.
  Stock_Database *database = (Stock_Database *)param;

  // Continue looping while the stock distributor is active.
  while (database->active_) {
    // @@ Shanshan, please consider using the ACE_GUARD (APG 256) macros along
    // with an ACE mutex type instead of the RTORB mutex.
    database->lock ();
    for (Stock_Database::StockMap::iterator iter = database->stock_map_.begin ();
      iter != database->stock_map_.end (); iter ++)
    {
      // Determine whether the stock has increased of decreased.
      int mult = ACE_OS::rand ();
      mult = mult % 2 == 0 ? 1 : -1;

      // Determine the amount of change of the stock. We will only permi
      // a 5 point change at a time.
      int delta = mult * (ACE_OS::rand () % 5);

      // Calculate the new values for the stock.
      iter->second->last += delta;
      if (iter->second->last < iter->second->low)
        iter->second->low = iter->second->last;
      else if (iter->second->last > iter->second->high)
        iter->second->high = iter->second->last;
      else
        ;
    }

    // Unlock the database and sleep for one second.
    database->unlock ();
    ACE_OS::sleep (1);
  }
  return 0;
}
