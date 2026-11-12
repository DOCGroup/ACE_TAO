#include "ace/INET_Addr.h"
#include "orbsvcs/RtecUDPAdminC.h"
#include "orbsvcs/RtecEventCommC.h"
#include "ace/OS_NS_strings.h"

#include <array>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ACE_TCHAR const* ior = ACE_TEXT ("file://as.ior");
      for (int i = 1; i < argc; ++i)
        {
          if (ACE_OS::strcasecmp (argv[i], ACE_TEXT ("-k")) == 0)
            {
              if (i + 1 < argc)
                ior = argv[++i];
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "usage:  %s "
                               "-k <ior> "
                               "\n",
                               argv [0]),
                              1);
        }

      CORBA::Object_var tmp =
        orb->string_to_object (ior);

      RtecUDPAdmin::AddrServer_var as =
        RtecUDPAdmin::AddrServer::_narrow (tmp.in ());

      if (CORBA::is_nil (as.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil AddrServer reference <%s>\n",
                             ior),
                            1);
        }

      RtecEventComm::EventHeader header {};
      header.type = 0;
      ACE_INET_Addr inet_addr {};
      try
        {
          RtecUDPAdmin::UDP_Address_var udp_addr {};

          as->get_address (header, udp_addr.out ());
          switch (udp_addr->_d ())
            {
            case RtecUDPAdmin::Rtec_inet:
              inet_addr.set (udp_addr->v4_addr ().port,
                             udp_addr->v4_addr ().ipaddr);
              break;
            case RtecUDPAdmin::Rtec_inet6:
#if defined (ACE_HAS_IPV6)
              inet_addr.set_type (PF_INET6);
              inet_addr.set_address (udp_addr->v6_addr ().ipaddr, 16, 0);
              inet_addr.set_port_number (udp_addr->v6_addr ().port);
#else
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "ERROR: address server returned "
                                 "an unusable IPv6 address\n"),
                                1);
#endif /* ACE_HAS_IPV6 */
              break;
            }
        }
      catch (::CORBA::BAD_OPERATION const&)
        {
          // server only supports IPv4
          // Grab the right mcast group for this event...
          RtecUDPAdmin::UDP_Addr udp_addr {};
          as->get_addr (header, udp_addr);
          inet_addr.set (udp_addr.port, udp_addr.ipaddr);
        }

      std::array<ACE_TCHAR, 256> buffer {};
      inet_addr.addr_to_string (buffer.data (), buffer.size ());
      ACE_DEBUG ((LM_DEBUG, "Should be IPv4: %s\n", buffer.data ()));
      header.type = 1;
      try
        {
          RtecUDPAdmin::UDP_Address_var udp_addr {};

          as->get_address (header, udp_addr.out ());
          switch (udp_addr->_d ())
            {
            case RtecUDPAdmin::Rtec_inet:
              inet_addr.set (udp_addr->v4_addr ().port,
                             udp_addr->v4_addr ().ipaddr);
              break;
            case RtecUDPAdmin::Rtec_inet6:
#if defined (ACE_HAS_IPV6)
              inet_addr.set_type (PF_INET6);
              inet_addr.set_address (udp_addr->v6_addr ().ipaddr, 16, 0);
              inet_addr.set_port_number (udp_addr->v6_addr ().port);
#else
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "ERROR: address server returned "
                                 "an unusable IPv6 address\n"),
                                1);
#endif /* ACE_HAS_IPV6 */
              break;
            }
        }
      catch (::CORBA::BAD_OPERATION const&)
        {
          // server only supports IPv4
          // Grab the right mcast group for this event...
          RtecUDPAdmin::UDP_Addr udp_addr {};
          as->get_addr (header, udp_addr);
          inet_addr.set (udp_addr.port, udp_addr.ipaddr);
        }

      inet_addr.addr_to_string (buffer.data (), buffer.size ());
      ACE_DEBUG ((LM_DEBUG, "Should be IPv6: %s\n", buffer.data ()));

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
