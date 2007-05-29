// $Id$

#include "ace/INET_Addr.h"
#include "orbsvcs/RtecUDPAdminC.h"
#include "orbsvcs/RtecEventCommC.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://as.ior";

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "");

      for (int i = 1; i < argc; i++)
        {
          if (strcasecmp(argv[i], "-k") == 0)
            {
              if (argv[i+1] != 0)
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
        orb->string_to_object(ior);

      RtecUDPAdmin::AddrServer_var as =
        RtecUDPAdmin::AddrServer::_narrow (tmp.in());

      if (CORBA::is_nil (as.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil AddrServer reference <%s>\n",
                             ior),
                            1);
        }

      RtecEventComm::EventHeader header;
      header.type = 0;
      ACE_INET_Addr addr;


      ACE_INET_Addr inet_addr;
      try
        {
          RtecUDPAdmin::UDP_Address_var udp_addr;

          as->get_address (header, udp_addr.out());
          switch (udp_addr->_d())
            {
            case RtecUDPAdmin::INET:
              inet_addr.set(udp_addr->v4_addr().port,
                            udp_addr->v4_addr().ipaddr);
              break;
            case RtecUDPAdmin::INET6:
              inet_addr.set_type(PF_INET6);
              inet_addr.set_address(udp_addr->v6_addr().ipaddr,16,0);
              inet_addr.set_port_number(udp_addr->v6_addr().port);
              break;
            }
        }
      catch (const ::CORBA::BAD_OPERATION &)
        {
          // server only supports IPv4
           // Grab the right mcast group for this event...
          RtecUDPAdmin::UDP_Addr udp_addr;
          as->get_addr (header, udp_addr);
          inet_addr.set (udp_addr.port, udp_addr.ipaddr);
        }

      char buffer[256];
      inet_addr.addr_to_string (buffer,256);
      ACE_DEBUG ((LM_DEBUG,"Should be IPv4: %s\n",buffer));
      ACE_OS::memset (buffer,0,255);
      header.type = 1;
      try
        {
          RtecUDPAdmin::UDP_Address_var udp_addr;

          as->get_address (header, udp_addr.out());
          switch (udp_addr->_d())
            {
            case RtecUDPAdmin::INET:
              inet_addr.set(udp_addr->v4_addr().port,
                            udp_addr->v4_addr().ipaddr);
              break;
            case RtecUDPAdmin::INET6:
              inet_addr.set_type(PF_INET6);
              inet_addr.set_address(udp_addr->v6_addr().ipaddr,16,0);
              inet_addr.set_port_number(udp_addr->v6_addr().port);
              break;
            }
        }
      catch (const ::CORBA::BAD_OPERATION &)
        {
          // server only supports IPv4
           // Grab the right mcast group for this event...
          RtecUDPAdmin::UDP_Addr udp_addr;
          as->get_addr (header, udp_addr);
          inet_addr.set (udp_addr.port, udp_addr.ipaddr);
        }

      ACE_OS::memset(buffer,0,256);
      inet_addr.addr_to_string (buffer,256);
      ACE_DEBUG ((LM_DEBUG,"Should be IPv6: %s\n",buffer));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
