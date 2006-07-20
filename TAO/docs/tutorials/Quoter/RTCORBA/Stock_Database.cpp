// $Id$

// ACE headers
#include <ace/OS_NS_unistd.h>

// local headers
#include "Stock_Database.h"

//
// Stock_Database
//
Stock_Database::Stock_Database (void)
{
  const char *stock_names [] = {"MSFT", "INTEL", "IBM"};

  Stock::StockInfo_var stock_info; 

  for (int i = 0; i < sizeof (stock_names) / sizeof (const char *); ++i) 
    {
      stock_info = new Stock::StockInfo;
      
      stock_info->name = stock_names[i];
      stock_info->high = 70 + (ACE_OS::rand () % 30);
      stock_info->last = 65 + (ACE_OS::rand () % 15);
      stock_info->low = 60 + (ACE_OS::rand () % 20);

      this->stock_map_.insert (std::make_pair (std::string (stock_names[i]), stock_info));
    }
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

  ACE_DEBUG ((LM_DEBUG, "*** message: returning stock_info to the client\n"));

  Stock::StockInfo_var stock_info = new Stock::StockInfo;
  stock_info->name = std::string (iter->second->name).c_str ();
  stock_info->high = iter->second->high;
  stock_info->last = iter->second->last;
  stock_info->low = iter->second->low;

  return stock_info._retn();;
}

//
// publish_stock_info
//
void Stock_Database::publish_stock_info (Stock::StockNameConsumer_ptr consumer)
{
  Stock::StockName_var stock_name = new OBV_Stock::StockName;

  for (StockMap::iterator iter = this->stock_map_.begin ();
    iter != this->stock_map_.end(); ++iter)
  {
    stock_name->name (iter->first.c_str());
    consumer->push_StockName (stock_name);
  }
}

int Stock_Database::svc (void)
{
  while (true) 
  {
    for (Stock_Database::StockMap::iterator iter = this->stock_map_.begin ();
      iter != this->stock_map_.end (); ++iter)
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
    }

    // Sleep for one second.
    ACE_OS::sleep (1);
  }
  return 0;
}
