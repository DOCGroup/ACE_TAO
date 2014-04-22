// $Id$

#ifndef Stock_Quoter_i_h
#define Stock_Quoter_i_h

#include "Stock_QuoterS.h"

// This class provides the implementation for Stock_Quoter interface.
class Stock_Quoter_i : public virtual POA_Stock_Quoter
{
public:
   Stock_Quoter_i (CORBA::ORB_ptr orb);
   virtual ~Stock_Quoter_i ();

   CORBA::Float get_quote (const char* stock_id);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#endif /* Stock_Quoter_i_h */
