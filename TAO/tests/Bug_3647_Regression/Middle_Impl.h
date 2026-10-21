#ifndef Bug_3647_Regression_Middle_Impl_h
#define Bug_3647_Regression_Middle_Impl_h

#include "TestS.h"

namespace Bug_3647_Regression
{
/**
 * @class Middle
 *
 * Implement the Bug_3647_Regression::Middle interface
 */
class Middle_Impl : public POA_Bug_3647_Regression::Middle
{
public:
  Middle_Impl (
      Backend_ptr backend,
      CORBA::ORB_ptr orb,
      bool verbose);
  Middle_Impl (Middle_Impl const &) = delete;
  Middle_Impl (Middle_Impl &&) = delete;
  Middle_Impl &operator= (Middle_Impl const &) = delete;
  Middle_Impl &operator= (Middle_Impl &&) = delete;
  ~Middle_Impl () override = default;

  void startup_test () override;
  void ping () override;

  void shutdown () override;

private:
  /// Keep a reference to the backend so we can call it, shutdown, etc.
  Backend_var backend_;

  /// Keep a reference to the ORB so we can shutdown the application.
  CORBA::ORB_var orb_;

  /// Use this flag to decide if the program should produce any output.
  bool verbose_;
};
} // namespace Bug_3647_Regression

#endif // Bug_3647_Regression_Middle_hpp
