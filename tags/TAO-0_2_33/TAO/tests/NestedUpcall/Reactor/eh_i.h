// -*- c++ -*-
// $Id$

#ifndef EVENTHANDLER_I_H
#  define EVENTHANDLER_I_H

#include "ReactorS.h"

class EventHandler_i : public POA_EventHandler
{
  // = TITLE
  //     Servant implementation for the <EventHandler> IDL interface.
public:
  EventHandler_i (void);
  // Constructor.

  virtual ~EventHandler_i (void);
  // Destructor.

  virtual CORBA::Long peer (CORBA::Environment &env);
  // Return some value...doesn't matter what.

  virtual CORBA::UShort decrement (Reactor_ptr eh,
                                   CORBA::UShort num,
                                   CORBA::Environment &env);
  // deccrement <num> by calling decrement thru <eh> until zero is
  // reached, then return.
};

#endif /* EVENTHANDLER_I_H */
