/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    netdb.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */


#include "pace/unistd.h"


PACE_INLINE
struct hostent *
pace_gethostbyaddr (const char * addr,
                    int len,
                    int type)
{
  return gethostbyaddr (addr, len, type);
}

PACE_INLINE
struct  hostent *
pace_gethostbyaddr_r (const char * addr,
                      int length,
                      int type,
                      struct hostent * result,
                      char * buffer,
                      int buflen,
                      int * h_errnop)
{
  return gethostbyaddr_r (addr, length, type, result, buffer, buflen, h_errnop);
}

PACE_INLINE
struct hostent *
pace_gethostbyname (const char * name)
{
  return gethostbyname (name);
}

PACE_INLINE
struct hostent *
pace_gethostbyname_r (const char * name,
                      struct hostent *result,
                      char * buffer,
                      int buflen,
                      int * h_errnop)
{
  return gethostbyname_r (name, result, buffer, buflen, h_errnop);
}

PACE_INLINE
struct hostent *
pace_gethostent ()
{
  return gethostent ();
}

PACE_INLINE
struct hostent *
pace_gethostent_r (struct hostent * result,
                   char * buffer,
                   int buflen,
                   int * h_errnop)
{
  return gethostent_r (result, buffer, buflen, h_errnop);
}

PACE_INLINE
int
pace_sethostent (int stayopen)
{
  return sethostent (stayopen);
}

PACE_INLINE
int
pace_endhostent ()
{
  return endhostent ();
}

PACE_INLINE
struct servent *
pace_getservbyname (const char * name,
                    const  char * proto)
{
  return getservbyname (name, proto);
}

PACE_INLINE
struct servent *
pace_getservbyname_r (const char * name,
                      const char * proto,
                      struct servent * result,
                      char *buffer,
                      int buflen)
{
  return getservbyname_r (name, proto, result, buffer, buflen);
}

PACE_INLINE
struct servent *
pace_getservbyport (int port, const char * proto)
{
  return getservbyport (port, proto);
}

PACE_INLINE
struct servent *
pace_getservbyport_r (int port,
                      const char * proto,
                      struct servent * result,
                      char * buffer,
                      int buflen)
{
  return getservbyport_r (port, proto, result, buffer, buflen);
}

PACE_INLINE
struct servent *
pace_getservent ()
{
  return getservent ();
}

PACE_INLINE
struct servent *
pace_getservent_r (struct servent * result,
                   char * buffer,
                   int buflen)
{
  return getservent_r (result, buffer, buflen);
}

PACE_INLINE
int
pace_setservent (int stayopen)
{
  return setservent (stayopen);
}

PACE_INLINE
int
pace_endservent ()
{
  return endservent ();
}
