
//=============================================================================
/**
 *  @file    INET_Addr_Test.cpp
 *
 *   Performs several tests on the ACE_INET_Addr class.  It creates several
 *   IPv4 and IPv6 addresses and checks that the address formed by the
 *   class is valid.
 *
 *  @author John Aughey (jha@aughey.com)
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_arpa_inet.h"
#include "ace/SString.h"
#include "ace/OS_NS_unistd.h"

// Make sure that ACE_Addr::addr_type_ is the same
// as the family of the inet_addr_.
static int check_type_consistency (const ACE_INET_Addr &addr)
{
  int family = -1;

  if (addr.get_type () == AF_INET)
    {
      struct sockaddr_in *sa4 = (struct sockaddr_in *)addr.get_addr();
      family = sa4->sin_family;
    }
#if defined (ACE_HAS_IPV6)
  else if (addr.get_type () == AF_INET6)
    {
      struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)addr.get_addr();
      family = sa6->sin6_family;
    }
#endif

  if (addr.get_type () != family)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Inconsistency between ACE_SOCK::addr_type_ (%d) ")
                  ACE_TEXT ("and the sockaddr family (%d)\n"),
                  addr.get_type (),
                  family));
      return 1;
    }
  return 0;
}

static bool test_tao_use (void)
{
  char host[256];
  if (::gethostname (host, 255) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Test TAO Use fail %p\n"),
                  ACE_TEXT ("gethostname")));
      return false;
    }

  ACE_INET_Addr addr;
  addr.set ((unsigned short)0, host);

  ACE_CString full (host);
  full += ":12345";

  addr.set (full.c_str ());

  u_short p = addr.get_port_number ();

  if (p != 12345)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Test TAO Use expected port 12345 got %d\n"),
                  p));
      return false;
   }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Test TAO Use passed\n")));
  return true;
}

static bool test_port_assignment (void)
{
#if defined (ACE_HAS_IPV6)
  ACE_INET_Addr addr1 (static_cast<unsigned short> (0), ACE_IPV6_ANY, AF_INET6);
  ACE_INET_Addr addr2;

  addr1.set_port_number (12345);
  addr2.set (addr1);
  if (addr1.get_port_number () != addr2.get_port_number ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("port number not properly copied. ")
                  ACE_TEXT ("addr1 port = %d addr2 port = %d\n"),
                  addr1.get_port_number (), addr2.get_port_number ()));
      return false;
    }
   ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Test Port Assignment passed\n")));
#else
   ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Test Port Assignment is IPv6 only\n")));
#endif /* ACE_HAS_IPV6 */
  return true;
}

static bool test_multiple (void)
{

  bool success = true;

  // Check the behavior when there are multiple addresses assigned to a name.
  // The NTP pool should always return multiples, though always different.
  ACE_INET_Addr ntp;
  if (ntp.set (123, ACE_TEXT ("pool.ntp.org")) == -1)
    {
      // This is just a warning to prevent fails on lookups on hosts with no
      // DNS service. The real value of this test is to accurately get
      // multiples from the result.
      ACE_ERROR ((LM_WARNING, ACE_TEXT ("%p\n"), ACE_TEXT ("pool.ntp.org")));
      return true;
    }
  size_t count = 0;
  ACE_TCHAR addr_string[256];
  do
    {
      ++count;      // If lookup succeeded, there's at least one
      ntp.addr_to_string (addr_string, sizeof (addr_string));
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("IPv4 %B: %s\n"), count, addr_string));
    }
  while (ntp.next ());
  success = count > 1;

#if defined (ACE_HAS_IPV6)
  ACE_INET_Addr ntp6;
  if (ntp6.set (123, ACE_TEXT ("2.pool.ntp.org"), 1, AF_INET6) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("2.pool.ntp.org")));
      return false;
    }
  count = 0;
  do
    {
      ++count;      // If lookup succeeded, there's at least one
      ntp6.addr_to_string (addr_string, sizeof (addr_string));
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("IPv6 %B: %s\n"), count, addr_string));
    }
  while (ntp6.next ());
  if (count <= 1)
    success = false;
#endif /* ACE_HAS_IPV6 */

  return success;
}

struct Address {
  const char* name;
  bool loopback;
};

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("INET_Addr_Test"));

  int status = 0;     // Innocent until proven guilty

  // Try to set up known IP and port.
  u_short port (80);
  ACE_UINT32 const ia_any = INADDR_ANY;
  ACE_INET_Addr local_addr(port, ia_any);
  status |= check_type_consistency (local_addr);
  if (local_addr.get_port_number () != 80)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Got port %d, expecting 80\n"),
                  (int)(local_addr.get_port_number ())));
      status = 1;
    }
  if (local_addr.get_ip_address () != ia_any)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Mismatch on local IP addr\n")));
      status = 1;
    }

  // Assignment constructor
  ACE_INET_Addr local_addr2 (local_addr);
  status |= check_type_consistency (local_addr2);
  if (local_addr2.get_port_number () != 80)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Copy got port %d, expecting 80\n"),
                  (int)(local_addr2.get_port_number ())));
      status = 1;
    }
  if (local_addr2.get_ip_address () != ia_any)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Mismatch on copy local IP addr\n")));
      status = 1;
    }
  if (local_addr != local_addr2)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Copy local addr mismatch\n")));
      status = 1;
    }

  // Try to parse out a simple address:port string. Intentionally reuse
  // the ACE_INET_Addr to ensure resetting an address works.
  const char *addr_ports[] =
    {
      "127.0.0.1:80", "www.dre.vanderbilt.edu:80", 0
    };
  ACE_INET_Addr addr_port;
  for (int i = 0; addr_ports[i] != 0; ++i)
    {
      if (addr_port.set (addr_ports[i]) == 0)
        {
          status |= check_type_consistency (addr_port);
          if (addr_port.get_port_number () != 80)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Got port %d from %s\n"),
                          (int)(addr_port.get_port_number ()),
                          addr_ports[i]));
              status = 1;
            }
          ACE_INET_Addr check (addr_ports[i]);
          if (addr_port != check)
            {
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("Reset on iter %d failed\n"), i));

              if (addr_port.get_type() != check.get_type()) {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("  addr_port.get_type()= %d, check.get_type()=%d\n")
                  , addr_port.get_type(), check.get_type()));
              }
              if (addr_port.get_size() != check.get_size()) {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("  addr_port.get_size()= %d, check.get_size()=%d\n")
                  , addr_port.get_size(), check.get_size()));
              }
#if defined(ACE_HAS_IPV6)
              if (addr_port.get_type() == check.get_type() && addr_port.get_size() == check.get_size()){
                if (addr_port.get_type() == AF_INET6) {
                  const struct sockaddr_in6 *addr_port_in6 =
                    static_cast<const struct sockaddr_in6*> (addr_port.get_addr());
                  const struct sockaddr_in6 *check_in6 =
                    static_cast<const struct sockaddr_in6*> (check.get_addr());
# if defined(AIX)

                  ACE_ERROR((LM_ERROR, ACE_TEXT ("  addr_port_in6->sin6_len=%d, check_in6->sin6_len=%d\n")
                    , (int)addr_port_in6->sin6_len, (int)check_in6->sin6_len));
# endif

                  ACE_ERROR((LM_ERROR, ACE_TEXT ("  addr_port_in6->sin6_family=%d, check_in6->sin6_family=%d\n")
                    , (int)addr_port_in6->sin6_family, (int)check_in6->sin6_family));

                  ACE_ERROR((LM_ERROR, ACE_TEXT ("  addr_port_in6->sin6_port=%d, check_in6->sin6_port=%d\n")
                    , (int)addr_port_in6->sin6_port, (int)check_in6->sin6_port));

                  ACE_ERROR((LM_ERROR, ACE_TEXT ("  addr_port_in6->sin6_flowinfo=%d, check_in6->sin6_flowinfo=%d\n")
                    , (int)addr_port_in6->sin6_flowinfo, (int)check_in6->sin6_flowinfo));

                  ACE_ERROR((LM_ERROR, ACE_TEXT ("  addr_port_in6->sin6_scope_id=%d, check_in6->sin6_scope_id=%d\n")
                    , (int)addr_port_in6->sin6_scope_id, (int)check_in6->sin6_scope_id));

                  ACE_ERROR((LM_DEBUG, ACE_TEXT ("  addr_port_in6->sin6_addr=")));
                  ACE_HEX_DUMP((LM_DEBUG, reinterpret_cast<const char*>(&addr_port_in6->sin6_addr), sizeof(addr_port_in6->sin6_addr)));
                  ACE_ERROR((LM_DEBUG, ACE_TEXT ("  check_in6->sin6_addr=")));
                  ACE_HEX_DUMP((LM_DEBUG, reinterpret_cast<const char*>(&check_in6->sin6_addr), sizeof(check_in6->sin6_addr)));
                }
              }
#endif
              status = 1;
            }
        }
      else
        {
          // Sometimes this fails because the run-time host lacks the capability to
          // resolve a name. But it shouldn't fail on the first one, 127.0.0.1.
          if (i == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("%C: %p\n"),
                          addr_ports[i],
                          ACE_TEXT ("lookup")));
              status = 1;
            }
          else
            {
              ACE_ERROR ((LM_WARNING,
                          ACE_TEXT ("%C: %p\n"),
                          addr_ports[i],
                          ACE_TEXT ("lookup")));
            }
        }
    }

  const char *ipv4_addresses[] =
    {
      "127.0.0.1", "138.38.180.251", "64.219.54.121", "192.0.0.1", "10.0.0.1", 0
    };

  ACE_INET_Addr addr;
  status |= check_type_consistency (addr);
  char hostaddr[1024];

  for (int i=0; ipv4_addresses[i] != 0; i++)
    {
      struct in_addr addrv4;
      ACE_OS::memset ((void *) &addrv4, 0, sizeof addrv4);
      ACE_UINT32 addr32;

      ACE_OS::inet_pton (AF_INET, ipv4_addresses[i], &addrv4);

      ACE_OS::memcpy (&addr32, &addrv4, sizeof (addr32));

      status |= !(addr.set (80, ipv4_addresses[i]) == 0);
      status |= check_type_consistency (addr);

      /*
      ** Now check to make sure get_ip_address matches and get_host_addr
      ** matches.
      */
      if (addr.get_ip_address () != ACE_HTONL (addr32))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Error: %C failed get_ip_address() check\n")
                      ACE_TEXT ("0x%x != 0x%x\n"),
                      ipv4_addresses[i],
                      addr.get_ip_address (),
                      ACE_HTONL (addr32)));
          status = 1;
        }

      if (addr.get_host_addr () != 0 &&
          ACE_OS::strcmp (addr.get_host_addr(), ipv4_addresses[i]) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%C failed get_host_addr() check\n")
                      ACE_TEXT ("%C != %C\n"),
                      ipv4_addresses[i],
                      addr.get_host_addr (),
                      ipv4_addresses[i]));
          status = 1;
        }

      // Now we check the operation of get_host_addr(char*,int)
      const char* haddr = addr.get_host_addr (&hostaddr[0], sizeof(hostaddr));
      if (haddr != 0 &&
          ACE_OS::strcmp (&hostaddr[0], haddr) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%C failed get_host_addr(char* buf,int) check\n")
                      ACE_TEXT ("buf ['%C'] != return value ['%C']\n"),
                      ipv4_addresses[i],
                      &hostaddr[0],
                      haddr));
          status = 1;
        }
      if (ACE_OS::strcmp (&hostaddr[0], ipv4_addresses[i]) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%C failed get_host_addr(char*,int) check\n")
                      ACE_TEXT ("buf ['%C'] != expected value ['%C']\n"),
                      ipv4_addresses[i],
                      &hostaddr[0],
                      ipv4_addresses[i]));
          status = 1;
        }

      // Clear out the address by setting it to 1 and check
      addr.set (0, ACE_UINT32 (1), 1);
      status |= check_type_consistency (addr);
      if (addr.get_ip_address () != 1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Failed to set address to 1\n")));
          status = 1;
        }

      // Now set the address using a 32 bit number and check that we get
      // the right string out of get_host_addr().
      addr.set (80, addr32, 0); // addr32 is already in network byte order
      status |= check_type_consistency(addr);

      if (addr.get_host_addr () != 0 &&
          ACE_OS::strcmp (addr.get_host_addr (), ipv4_addresses[i]) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%C failed second get_host_addr() check\n")
                      ACE_TEXT ("return value ['%C'] != expected value ['%C']\n"),
                      ipv4_addresses[i],
                      addr.get_host_addr (),
                      ipv4_addresses[i]));
          status = 1;
        }

      // Test for ACE_INET_Addr::set_addr().
      struct sockaddr_in sa4;
      sa4.sin_family = AF_INET;
      sa4.sin_addr = addrv4;
      sa4.sin_port = ACE_HTONS (8080);

      addr.set (0, ACE_UINT32 (1), 1);
      addr.set_addr (&sa4, sizeof(sa4));
      status |= check_type_consistency (addr);

      if (addr.get_port_number () != 8080)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ACE_INET_Addr::set_addr() ")
                      ACE_TEXT ("failed to update port number.\n")));
          status = 1;
        }

      if (addr.get_ip_address () != ACE_HTONL (addr32))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ACE_INET_Addr::set_addr() ")
                      ACE_TEXT ("failed to update address.\n")));
          status = 1;
        }

    }

#if defined (ACE_HAS_IPV6)
  if (ACE::ipv6_enabled ())
    {
      char local_host_name[1024];
      ACE_OS::hostname(local_host_name, 1024);
      const char* local_names[] = {"localhost", local_host_name};

      for (int i = 0; i < 2; ++i)
      {
         ACE_INET_Addr addr;
         int old_type = addr.get_type();
         if (addr.set(12345, local_names[i]) == 0) {
           if (addr.get_type() != old_type) {
             ACE_ERROR ((LM_ERROR,
                         ACE_TEXT ("IPv6 set failed: addr.set(12345, \"%C\"), old addr.type() = %d, new addr_type()= %d\n"),
                         local_names[i],
                         old_type,
                         addr.get_type ()));
             status = 1;
           }
         }
         else {
           ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("IPv6 set failed: addr.set(12345, \"%C\") returns nonzero\n"), local_names[i]));
         }
      }

      const char *ipv6_addresses[] = {
        "1080::8:800:200c:417a", // unicast address
        "ff01::101",             // multicast address
        "::1",                   // loopback address
        "::",                    // unspecified addresses
        0
      };

      for (int i=0; ipv6_addresses[i] != 0; i++)
        {
          ACE_INET_Addr addr (80, ipv6_addresses[i]);
          status |= check_type_consistency (addr);

          if (0 != ACE_OS::strcmp (addr.get_host_addr (), ipv6_addresses[i]))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("IPv6 get_host_addr failed: %C != %C\n"),
                          addr.get_host_addr (),
                          ipv6_addresses[i]));
              status = 1;
            }
        }

      const char *ipv6_names[] = {
        "naboo.dre.vanderbilt.edu",
        "v6.ipv6-test.com",
        0
      };
      for (int i=0; ipv6_names[i] != 0; i++)
        {
          ACE_INET_Addr addr (80, ipv6_names[i]);
          status |= check_type_consistency (addr);

          if (0 != ACE_OS::strcmp (addr.get_host_name (), ipv6_names[i]))
            {
              // Alias? Check lookup on the reverse.
              ACE_INET_Addr alias_check;
              if (alias_check.set (80, addr.get_host_name ()) == 0)
                {
                  if (addr != alias_check)
                    ACE_ERROR ((LM_WARNING,
                                ACE_TEXT ("IPv6 name mismatch: %s (%s) != %s\n"),
                                addr.get_host_name (),
                                addr.get_host_addr (),
                                ipv6_names[i]));
                }
              else
                {
                  ACE_ERROR ((LM_WARNING,
                              ACE_TEXT ("IPv6 reverse lookup mismatch: %s (%s) != %s\n"),
                              addr.get_host_name (),
                              addr.get_host_addr (),
                              ipv6_names[i]));
                }
            }
        }
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("IPv6 tests done\n")));
#else
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ACE_HAS_IPV6 not set; no IPv6 tests run\n")));
#endif

  struct Address loopback_addresses[] =
    { {"127.0.0.1", true}, {"127.1.2.3", true}
      , {"127.0.0.0", true}, {"127.255.255.255", true}
      , {"126.255.255.255", false}, {"128.0.0.0", false}, {0, true}
    };

  for (int i=0; loopback_addresses[i].name != 0; i++)
    {
      struct in_addr addrv4;
      ACE_UINT32 addr32 = 0;

      ACE_OS::inet_pton (AF_INET, loopback_addresses[i].name, &addrv4);

      ACE_OS::memcpy (&addr32, &addrv4, sizeof (addr32));

      addr.set (80, loopback_addresses[i].name);

      if (addr.is_loopback() != loopback_addresses[i].loopback)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ACE_INET_Addr::is_loopback() ")
                      ACE_TEXT ("failed to distinguish loopback address. %C\n")
                      , loopback_addresses[i].name));
          status = 1;
        }
    }

  if (addr.string_to_addr ("127.0.0.1:72000", AF_INET) != -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ACE_INET_Addr::string_to_addr() ")
                  ACE_TEXT ("failed to detect port number overflow\n")));
      status = 1;
    }

  if (!test_tao_use ())
    status = 1;

  if (!test_multiple ())
    status = 1;

  if (!test_port_assignment ())
    status = 1;

  ACE_INET_Addr a1 (80, "127.0.0.1");
  ACE_INET_Addr a2 = a1;
  if (a1 != a2)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Address equality check failed after assignment\n")));
      status = 1;
    }

  ACE_END_TEST;

  return status;
}
