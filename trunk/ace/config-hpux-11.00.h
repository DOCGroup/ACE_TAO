/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for HP
// platforms running HP-UX 11.00 using aC++, CC, or gcc (2.95 and up).

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#if defined (__GNUG__)

// config-g++-common.h undef's ACE_HAS_STRING_CLASS with -frepo, so
// this must appear before its #include.
# define ACE_HAS_STRING_CLASS

# include "ace/config-g++-common.h"

// gcc 2.95.2 supplies the ssize_t typedef.
# define ACE_HAS_SSIZE_T

# include "ace/config-hpux11.h"

#else

// It's one of the HP compilers, so use the old file. This should be
// folded into this file to avoid confusion with so many filenames.
// Undef ACE_CONFIG_H so the other file isn't ignored.
# undef ACE_CONFIG_H
# include "ace/config-hpux-11.x-hpc++.h"

#endif /* __GNUG__ */

#endif /* ACE_CONFIG_H */
