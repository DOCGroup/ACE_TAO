/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_stropts.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_STROPTS_H
#define ACE_ACE_STROPTS_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ <stropts.h>

# if defined (ACE_LACKS_STRRECVFD)
struct strrecvfd {};
# endif /* ACE_LACKS_STRRECVFD */

#endif /* ACE_ACE_STROPTS_H */
