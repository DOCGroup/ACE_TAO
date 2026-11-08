/**
 * @file Stock_Database.h
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Douglas C. Schmidt <schmidt@dre.vanderbilt.edu>
 */

#ifndef STOCK_DATABASE_H_
#define STOCK_DATABASE_H_

// ACE headers
#include "ace/Task.h"

// STL headers
#include <map>
#include <string>

/**
 * @class Stock_Database
 * @brief This class is used to install, update and publish the information of
 * all the stocks. It uses the singleton design pattern.
 * The parameter type may be any type that has a method called "updated_stocks" and
 * accepts a std::vector of std::strings as an argument.
 */
template <typename CALLBACK_TYPE>
class Stock_Database : public ACE_Task_Base
{
public:
  /// Default constructor
  /// Initializes the stock database with MSFT, IBM, and INTEL.
  /// @param rate The rate at which to perform updates.
  explicit Stock_Database (u_int rate = 1);

  /// Constructor.
  /// @param file The name of a file to read initial stocks and values from.
  explicit Stock_Database (std::string const &file, u_int rate = 1);

  using Init_Map = std::map<std::string, unsigned int>;

  /// Constructor
  /// @param stockmap  A map containing stocks and initial values.  An initial value
  /// of 0 will be assigned a random start value.
  explicit Stock_Database (Init_Map const &stockmap, u_int rate = 0);

  Stock_Database (Stock_Database const &) = delete;
  Stock_Database (Stock_Database &&) = delete;
  Stock_Database &operator= (Stock_Database const &) = delete;
  Stock_Database &operator= (Stock_Database &&) = delete;

  using Cookie = std::string;

  /**
   * Register a callback object with the database. The callback object must have
   * the () operator defined accepting a std::vector of strings as the argument.
   * @returns A cookie to identify the registration
   */
  Cookie register_callback (CALLBACK_TYPE &obj);

  /**
   * Removes a callback from the notification queue.
   * @returns false if the provided cookie is not found.
   */
  bool remove_callback (const Cookie &);

  /// Raised if an invalid stock name is requested.
  class Invalid_Stock {};

  struct StockInfo
  {
    StockInfo () = default;

    explicit StockInfo (std::string const &name)
      : name_ (name)
    {}

    std::string name_;
    int high_ {};
    int low_ {};
    int last_ {};
  };

  /**
   * Create a StockInfo object stored in the database with the given name.
   *
   * @param name The name of the stock.
   * @return A StockInfo object.
   */
  StockInfo get_stock_info (std::string const &name);

  /**
   * This function is used to calculate the new high, low and last values
   * for each stock in the stock database randomly.
   */
  int svc () override;

  /// Change the rate at which database updates are made
  void update_rate (u_int rate);

  /// Launch the active object
  void start ();

  /// Stop the active object
  void stop ();

  using Stock_Map = std::map<std::string, StockInfo>;

  /// This method is not intended to be called by clients of this class,
  /// it is public only by necessity.
  int handle_signal (int signum,
                     siginfo_t * = nullptr,
                     ucontext_t * = nullptr) override;

  using Callback_Map = std::map<Cookie, CALLBACK_TYPE *>;

private:
  /// The filname initialized from, if any.
  std::string const filename_;

  /// Keep track of the stock names and information about them.
  Stock_Map stock_map_;

  /// Lock to protect concurrent access to the <stock_map_>.
  ACE_RW_Thread_Mutex lock_;

  /// Rate at which updates are made.
  u_int rate_;

  /// Borrowed callback pointers; each callback must outlive its registration.
  Callback_Map callbacks_;

  bool active_ {};
};

#include "Stock_Database.tpp"

#endif  // !defined STOCK_DATABASE_H_
