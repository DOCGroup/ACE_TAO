#ifndef Bug_3647_Regression_Backend_Impl_h
#define Bug_3647_Regression_Backend_Impl_h

#include "TestS.h"

namespace Bug_3647_Regression
{
/**
 * @class Backend
 *
 * Implement the Bug_3647_Regression::Backend interface
 */
class Backend_Impl : public POA_Bug_3647_Regression::Backend
{
public:
  Backend_Impl (CORBA::ORB_ptr orb, bool verbose);
  Backend_Impl (Backend_Impl const &) = delete;
  Backend_Impl (Backend_Impl &&) = delete;
  Backend_Impl &operator= (Backend_Impl const &) = delete;
  Backend_Impl &operator= (Backend_Impl &&) = delete;
  ~Backend_Impl () override = default;

  void startup_test () override;
  void ping (Payload const &the_payload) override;
  void freeze (CORBA::ULong seconds) override;

  void shutdown () override;

private:
  /// Keep a reference to the ORB so we can shutdown the application.
  CORBA::ORB_var orb_;

  /// Use this flag to decide if the program should produce any output.
  bool verbose_;
};
} // namespace Bug_3647_Regression

#endif // Bug_3647_Regression_Backend_h
