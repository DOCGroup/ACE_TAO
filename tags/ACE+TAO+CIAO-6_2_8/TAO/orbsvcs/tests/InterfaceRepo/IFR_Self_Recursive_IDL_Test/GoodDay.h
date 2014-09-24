//
// $Id$
//

#ifndef GOODDAY_H
#define GOODDAY_H
#include /**/ "ace/pre.h"

// Must include this header file and link to TAO_IFR_Client.lib
// to dynamically load this necessary library.
#include "tao/IFR_Client/IFR_Client_Adapter_Impl.h"

#include "TestS.h"

/// Implement the Test::Hello interface
class GoodDay
  : public virtual POA_Test::GoodDay
{
public:
  /// Constructor
  GoodDay (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual char * get_string (void);

  virtual void shutdown (void);

  virtual ::Test::GoodDay::RoughWeek * this_week (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* GOODDAY_H */
