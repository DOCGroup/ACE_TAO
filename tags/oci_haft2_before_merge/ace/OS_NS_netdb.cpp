// -*- C++ -*-
// $Id$

#include "ace/OS_NS_netdb.h"

ACE_RCSID(ace, OS_NS_netdb, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_netdb.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

#include "ace/os_include/net/os_if.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stropts.h"
#include "ace/OS_NS_sys_socket.h"

#if defined (VXWORKS)

struct hostent *
ACE_OS::gethostbyaddr (const char *addr, int length, int type)
{
  ACE_OS_TRACE ("ACE_OS::gethostbyaddr");

  if (length != 4 || type != AF_INET)
    {
      errno = EINVAL;
      return 0;
    }

  // not thread safe!
  static hostent ret;
  static char name [MAXNAMELEN + 1];
  static char *hostaddr[2];
  static char *aliases[1];

  if (::hostGetByAddr (*(int *) addr, name) != 0)
    {
      // errno will have been set to S_hostLib_UNKNOWN_HOST.
      return 0;
    }

  // Might not be official: just echo input arg.
  hostaddr[0] = (char *) addr;
  hostaddr[1] = 0;
  aliases[0] = 0;

  ret.h_name = name;
  ret.h_addrtype = AF_INET;
  ret.h_length = 4;  // VxWorks 5.2/3 doesn't define IP_ADDR_LEN;
  ret.h_addr_list = hostaddr;
  ret.h_aliases = aliases;

  return &ret;
}

struct hostent *
ACE_OS::gethostbyaddr_r (const char *addr, int length, int type,
                         hostent *result, ACE_HOSTENT_DATA buffer,
                         int *h_errnop)
{
  ACE_OS_TRACE ("ACE_OS::gethostbyaddr_r");
  if (length != 4 || type != AF_INET)
    {
      errno = EINVAL;
      return 0;
    }

  if (ACE_OS::netdb_acquire ())
    return 0;
  else
    {
      // buffer layout:
      // buffer[0-3]: h_addr_list[0], the first (and only) addr.
      // buffer[4-7]: h_addr_list[1], the null terminator for the h_addr_list.
      // buffer[8]: the name of the host, null terminated.

      // Call ::hostGetByAddr (), which puts the (one) hostname into
      // buffer.
      if (::hostGetByAddr (*(int *) addr, &buffer[8]) == 0)
        {
          // Store the return values in result.
          result->h_name = &buffer[8];  // null-terminated host name
          result->h_addrtype = AF_INET;
          result->h_length = 4;  // VxWorks 5.2/3 doesn't define IP_ADDR_LEN.

          result->h_addr_list = (char **) buffer;
          // Might not be official: just echo input arg.
          result->h_addr_list[0] = (char *) addr;
          // Null-terminate the list of addresses.
          result->h_addr_list[1] = 0;
          // And no aliases, so null-terminate h_aliases.
          result->h_aliases = &result->h_addr_list[1];
        }
      else
        {
          // errno will have been set to S_hostLib_UNKNOWN_HOST.
          result = 0;
        }
    }

  ACE_OS::netdb_release ();
  *h_errnop = errno;
  return result;
}

struct hostent *
ACE_OS::gethostbyname (const char *name)
{
  ACE_OS_TRACE ("ACE_OS::gethostbyname");

  // not thread safe!
  static hostent ret;
  static int first_addr;
  static char *hostaddr[2];
  static char *aliases[1];

  ACE_OSCALL (::hostGetByName ((char *) name), int, -1, first_addr);
  if (first_addr == -1)
    return 0;

  hostaddr[0] = (char *) &first_addr;
  hostaddr[1] = 0;
  aliases[0] = 0;

  // Might not be official: just echo input arg.
  ret.h_name = (char *) name;
  ret.h_addrtype = AF_INET;
  ret.h_length = 4;  // VxWorks 5.2/3 doesn't define IP_ADDR_LEN;
  ret.h_addr_list = hostaddr;
  ret.h_aliases = aliases;

  return &ret;
}

struct hostent *
ACE_OS::gethostbyname_r (const char *name, hostent *result,
                         ACE_HOSTENT_DATA buffer,
                         int *h_errnop)
{
  ACE_OS_TRACE ("ACE_OS::gethostbyname_r");

  if (ACE_OS::netdb_acquire ())
    return 0;
  else
    {
      int addr;
      ACE_OSCALL (::hostGetByName ((char *) name), int, -1, addr);

      if (addr == -1)
        {
          // errno will have been set to S_hostLib_UNKNOWN_HOST
          result = 0;
        }
      else
        {
          // Might not be official: just echo input arg.
          result->h_name = (char *) name;
          result->h_addrtype = AF_INET;
          result->h_length = 4;  // VxWorks 5.2/3 doesn't define IP_ADDR_LEN;

          // buffer layout:
          // buffer[0-3]: h_addr_list[0], pointer to the addr.
          // buffer[4-7]: h_addr_list[1], null terminator for the h_addr_list.
          // buffer[8-11]: the first (and only) addr.

          // Store the address list in buffer.
          result->h_addr_list = (char **) buffer;
          // Store the actual address _after_ the address list.
          result->h_addr_list[0] = (char *) &result->h_addr_list[2];
          result->h_addr_list[2] = (char *) addr;
          // Null-terminate the list of addresses.
          result->h_addr_list[1] = 0;
          // And no aliases, so null-terminate h_aliases.
          result->h_aliases = &result->h_addr_list[1];
        }
    }

  ACE_OS::netdb_release ();
  *h_errnop = errno;
  return result;
}

#endif /* VXWORKS */

// Include if_arp so that getmacaddr can use the
// arp structure.
#if defined (sun)
# include /**/ <net/if_arp.h>
#endif

int
ACE_OS::getmacaddress (struct macaddr_node_t *node)
{
  ACE_OS_TRACE ("ACE_OS::getmacaddress");

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

  /** Define a structure for use with the netbios routine */
  struct ADAPTERSTAT
  {
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
  };

  NCB         ncb;
  LANA_ENUM   lenum;
  unsigned char result;

  ACE_OS::memset (&ncb, 0, sizeof(ncb));
  ncb.ncb_command = NCBENUM;
  ncb.ncb_buffer  = ACE_reinterpret_cast (unsigned char*,&lenum);
  ncb.ncb_length  = sizeof(lenum);

  result = Netbios (&ncb);

  for(int i = 0; i < lenum.length; i++)
    {
      ACE_OS::memset (&ncb, 0, sizeof(ncb));
      ncb.ncb_command  = NCBRESET;
      ncb.ncb_lana_num = lenum.lana [i];

      /** Reset the netbios */
      result = Netbios (&ncb);

      if (ncb.ncb_retcode != NRC_GOODRET)
	{
	  return -1;
	}

      ADAPTERSTAT adapter;
      ACE_OS::memset (&ncb, 0, sizeof (ncb));
      ACE_OS::strcpy (ACE_reinterpret_cast (char*, ncb.ncb_callname), "*");
      ncb.ncb_command     = NCBASTAT;
      ncb.ncb_lana_num    = lenum.lana[i];
      ncb.ncb_buffer      = ACE_reinterpret_cast (unsigned char*, &adapter);
      ncb.ncb_length      = sizeof (adapter);

      result = Netbios (&ncb);

      if (result == 0)
	{
	  ACE_OS::memcpy (node->node,
			  adapter.adapt.adapter_address,
			  6);
	  return 0;
	}
    }
  return 0;
#elif defined (sun)

  /** obtain the local host name */
  char hostname [MAXHOSTNAMELEN];
  ACE_OS::hostname (hostname, sizeof (hostname));

  /** Get the hostent to use with ioctl */
  struct hostent *phost =
    ACE_OS::gethostbyname (hostname);

  if (phost == 0)
    {
      return -1;
    }

  ACE_HANDLE handle =
    ACE_OS::socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (handle == ACE_INVALID_HANDLE)
    {
      return -1;
    }

  char **paddrs = phost->h_addr_list;

  struct arpreq ar;

  struct sockaddr_in *psa =
    (struct sockaddr_in *)&(ar.arp_pa);

  ACE_OS::memset (&ar,
		  0,
		  sizeof (struct arpreq));

  psa->sin_family = AF_INET;

  ACE_OS::memcpy (&(psa->sin_addr),
		  *paddrs,
		  sizeof (struct in_addr));

  if (ACE_OS::ioctl (handle,
                     SIOCGARP,
                     &ar) == -1)
    {
      return -1;
    }

  ACE_OS::close (handle);

  ACE_OS::memcpy (node->node,
		  ar.arp_ha.sa_data,
		  6);

  return 0;

#elif defined (linux)

  struct ifreq ifr;

  ACE_HANDLE handle =
    ACE_OS::socket (PF_INET, SOCK_DGRAM, 0);

  if (handle == ACE_INVALID_HANDLE)
    {
      return -1;
    }

  ACE_OS::strcpy (ifr.ifr_name, "eth0");

  if (ACE_OS::ioctl (handle/*s*/, SIOCGIFHWADDR, &ifr) < 0)
    {
      ACE_OS::close (handle);
      return -1;
    }

  struct sockaddr* sa =
    (struct sockaddr *) &ifr.ifr_addr;

  ACE_OS::memcpy (node->node,
		  sa->sa_data,
                  6);

  return 0;

#else
  ACE_UNUSED_ARG (node);
  ACE_NOTSUP_RETURN (-1);
#endif
}

# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0) && defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS)
#   include "ace/OS_NS_Thread.h"
#   include "ace/Object_Manager_Base.h"
int
ACE_OS::netdb_acquire (void)
{
  return ACE_OS::thread_mutex_lock ((ACE_thread_mutex_t *)
    ACE_OS_Object_Manager::preallocated_object[
      ACE_OS_Object_Manager::ACE_OS_MONITOR_LOCK]);
}

int
ACE_OS::netdb_release (void)
{
  return ACE_OS::thread_mutex_unlock ((ACE_thread_mutex_t *)
    ACE_OS_Object_Manager::preallocated_object[
      ACE_OS_Object_Manager::ACE_OS_MONITOR_LOCK]);
}
# endif /* defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS) */
