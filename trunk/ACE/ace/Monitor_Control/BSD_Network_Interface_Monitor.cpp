// $Id$

#include "ace/Monitor_Control/BSD_Network_Interface_Monitor.h"

#if defined (__FreeBSD__) || defined (__NetBSD__) || defined (__OpenBSD__)

#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sockio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    BSD_Network_Interface_Monitor::BSD_Network_Interface_Monitor (
      const ACE_TCHAR *lookup_str)
      : value_ (0UL),
        lookup_str_ (lookup_str)
    {
    }

    void
    BSD_Network_Interface_Monitor::update_i (void)
    {
      ACE_UINT64 count = 0;
      int fd = socket (AF_INET, SOCK_DGRAM, 0);
      
      if (fd == -1) 
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("socket failed\n")));
          return;
        }

      struct ifaddrs *ifa, *ifap;
      
      if (getifaddrs (&ifap) < 0) 
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("getifaddrs failed\n")));
          close (fd);
          return;     
        }

      char *p = 0;
      
      for (ifa = ifap; ifa != 0; ifa = ifa->ifa_next) 
        {
          if (p && strcmp (p, ifa->ifa_name) == 0)
            {
              continue;
            }
            
          p = ifa->ifa_name; 

          struct ifdatareq ifdr;
          memset (&ifdr, 0, sizeof (ifdr));
          strncpy (ifdr.ifdr_name, ifa->ifa_name, sizeof (ifdr));

          if (ioctl (fd, SIOCGIFDATA, &ifdr) == -1) 
            {
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("SIOCGIFDATA failed\n")));
            }

          struct if_data * const ifi = &ifdr.ifdr_data;

          if (this->lookup_str_ == "ibytes")
            {
              count += ifi->ifi_ibytes;
            }
          else if (this->lookup_str_ == "ipackets")
            {
              count += ifi->ifi_ipackets;
            }
          else if (this->lookup_str_ == "obytes")
            {
              count += ifi->ifi_obytes;
            }
          else if (this->lookup_str_ == "opackets")
            {
              count += ifi->ifi_opackets;
            }
        }

      freeifaddrs (ifap);
      close (fd);
      value_ = count;
    }

    void
    BSD_Network_Interface_Monitor::clear_impl (void)
    {
      /// It looks like update_i() is getting the value since
      /// the machine was last rebooted. If that's the case,
      /// we need to get a value in the constructor and subtract
      /// that from subsequent values obtained by the code in
      /// udpate_i(). This method would then reset that constructor
      /// value.
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* defined (__FreeBSD__) || defined (__NetBSD__) || defined (__OpenBSD__) */
