// $Id$

#include "RMCast_UDP_Proxy.h"
#include "RMCast_Module_Factory.h"
#include "ace/OS_Memory.h"

ACE_INLINE
ACE_RMCast_IO_UDP::
    ACE_RMCast_IO_UDP (ACE_RMCast_Module_Factory *factory)
  :  factory_ (factory),
     dgram_ (ACE_SOCK_Dgram_Mcast::OPT_BINDADDR_NO)
{
}

// Workaround failure in gcc for lynx hosted on solaris
// see ACE_RMCast_IO_UDP::handle_input() for more details
ACE_INLINE ACE_RMCast_UDP_Proxy *
ACE_RMCast_IO_UDP::allocate_and_bind_proxy (ACE_RMCast_Module *module,
                                            const ACE_INET_Addr& from_address)
{
  ACE_RMCast_UDP_Proxy *proxy;
  ACE_NEW_RETURN (proxy,
                  ACE_RMCast_UDP_Proxy(this,
                                       from_address),
                  0);
  proxy->next (module);

  if (this->map_.bind (from_address, proxy) != 0)
    {
      // @@ LOG??
      return 0;
    }

  return proxy;
}


