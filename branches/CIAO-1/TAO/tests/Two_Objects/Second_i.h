//
// $Id$
//

#ifndef TAO_TWO_OBJECTS_SECOND_I_H
#define TAO_TWO_OBEJCTS_SECOND_I_H

#include "Two_ObjectsS.h"

/// Implement the POA_Two_Objects_Test::Second interface
class Second_i
  : public virtual POA_Two_Objects_Test::Second
{
public:

  Second_i ( CORBA::ORB_ptr orb,
             CORBA::ULong len, ACE_Auto_Event &two_way_done);

  Two_Objects_Test::Octet_Seq *
    twoway_method (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  CORBA::ULong length_;
  ACE_Auto_Event &two_way_done_;
};

#endif /* TAO_TWO_OBEJCTS_SECOND_I_H*/
