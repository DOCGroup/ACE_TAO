/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    IO_Event_Masks.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_IO_EVENT_MASKS_H
#define TAO_ACE_IO_EVENT_MASKS_H
#include "ace/pre.h"
#include "TAO_ACE_export.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_ACE_IO_Event_Masks
 *
 * @brief Provides an abstract interface for handling socket events.
 *
 * @@ todo<Bala>: Write comments
 */

class TAO_ACE_Export TAO_ACE_IO_Event_Masks
{

public:
  enum
    {
    NULL_MASK = 0,
#if defined (ACE_USE_POLL)
    READ_MASK = POLLIN,
    WRITE_MASK = POLLOUT,
    EXCEPT_MASK = POLLPRI,
#else /* USE SELECT */
    READ_MASK = (1 << 0),
    WRITE_MASK = (1 << 1),
    EXCEPT_MASK = (1 << 2),
#endif /* ACE_USE_POLL */
    ACCEPT_MASK = (1 << 3),
    CONNECT_MASK = (1 << 4),
    ALL_EVENTS_MASK = READ_MASK |
                      WRITE_MASK |
                      EXCEPT_MASK |
                      ACCEPT_MASK |
                      CONNECT_MASK ,
    RWE_MASK = READ_MASK |
               WRITE_MASK |
               EXCEPT_MASK,
    // Mask to indicate that the reactor should not make an upcall
    // associated with any event if this mask is present.
    DONT_CALL = (1 << 5)
    };

  typedef unsigned long Event_Masks;
};



#include "ace/post.h"
#endif /*TAO_ACE_IO_EVENET_MASKS*/
