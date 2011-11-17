// -*- C++ -*-
//
// $Id$

#ifndef TAO_TWO_OBJECTS_SECOND_I_H
#define TAO_TWO_OBEJCTS_SECOND_I_H

#include "Two_ObjectsS.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Auto_Event;
ACE_END_VERSIONED_NAMESPACE_DECL

/// Implement the POA_Two_Objects_Test::Second interface
class Second_i
  : public virtual POA_Two_Objects_Test::Second
{
public:

  Second_i ( CORBA::ORB_ptr orb,
             CORBA::ULong len, ACE_Auto_Event &two_way_done);

  Two_Objects_Test::Octet_Seq *
    twoway_method (void);

  void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  CORBA::ULong length_;
  ACE_Auto_Event &two_way_done_;
};

#endif /* TAO_TWO_OBEJCTS_SECOND_I_H*/
