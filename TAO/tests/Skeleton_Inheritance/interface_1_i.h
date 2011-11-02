//
// $Id$
//

#ifndef INTERFACE_1_I_H_
#define INTERFACE_1_I_H_
#include /**/ "ace/pre.h"

#include "idl_1S.h"

class interface_1_i
  : public virtual POA_Skeleton_Inheritance::Interface_1
{
public:
  /// Constructor
  interface_1_i (CORBA::ORB_ptr orb);

  virtual char * operation_1 (void);

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* INTERFACE_1_I_H_ */
