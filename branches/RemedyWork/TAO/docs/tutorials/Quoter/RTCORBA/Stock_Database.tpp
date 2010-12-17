// $Id$

#ifndef STOCK_DATABASE_TPP
#define STOCK_DATABASE_TPP
// local headers
#include "Stock_Database.h"

// ACE headers
#include <ace/OS_NS_unistd.h>
#include <ace/UUID.h>

#include <fstream>
#include <vector>
template <typename CALLBACK>
struct Map_Init
{
  Map_Init (typename Stock_Database<CALLBACK>::Stock_Map &map)
    : map_ (map)
  {
  }

  void operator () (const typename Stock_Database<CALLBACK>::Init_Map::value_type &item)
  {
    typename Stock_Database<CALLBACK>::StockInfo stock_info (item.first.c_str ());

    // If the initial value is nonzero, use that - otherwise, use a number
    // between 0 and 100
    stock_info.low_ =
      stock_info.last_ =
      stock_info.high_ = item.second ? item.second : ACE_OS::rand () % 100;

    map_[item.first] = stock_info;
  }

  typename Stock_Database<CALLBACK>::Stock_Map &map_;
};

// Stock_Database
template <typename CALLBACK>
Stock_Database<CALLBACK>::Stock_Database (u_int rate)
  : rate_ (rate),
    active_ (false)
{
  Init_Map map;
  map["IBM"] = 0;
  map["MSFT"] = 0;
  map["INTEL"] = 0;

  std::for_each (map.begin (),
                 map.end (),
                 Map_Init<CALLBACK> (this->stock_map_));
}

template <typename CALLBACK>
Stock_Database<CALLBACK>::Stock_Database (const char *file, u_int rate)
  : filename_ (file),
    rate_ (rate),
    active_ (false)
{
  this->handle_signal (0, 0, 0);
}

template <typename CALLBACK>
Stock_Database<CALLBACK>::Stock_Database (const Init_Map &stockmap,
                                          u_int rate)
  : rate_ (rate),
    active_ (false)
{
  std::for_each (stockmap.begin (),
                 stockmap.end (),
                 Map_Init<CALLBACK> (this->stock_map_));
}

// get_stock_info
template <typename CALLBACK>
typename Stock_Database<CALLBACK>::StockInfo
Stock_Database<CALLBACK>::get_stock_info(const char *name)
{
  ACE_READ_GUARD_RETURN (ACE_RW_Thread_Mutex,
                         guard,
                         this->lock_,
                         StockInfo ("Error"));

  // Locate the <stock_name> in the database.
  ACE_DEBUG ((LM_DEBUG,
              "*** message: searching the stock_map_ for the stock_name\n"));

  typename Stock_Map::iterator iter = this->stock_map_.find (std::string (name));

  if (iter == this->stock_map_.end ())
    throw Invalid_Stock ();

  ACE_DEBUG ((LM_DEBUG, "*** message: returning stock_info to the client\n"));

  return iter->second;
}

template <typename CALLBACK>
typename Stock_Database<CALLBACK>::Cookie
Stock_Database<CALLBACK>::register_callback (CALLBACK &obj)
{
  ACE_Utils::UUID uuid;
  ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);

  this->callbacks_[uuid.to_string ()->c_str ()] = &obj;

  return uuid.to_string ()->c_str ();
}

template <typename CALLBACK>
void
Stock_Database<CALLBACK>::update_rate (u_int rate)
{
  this->rate_ = rate;
}

template <typename CALLBACK>
void
Stock_Database<CALLBACK>::start (void)
{
  if (!this->active_)
    { // Double checked locking
      ACE_WRITE_GUARD (ACE_RW_Thread_Mutex,
                       guard,
                       this->lock_);
      if (!this->active_)
        {
          this->active_ = true;
          this->activate (THR_NEW_LWP | THR_JOINABLE, 1);
        }
    }
}

template <typename CALLBACK>
void
Stock_Database<CALLBACK>::stop (void)
{
  ACE_WRITE_GUARD (ACE_RW_Thread_Mutex,
                          guard,
                          this->lock_);

  this->active_ = false;
}

template <typename CALLBACK>
int
Stock_Database<CALLBACK>::handle_signal (int,
                                         siginfo_t *,
                                         ucontext_t *)
{
  ACE_WRITE_GUARD_RETURN (ACE_RW_Thread_Mutex,
                          guard,
                          this->lock_,
                          -1);

  std::ifstream input (this->filename_.c_str ());

  std::string name;
  u_int value = 0;

  typename Stock_Database<CALLBACK>::Init_Map map;

  while (input.good ())
    {
      input >> name;
      input >> value;
      map[name] = value;
    }

  input.close ();

  std::for_each (map.begin (),
                 map.end (),
                 Map_Init<CALLBACK> (this->stock_map_));

  return 0;
}


template <typename CALLBACK>
struct Stock_Updater
{
  void operator () (typename Stock_Database<CALLBACK>::Stock_Map::value_type &item)
  {
    // Determine if the stock has changed.
    if (ACE_OS::rand () % 2)
      return; // Nope! On to the next!

    changed_.push_back (item.first);

    // Determine the amount of change of the stock. We will
    // only permit a 5 point change at a time in either direction
    int delta = ACE_OS::rand () % 10 - 5;

    // We don't want negative stock values!
    if (item.second.last_ <= delta)
      delta *= -1;

    // Calculate the new values for the stock.
    item.second.last_ += delta;

    if (item.second.last_ < item.second.low_)
      item.second.low_ = item.second.last_;
    else if (item.second.last_ > item.second.high_)
      item.second.high_ = item.second.last_;
  }

  void operator () (typename Stock_Database<CALLBACK>::Callback_Map::value_type &item)
  {
    (*item.second)  (changed_);
  }

private:
  std::vector <std::string> changed_;
};

template <typename CALLBACK>
int
Stock_Database<CALLBACK>::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "tock!\n"));

  while (this->active_)
    {
      {
        // Init our functor
        Stock_Updater<CALLBACK> updater;

        { // Control the scope of our mutex to avoid deadlock.
          ACE_WRITE_GUARD_RETURN (ACE_RW_Thread_Mutex,
                                  guard,
                                  this->lock_,
                                  -1);

          updater = std::for_each (this->stock_map_.begin (),
                                   this->stock_map_.end (),
                                   updater);
        }

        // Update stock prices
        // notify callbacks
        std::for_each (this->callbacks_.begin (),
                       this->callbacks_.end (),
                       updater);
      }

      // Sleep for one second.
      ACE_OS::sleep (this->rate_);
    }

  return 0;
}

#endif /* STOCK_DATABASE_TPP */
