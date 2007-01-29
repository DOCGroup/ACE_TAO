/* -*- c++ -*- */
// $Id$

#ifndef TERA_SS_STATE_READ_H
#define TERA_SS_STATE_READ_H

#include "ace/OS.h"
#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"
#include "jaws3/Protocol_Handler.h"

class TeraSS_State_READ : public JAWS_Protocol_State
{
public:

  int service (JAWS_Event_Completer *, void *data);

  JAWS_Protocol_State * transition ( const JAWS_Event_Result &result
                                   , void *data
                                   , void *act
                                   );

  static JAWS_Protocol_State * instance (void)
  {
    return ACE_Singleton<TeraSS_State_READ, ACE_SYNCH_MUTEX>::instance ();
  }

};

#endif /* TERA_SS_STATE_READ_H */
