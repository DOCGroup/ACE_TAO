// -*- c++ -*-
// $Id$

#if !defined (EVENTHANDLER_I_H)
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
};

#endif /* EVENTHANDLER_I_H */
