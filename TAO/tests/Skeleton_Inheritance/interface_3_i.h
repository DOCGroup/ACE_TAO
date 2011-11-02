//
// $Id$
//

#ifndef INTERFACE_3_I_H_
#define INTERFACE_3_I_H_
#include /**/ "ace/pre.h"

#include "idl_3S.h"
#include "interface_2_i.h"

class interface_3_i
  : public virtual interface_2_i
  , public virtual POA_Skeleton_Inheritance::Interface_3
{
public:
  /// Constructor
  interface_3_i (CORBA::ORB_ptr orb);

  virtual char * operation_3 (void);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* INTERFACE_3_I_H_ */
