/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for CygWin
// platforms using GNU C++.

#ifndef ACE_CONFIG_CYGWIN32_H
#define ACE_CONFIG_CYGWIN32_H
#include "ace/pre.h"

#include "ace/config-cygwin32-common.h"

#define ACE_HAS_AUTOMATIC_INIT_FINI

#define ACE_LACKS_INTTYPES_H
#define ACE_LACKS_STDINT_H
#define ACE_LACKS_STROPTS_H
#define ACE_LACKS_SYS_IPC_H
#define ACE_LACKS_SYS_MSG_H
#define ACE_LACKS_SYS_SEM_H
#define ACE_LACKS_UCONTEXT_H

#include "ace/post.h"
#endif /* ACE_CONFIG_CYGWIN32_H */
