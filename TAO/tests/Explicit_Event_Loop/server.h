// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Explicit_Event_Loop
//
// = FILENAME
//    server.h
//
// = AUTHORS
//   Source code used in TAO has been modified and adapted from the code
//   provided in the book, "Advanced CORBA Programming with C++" by Michi
//   Henning and Steve Vinoski. Copyright 1999. Addison-Wesley, Reading,
//   MA.  Used with permission of Addison-Wesley.
//
//   Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
//
// ============================================================================

#ifndef server_HH_
#define server_HH_

#include "timeS.h"

class Time_impl : public virtual POA_Time
{
public:
  virtual TimeOfDay get_gmt (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#endif /* server_HH_ */
