/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SOCK_Dgram_Mcast.h
 *
 *  $Id$
 *
 *  @author Irfan Pyrali <irfan@cs.wustl.edu>
 *  @author Tim Harrison <harrison@cs.wustl.edu>
 *  @author Bill Fulton <bill_a_fulton@raytheon.com>
 *  @author and Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_SOCK_DGRAM_MCAST_H
#define ACE_SOCK_DGRAM_MCAST_H
#include "ace/pre.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include  "ace/SOCK_Dgram_Mcast_T.h"

/**
 * @class ACE_SOCK_Dgram_Mcast
 *
 * @brief Explicit specialization of ACE_SOCK_Dgram_Mcast_Ex that
 * defines the legacy class signature.
 *
 * ACE_SOCK_Dgram_Mcast_Ex implements a multicast datagram interface.
 * See ACE_SOCK_Dgram_Mcast_Ex for the generic class interface definition.
 */
class ACE_Export ACE_SOCK_Dgram_Mcast : 
    public ACE_SOCK_Dgram_Mcast_Ex <ACE_SDM_DEFOPT_LOCK>
{

};

#include "ace/post.h"
#endif /* ACE_SOCK_DGRAM_MCAST_H */
