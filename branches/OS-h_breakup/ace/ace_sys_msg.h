/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_msg.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_MSG_H
#define ACE_ACE_SYS_MSG_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ <sys/msg.h>

# if defined (ACE_LACKS_MSGBUF_T)
struct msgbuf {};
# endif /* ACE_LACKS_MSGBUF_T */


#endif /* ACE_ACE_SYS_MSG_H */
