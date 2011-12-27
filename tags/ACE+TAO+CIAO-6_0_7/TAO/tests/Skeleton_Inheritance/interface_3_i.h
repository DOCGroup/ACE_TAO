//
// $Id$
//

#ifndef INTERFACE_3_I_H_
#define INTERFACE_3_I_H_
#include /**/ "ace/pre.h"

#include "idl_3S.h"

class interface_3_i
  : public virtual POA_Skeleton_Inheritance::Interface_3
{
public:
  /// Constructor
  interface_3_i (CORBA::ORB_ptr orb);

  virtual char * operation_1 (void);
  virtual char * operation_2 (void);
  virtual char * operation_3 (void);

  virtual void shutdown (void);

  virtual ::CORBA::Long attribute_1 (void);
  virtual void attribute_1 (::CORBA::Long attribute_1);

  virtual char * attribute_2 (void);

  virtual ::CORBA::Short attribute_3 (void);
  virtual void attribute_3 (::CORBA::Short attribute_3);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* INTERFACE_3_I_H_ */
