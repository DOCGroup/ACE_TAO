// This may look like C, but it's really -*- C++ -*-
// $Id$

// ===========================================================================
//
// = LIBRARY
//     TAO
//
// = AUTHOR
//     Carlos O'Ryan <coryan@uci.edu>,
//     Chad Elliott <elliott_c@ociweb.com>
//
// ===========================================================================

#ifndef TAO_BIND_DISPATCHER_GUARD_H
#define TAO_BIND_DISPATCHER_GUARD_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

class TAO_Reply_Dispatcher;
class TAO_Transport_Mux_Strategy;

class TAO_Bind_Dispatcher_Guard
{
public:
  TAO_Bind_Dispatcher_Guard (CORBA::ULong request_id,
                             TAO_Reply_Dispatcher* rd,
                             TAO_Transport_Mux_Strategy* tms);

  ~TAO_Bind_Dispatcher_Guard (void);

  int& status(void);

private:
  int                         status_;
  CORBA::ULong                request_id_;
  TAO_Reply_Dispatcher*       rd_;
  TAO_Transport_Mux_Strategy* tms_;
};

#if defined (__ACE_INLINE__)
# include "tao/Bind_Dispatcher_Guard.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif
