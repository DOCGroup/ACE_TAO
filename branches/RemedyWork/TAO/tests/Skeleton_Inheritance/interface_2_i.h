//
// $Id$
//

#ifndef INTERFACE_2_I_H_
#define INTERFACE_2_I_H_
#include /**/ "ace/pre.h"

#include "idl_2S.h"
#include "interface_1_i.h"

class interface_2_i
  : public virtual interface_1_i
  , public virtual POA_Skeleton_Inheritance::Interface_2
{
public:
  /// Constructor
  interface_2_i (CORBA::ORB_ptr orb);

  virtual char * operation_2 (void);

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* INTERFACE_2_I_H_ */
