
#ifndef INTERFACE_3_I_H_
#define INTERFACE_3_I_H_
#include /**/ "ace/pre.h"

#include "idl_3S.h"

class interface_3_i
  : public virtual POA_Skeleton_Inheritance::Interface_3
{
public:
  /// Constructor
  explicit interface_3_i (CORBA::ORB_ptr const orb);
  interface_3_i (interface_3_i const &) = delete;
  interface_3_i (interface_3_i &&) = delete;
  interface_3_i &operator= (interface_3_i const &) = delete;
  interface_3_i &operator= (interface_3_i &&) = delete;

  char *operation_1 () override;
  char *operation_2 () override;
  char *operation_3 () override;

  void shutdown () override;

  ::CORBA::Long attribute_1 () override;
  void attribute_1 (::CORBA::Long attribute_1) override;

  char *attribute_2 () override;

  ::CORBA::Short attribute_3 () override;
  void attribute_3 (::CORBA::Short attribute_3) override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var const orb_ {};
};

#include /**/ "ace/post.h"
#endif /* INTERFACE_3_I_H_ */
