// -*- c++ -*-
// $Id$

#if !defined (REACTOR_I_H)
#  define REACTOR_I_H

#include "ReactorS.h"

class Reactor_i : public POA_Reactor
{
  // = TITLE
  //     Implement the <Reactor> IDL interface.
public:
  Reactor_i (void);
  // Constructor.

  virtual ~Reactor_i (void);
  // Destructor.

  virtual CORBA::Long register_handler(EventHandler_ptr eh,
                                       CORBA::Environment &env);
  // Register (with nothing...it's an example!)

  virtual void set_value (CORBA::Environment &env);
};

#endif /* REACTOR_I_H */
