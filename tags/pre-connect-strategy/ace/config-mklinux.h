/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for MkLinux
// platforms using GNU C++.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#include "ace/config-linux-common.h"

#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_HAS_AUTOMATIC_INIT_FINI

#undef ACE_HAS_SOCKLEN_T
#define ACE_HAS_SIZET_SOCKET_LEN

#endif /* ACE_CONFIG_H */
