/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    netdb.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_NETDB_H
#define PACE_NETDB_H


#include "pace/defines.h"
#include <netdb.h>


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE struct hostent * pace_gethostbyaddr (const char * addr,
                                                   int len,
                                                   int type);

  PACE_INLINE struct  hostent * pace_gethostbyaddr_r (const char * addr,
                                                      int length,
                                                      int type,
                                                      struct hostent * result,
                                                      char * buffer,
                                                      int buflen,
                                                      int * h_errnop);

  PACE_INLINE struct hostent * pace_gethostbyname (const char * name);

  PACE_INLINE struct hostent * pace_gethostbyname_r (const char * name,
                                                     struct hostent *result,
                                                     char * buffer,
                                                     int buflen,
                                                     int * h_errnop);

  PACE_INLINE struct hostent * pace_gethostent ();

  PACE_INLINE struct hostent * pace_gethostent_r (struct hostent * result,
                                                  char * buffer,
                                                  int buflen,
                                                  int * h_errnop);

  PACE_INLINE int pace_sethostent (int stayopen);

  PACE_INLINE int pace_endhostent ();

  PACE_INLINE struct servent * pace_getservbyname (const char * name,
                                                   const  char * proto);

  PACE_INLINE struct servent * pace_getservbyname_r (const char * name,
                                                     const char * proto,
                                                     struct servent * result,
                                                     char * buffer,
                                                     int buflen);

  PACE_INLINE struct servent * pace_getservbyport (int port,
                                                   const char * proto);

  PACE_INLINE struct servent * pace_getservbyport_r (int port,
                                                     const char *proto,
                                                     struct servent *result,
                                                     char *buffer,
                                                     int buflen);

  PACE_INLINE struct servent * pace_getservent ();

  PACE_INLINE struct servent * pace_getservent_r (struct servent * result,
                                                  char * buffer,
                                                  int buflen);

  PACE_INLINE int pace_setservent (int stayopen);

  PACE_INLINE int pace_endservent ();

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "pace/netdb.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_NETDB_H */
