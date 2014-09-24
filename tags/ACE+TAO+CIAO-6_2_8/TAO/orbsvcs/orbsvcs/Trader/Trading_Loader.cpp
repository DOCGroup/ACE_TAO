// $Id$

//===========================================================================

// LIBRARY
//   libTAO_CosTrading.so
//
// DESCRIPTION
//   This class allows for dynamically loading
//   the Trading Service.
//
// AUTHORS
//   Priyanka Gontla <pgontla@ece.uci.edu>

//===========================================================================

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Trader/Trading_Loader.h"

#include "orbsvcs/Daemon_Utilities.h"

#include "tao/ORB_Core.h"
#include "tao/default_ports.h"
#include "tao/IORTable/IORTable.h"

#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_netdb.h"



TAO_Trading_Loader::TAO_Trading_Loader (void)
  : federate_ (0),
    ior_output_file_ (0),
    bootstrapper_ (0)
{
  char *trader_name = CORBA::string_alloc (MAXHOSTNAMELEN + 10);

  if (trader_name != 0)
    {
      // The trader name is the concatenation of the local host name
      // and the server's process id.
      char host_name[MAXHOSTNAMELEN + 1];
      ACE_INET_Addr localhost ((u_short) 0);
      if (localhost.get_host_name (host_name, sizeof (host_name)) != 0)
        {
          const char *tmp = localhost.get_host_addr ();
          if (tmp == 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nTAO Trading Service (%P|%t) ")
                        ACE_TEXT ("TAO_Trading_Loader ")
                        ACE_TEXT ("- %p\n\n"),
                        ACE_TEXT ("cannot determine hostname")));
          else
            ACE_OS::strcpy (host_name, tmp);
        }

      ACE_OS::sprintf (trader_name,
                       "%s_%ld",
                       host_name,
                       static_cast<long> (ACE_OS::getpid ()));

      for (char *dot = 0;
           (dot = ACE_OS::strchr (trader_name, '.')) != 0;
           *dot = '_')
        continue;

      ORBSVCS_DEBUG ((LM_DEBUG,
                  "*** Trading Service %C initializing.\n",
                  trader_name));

      this->name_ = trader_name;
    }
}

TAO_Trading_Loader::~TAO_Trading_Loader (void)
{
  // Destructor
}

int
TAO_Trading_Loader::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Check if -ORBDaemon is specified and if so, daemonize at this moment,
      // -ORBDaemon in the ORB core is faulty, see bugzilla 3335
      TAO_Daemon_Utility::check_for_daemon (argc, argv);

      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line(argc, argv);

      // Initialize the ORB Manager
      this->orb_manager_.init (command_line.get_argc(),
                               command_line.get_TCHAR_argv());

      CORBA::ORB_var orb =
        this->orb_manager_.orb ();

      // Initializes and sets up the Trading Service
      CORBA::Object_var object =
        this->create_object (orb.in (), command_line.get_argc(), command_line.get_TCHAR_argv());

    }
  catch (const CORBA::Exception&)
    {
      //    @@ Should we log this???
      return -1;
    }
  return 0;
}

int
TAO_Trading_Loader::fini (void)
{
  try
    {
      if (this->trader_.get () != 0)
        {
          TAO_Trading_Components_i& trd_comp
            = this->trader_->trading_components ();
          CosTrading::Link_ptr our_link =
            trd_comp.link_if ();

          CosTrading::LinkNameSeq_var link_name_seq =
            our_link->list_links ();

          ORBSVCS_DEBUG ((LM_DEBUG,
                      "*** Unlinking from federated traders.\n"));

          for (CORBA::ULong j = 0;
               j != link_name_seq->length ();
               ++j)
            {
              CORBA::ULong i =
                link_name_seq->length () - j - 1;

              ORBSVCS_DEBUG ((LM_DEBUG,
                          "*** Describing the next link.\n"));
              CosTrading::Link::LinkInfo_var link_info =
                our_link->describe_link (link_name_seq[i]);

              ORBSVCS_DEBUG ((LM_DEBUG,
                          "*** Removing link to %C.\n",
                          static_cast<const char *> (link_name_seq[i])));
              our_link->remove_link (link_name_seq[i]);

              CosTrading::Lookup_ptr remote_lookup;
              remote_lookup =
                link_info->target.in ();

              ORBSVCS_DEBUG ((LM_DEBUG,
                          "*** Retrieving its link interface.\n"));
              CosTrading::Link_var remote_link =
                remote_lookup->link_if ();

              ORBSVCS_DEBUG ((LM_DEBUG,
                          "*** Removing its link to us.\n"));

              if (this->bootstrapper_)
                {
                  remote_link->remove_link ("Bootstrap");
                }
              else
                {
                  remote_link->remove_link (this->name_.in ());
                }
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Trading Service shutting down");
    }

  return 0;
}

int
TAO_Trading_Loader::run (void)
{
  int return_value =
    this->orb_manager_.run ();

  return return_value;
}

CORBA::Object_ptr
TAO_Trading_Loader::create_object (CORBA::ORB_ptr orb_ptr,
                                   int argc,
                                   ACE_TCHAR *argv[])
{
  // Duplicate the ORB
  CORBA::ORB_var orb = CORBA::ORB::_duplicate (orb_ptr);

  // Activating the poa manager
  this->orb_manager_.activate_poa_manager ();

  // Create a Trader Object and set its Service Type Repository.
  auto_ptr<TAO_Trader_Factory::TAO_TRADER> auto_trader (TAO_Trader_Factory::create_trader (argc, argv));

  this->trader_ = auto_trader;

  TAO_Support_Attributes_i &sup_attr =
    this->trader_->support_attributes ();

  TAO_Trading_Components_i &trd_comp =
    this->trader_->trading_components ();

  sup_attr.type_repos (this->type_repos_._this ());

  // The Spec says: return a reference to the Lookup interface from
  // the resolve_initial_references method.
  CosTrading::Lookup_ptr lookup =
    trd_comp.lookup_if ();

  this->ior_ =
    orb->object_to_string (lookup);

  // Parse the args
  if (this->parse_args (argc, argv) == -1)
    return CORBA::Object::_nil ();

  // Dump the ior to a file.
  if (this->ior_output_file_ != 0)
    {
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       this->ior_.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  CORBA::Object_var table_object =
    orb->resolve_initial_references ("IORTable");

  IORTable::Table_var adapter =
    IORTable::Table::_narrow (table_object.in ());

  if (CORBA::is_nil (adapter.in ()))
    {
      ORBSVCS_ERROR ((LM_ERROR, "Nil IORTable\n"));
    }
  else
    {
      adapter->bind ("TradingService",
                     this->ior_.in ());
    }

  if (this->federate_)
    {
      // Only become a multicast server if we're the only trader
      // on the multicast network.
      // @@ Could do other things. For example, every timeout
      // period try to federate again, but let's not hardcode that
      // policy.
      int rc = this->bootstrap_to_federation ();

      if (rc == -1)
        this->init_multicast_server ();
    }
  else
    this->init_multicast_server ();

  return CORBA::Object::_nil ();
}

int
TAO_Trading_Loader::bootstrap_to_federation (void)
{
  // If all traders follow this strategy, it creates a complete graph
  // of all known traders on a multicast network.
  CORBA::ORB_var orb =
    this->orb_manager_.orb ();

  ORBSVCS_DEBUG ((LM_DEBUG,
              "*** Bootstrapping to another Trading Service.\n"));
  CORBA::Object_var trading_obj =
    orb->resolve_initial_references ("TradingService");

  if (CORBA::is_nil (trading_obj.in ()))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       "We're all alone. "
                       "Unable to link to other traders.\n"),
                      -1);

  ORBSVCS_DEBUG ((LM_DEBUG,
              "*** Narrowing the lookup interface.\n"));
  CosTrading::Lookup_var lookup_if =
    CosTrading::Lookup::_narrow (trading_obj.in ());

  ORBSVCS_DEBUG ((LM_DEBUG,
              "*** Obtaining the link interface.\n"));
  CosTrading::Link_var link_if =
    lookup_if->link_if ();

  TAO_Trading_Components_i &trd_comp =
    this->trader_->trading_components ();
  CosTrading::Lookup_ptr our_lookup =
    trd_comp.lookup_if ();
  CosTrading::Link_ptr our_link =
    trd_comp.link_if ();

  ORBSVCS_DEBUG ((LM_DEBUG,
              "*** Linking found trader to self.\n"));
  link_if->add_link (this->name_.in (),
                     our_lookup,
                     CosTrading::always,
                     CosTrading::always);

  ORBSVCS_DEBUG ((LM_DEBUG,
              "*** Linking self to found trader.\n"));
  our_link->add_link ("Bootstrap",
                      lookup_if.in (),
                      CosTrading::always,
                      CosTrading::always);

  ORBSVCS_DEBUG ((LM_DEBUG,
              "*** Retrieving list of known linked traders.\n"));
  CosTrading::LinkNameSeq_var link_name_seq =
    link_if->list_links ();

  ORBSVCS_DEBUG ((LM_DEBUG,
              "*** Linking self to all linked traders.\n"));
  for (CORBA::ULong i = link_name_seq->length () - 1;
       i > 0;
       i--)
    {
      // Avoid linking to ourselves.
      if (ACE_OS::strcmp (static_cast<const char *> (link_name_seq[i]),
                          this->name_.in ()) != 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                      "*** Getting info for link %s.\n",
                      static_cast<const char *> (link_name_seq[i])));
          CosTrading::Link::LinkInfo_var link_info =
            link_if->describe_link (link_name_seq[i]);

          CosTrading::Lookup_ptr remote_lookup;
          remote_lookup = link_info->target.in ();

          ORBSVCS_DEBUG ((LM_DEBUG,
                      "*** Retrieving its link interface.\n"));
          CosTrading::Link_var remote_link =
            remote_lookup->link_if ();

          ORBSVCS_DEBUG ((LM_DEBUG,
                      "*** Creating a link to me from it.\n"));
          remote_link->add_link (this->name_.in (),
                                 our_lookup,
                                 CosTrading::always,
                                 CosTrading::always);

          ORBSVCS_DEBUG ((LM_DEBUG,
                      "*** Creating a link to it from me.\n"));
          our_link->add_link (link_name_seq[i],
                              remote_lookup,
                              CosTrading::always,
                              CosTrading::always);
        }
    }

  return 0;
}

int
TAO_Trading_Loader::init_multicast_server (void)
{
#if defined (ACE_HAS_IP_MULTICAST)
  // Get reactor instance from TAO.
  ACE_Reactor *reactor = TAO_ORB_Core_instance ()->reactor ();

  // See if the -ORBMulticastDiscoveryEndpoint option was specified.
  ACE_CString mde (TAO_ORB_Core_instance ()->orb_params ()
                   ->mcast_discovery_endpoint ());

  // First, see if the user has given us a multicast port number for
  // the name service on the command-line;
  u_short port =
    TAO_ORB_Core_instance ()->orb_params ()->service_port (TAO::MCAST_TRADINGSERVICE);

  if (port == 0)
    {
      const char *port_number =
        ACE_OS::getenv ("TradingServicePort");

      if (port_number != 0)
        port = static_cast<u_short> (ACE_OS::atoi (port_number));
      else
        port = TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT;
    }

  // Instantiate a server that will receive requests for an ior
  if (mde.length () != 0)
    {
      if (this->ior_multicast_.init ((char *) this->ior_.in (),
                                     mde.c_str (),
                                     TAO_SERVICEID_TRADINGSERVICE) == -1)
        return -1;
    }
  else
    {
      if (this->ior_multicast_.init ((char *) this->ior_.in (),
                                     port,
                                     ACE_DEFAULT_MULTICAST_ADDR,
                                     TAO_SERVICEID_TRADINGSERVICE) == -1)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "Failed to init IOR multicast.\n"),
                          -1);
    }

  // Register event handler for the ior multicast.
  if (reactor->register_handler (&this->ior_multicast_,
                                 ACE_Event_Handler::READ_MASK) == -1)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "cannot register Event handler\n"));
  else
    ORBSVCS_DEBUG ((LM_DEBUG,
                "The multicast server setup is done.\n"));

  // Other trader instances will bootstrap to us.
  this->bootstrapper_ = 1;

#endif /* ACE_HAS_IP_MULTICAST */
  return 0;
}


int
TAO_Trading_Loader::parse_args (int &argc, ACE_TCHAR *argv [])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *current_arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (current_arg,
                          ACE_TEXT("-TSfederate")) == 0)
        {
          arg_shifter.consume_arg ();
          this->federate_ = 1;
        }
      if (ACE_OS::strcmp (current_arg,
                          ACE_TEXT("-TSdumpior")) == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR *file_name =
                arg_shifter.get_current ();
              this->ior_output_file_ =
                ACE_OS::fopen (file_name, ACE_TEXT("w"));

              if (this->ior_output_file_ == 0)
                ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                   "Unable to open %s for writing: %p\n",
                                   file_name), -1);
              arg_shifter.consume_arg ();
            }
          else
            this->ior_output_file_ =
              ACE_OS::fdopen (ACE_STDOUT,
                              ACE_TEXT("w"));
        }

      else
        arg_shifter.ignore_arg ();
    }

  return 0;
}

ACE_FACTORY_DEFINE (TAO_Trading_Serv, TAO_Trading_Loader)

