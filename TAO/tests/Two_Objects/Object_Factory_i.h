// -*- C++ -*-
//
// $Id$

#ifndef OBJECT_FACTORY_I_H
#define OBJECT_FACTORY_I_H

#include "Two_ObjectsS.h"
#include "First_i.h"
#include "Second_i.h"

#include "ace/Auto_Event.h"


/// Implement the MTServer::Test interface
class Object_Factory_i
  : public virtual POA_Two_Objects_Test::Object_Factory
{
public:

  Object_Factory_i (CORBA::ORB_ptr orb,
                    CORBA::ULong len);

  Two_Objects_Test::First_ptr create_first (void);

  Two_Objects_Test::Second_ptr create_second (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  CORBA::ULong length_;
  ACE_Auto_Event two_way_done_;
};

#endif /* OBJECT_FACTORY_I_H */
