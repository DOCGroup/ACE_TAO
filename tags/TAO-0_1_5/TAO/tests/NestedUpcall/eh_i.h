// -*- c++ -*-
//
// $Id$
//

#if !defined(EVENTHANDLER_I_H)
#  define EVENTHANDLER_I_H

#include "ReactorS.h"

class EventHandler_i : public POA_EventHandler
{
public:
  EventHandler_i (void);
  // CTOR

  virtual ~EventHandler_i (void);
  // DTOR

  virtual CORBA::Long peer (CORBA::Environment &env);
  // return some value...doesn't matter what
};

#endif /* EVENTHANDLER_I_H */
