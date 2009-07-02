#ifndef Bug_3647_Regression_Backend_Impl_h
#define Bug_3647_Regression_Backend_Impl_h
//
// $Id$
//

#include "TestS.h"

namespace Bug_3647_Regression
{

/**
 * @class Backend
 *
 * Implement the Bug_3647_Regression::Backend interface
 *
 */
class Backend_Impl : public POA_Bug_3647_Regression::Backend
{
public:
  Backend_Impl(CORBA::ORB_ptr orb, bool verbose);
  virtual ~Backend_Impl();

  virtual void startup_test();
  virtual void ping(Bug_3647_Regression::Payload const & the_payload);
  virtual void freeze(CORBA::ULong seconds);

  virtual void shutdown();

private:
  /// Keep a reference to the ORB so we can shutdown the application.
  CORBA::ORB_var orb_;

  /// Use this flag to decide if the program should produce any output.
  bool verbose_;
};

} // namespace Bug_3647_Regression

#endif // Bug_3647_Regression_Backend_h
