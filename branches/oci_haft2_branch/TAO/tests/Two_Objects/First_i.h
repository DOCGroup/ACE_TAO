//
// $Id$
//


#ifndef TAO_TWO_OBJECTS_FIRST_I_H
#define TAO_TWO_OBJECTS_FIRST_I_H

#include "Two_ObjectsS.h"
#include "ace/Synch.h"

/// Implement the Two_Objects_Test::First interface
class First_i
  : public virtual POA_Two_Objects_Test::First
{
public:

  // Ctor..
  First_i (CORBA::ORB_ptr orb, ACE_Auto_Event &two_way_done);

  /// Interface methods..
  void oneway_method (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  ACE_Auto_Event &two_way_done_;
};

#endif /* TAO_TWO_OBJECTS_FIRST_I_H*/
