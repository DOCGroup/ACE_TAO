/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_netinet_in.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_NETINET_IN_H
# define ACE_ACE_NETINET_IN_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_NETINET_IN_H)
#   include /**/ <netinet/in.h>
# endif /* !ACE_LACKS_NETINET_IN_H */

# if !defined (IP_DROP_MEMBERSHIP)
#   define IP_DROP_MEMBERSHIP 0
# endif /* IP_DROP_MEMBERSHIP */

# if !defined (IP_ADD_MEMBERSHIP)
#   define IP_ADD_MEMBERSHIP 0
#   define ACE_LACKS_IP_ADD_MEMBERSHIP
# endif /* IP_ADD_MEMBERSHIP */

# if !defined (IP_DEFAULT_MULTICAST_TTL)
#   define IP_DEFAULT_MULTICAST_TTL 0
# endif /* IP_DEFAULT_MULTICAST_TTL */

# if !defined (IP_DEFAULT_MULTICAST_LOOP)
#   define IP_DEFAULT_MULTICAST_LOOP 0
# endif /* IP_DEFAULT_MULTICAST_LOOP */

# if !defined (IP_MULTICAST_IF)
#   define IP_MULTICAST_IF 0
# endif /* IP_MULTICAST_IF */

# if !defined (IP_MULTICAST_TTL)
#   define IP_MULTICAST_TTL 1
# endif /* IP_MULTICAST_TTL */

# if !defined (IP_MAX_MEMBERSHIPS)
#   define IP_MAX_MEMBERSHIPS 0
# endif /* IP_MAX_MEMBERSHIP */

# if !defined (SIOCGIFBRDADDR)
#   define SIOCGIFBRDADDR 0
# endif /* SIOCGIFBRDADDR */

# if !defined (SIOCGIFADDR)
#   define SIOCGIFADDR 0
# endif /* SIOCGIFADDR */

#endif /* ACE_ACE_NETINET_IN_H */
