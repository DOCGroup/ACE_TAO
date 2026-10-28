#ifndef Stock_Quoter_i_h
#define Stock_Quoter_i_h

#include "Stock_QuoterS.h"

// This class provides the implementation for Stock_Quoter interface.
class Stock_Quoter_i : public virtual POA_Stock_Quoter
{
public:
  Stock_Quoter_i (CORBA::ORB_ptr orb);
  Stock_Quoter_i (Stock_Quoter_i const &) = delete;
  Stock_Quoter_i (Stock_Quoter_i &&) = delete;
  Stock_Quoter_i &operator= (Stock_Quoter_i const &) = delete;
  Stock_Quoter_i &operator= (Stock_Quoter_i &&) = delete;
  ~Stock_Quoter_i () override;

  CORBA::Float get_quote (char const *stock_id) override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#endif /* Stock_Quoter_i_h */
