// -*- c++ -*-
//
// $Id$
//

#if !defined(REACTOR_I_H)
#  define REACTOR_I_H

#include "ReactorS.h"

// Forward decls
class Reactor_i;

typedef Reactor_i *Reactor_i_ptr; // why necessary?

class Reactor_i : public POA_Reactor
{
public:
  Reactor_i (void);
  // CTOR

  virtual ~Reactor_i (void);
  // DTOR

  virtual CORBA::Long register_handler(EventHandler_ptr eh,
                                       CORBA::Environment &env);
  // register (with nothing...it's an example!)
};

#endif /* REACTOR_I_H */
