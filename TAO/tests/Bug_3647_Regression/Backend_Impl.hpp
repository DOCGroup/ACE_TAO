#ifndef Bug_3647_Regression_Backend_Impl_hpp
#define Bug_3647_Regression_Backend_Impl_hpp

#include "TestS.h"
#include "Throw_Spec.h"

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

  virtual void startup_test()
      THROW_SPEC (CORBA::SystemException);
  virtual void ping(Bug_3647_Regression::Payload const & the_payload)
      THROW_SPEC (CORBA::SystemException);
  virtual void freeze(CORBA::ULong seconds)
      THROW_SPEC (CORBA::SystemException);

  virtual void shutdown()
      THROW_SPEC (CORBA::SystemException);

private:
  /// Keep a reference to the ORB so we can shutdown the application.
  CORBA::ORB_var orb_;

  /// Use this flag to decide if the program should produce any output.
  bool verbose_;
};

} // namespace Bug_3647_Regression

#endif // Bug_3647_Regression_Backend_hpp
