// $Id$

#include "ace/OS.h"
#include "ace/Arg_Shifter.h"
#include "Trading_Service.h"

ACE_RCSID(Trading_Service, Trading_Service, "$Id$")

Trading_Shutdown::Trading_Shutdown (Trading_Service& trader)
  : trader_ (trader)
{
  if (this->shutdown_.register_handler (SIGINT, this) == -1)
    {
      ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));
    }

  if (this->shutdown_.register_handler (SIGTERM, this) == -1)
    {
      ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));
    }
}

int
Trading_Shutdown::handle_signal (int signum, siginfo_t* sinfo, ucontext_t* ucon)
{
  ACE_UNUSED_ARG (signum);
  ACE_UNUSED_ARG (sinfo);
  ACE_UNUSED_ARG (ucon);
  this->trader_.~Trading_Service ();
  exit (0);
  return 0;
}

Trading_Service::Trading_Service (void)
  : federate_ (0),
    ior_output_file_ (0),
    bootstrapper_ (0)
{
  char* trader_name =
    CORBA::string_alloc (MAXHOSTNAMELEN + 10);

  if (trader_name != 0)
    {
      // The trader name is the concatenation of the local host name
      // and the server's process id.
      char host_name[MAXHOSTNAMELEN];
      ACE_INET_Addr localhost ((u_short) 0);
      localhost.get_host_name (host_name, MAXHOSTNAMELEN);
      ACE_OS::sprintf (trader_name, "%s_%d", host_name, ACE_OS::getpid ());

      char* dot = 0;
      while ((dot = ACE_OS::strchr (trader_name, '.')) != 0)
        *dot = '_';

      ACE_DEBUG ((LM_DEBUG, "*** Trading Service %s initializing.\n", trader_name));

      this->name_ = trader_name;
    }
}

Trading_Service::~Trading_Service (void)
{
  this->shutdown ();
}

int
Trading_Service::init (int argc, char* argv[])
{
  TAO_TRY
    {
      this->orb_manager_.init (argc, argv, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->parse_args (argc, argv) == -1)
        return -1;

      CORBA::ORB_ptr orb = this->orb_manager_.orb ();

      // Create a Trader Object and set its Service Type Repository.
      auto_ptr<TAO_Trader_Factory::TAO_TRADER> auto_trader (TAO_Trader_Factory::create_trader (argc, argv));
      this->trader_ = auto_trader;
      TAO_Support_Attributes_i& sup_attr = this->trader_->support_attributes ();
      TAO_Trading_Components_i& trd_comp = this->trader_->trading_components ();
      sup_attr.type_repos (this->type_repos_._this (TAO_TRY_ENV));
      TAO_CHECK_ENV;

      // The Spec says: return a reference to the Lookup interface
      // from the resolve_initial_references method.
      CosTrading::Lookup_ptr lookup = trd_comp.lookup_if ();
      this->ior_ = orb->object_to_string (lookup, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Dump the ior to a file.
      if (this->ior_output_file_ != 0)
        {
          ACE_OS::fprintf (this->ior_output_file_, "%s", this->ior_.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }

      if (this->federate_)
        {
          // Only become a multicast server if we're the only trader
          // on the multicast network.
          // @@ Could do other things. For example, every timeout
          // period try to federate again, but let's not hardcode that
          // policy.
          if (this->bootstrap_to_federation () == -1)
            this->init_multicast_server ();
        }
      else
        this->init_multicast_server ();
    }
    TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Trading Service");
    }
  TAO_ENDTRY;

  return 0;
}


int
Trading_Service::run (void)
{
  int return_value;
  CORBA::Environment TAO_IN_ENV;
  Trading_Shutdown trading_shutdown (*this);

  // Run the Trading Service.
  return_value = this->orb_manager_.run (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN (TAO_IN_ENV, -1);

  return return_value;
}

int
Trading_Service::init_multicast_server (void)
{
#if defined ACE_HAS_IP_MULTICAST
  // Get reactor instance from TAO.
  ACE_Reactor *reactor = TAO_ORB_Core_instance ()->reactor ();

  // First, see if the user has given us a multicast port number
  // for the name service on the command-line;
  u_short port =
    TAO_ORB_Core_instance ()->orb_params ()->trading_service_port ();

  if (port == 0)
    {
      const char *port_number =
        ACE_OS::getenv ("TradingServicePort");

      if (port_number != 0)
        port = ACE_OS::atoi (port_number);
      else
        port = TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT;
    }

  // Instantiate a server that will receive requests for an ior
  if (this->ior_multicast_.init ((char *) this->ior_.in (),
                                 port,
                                 ACE_DEFAULT_MULTICAST_ADDR,
                                 TAO_SERVICEID_TRADINGSERVICE) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Failed to init IOR multicast.\n"), -1);
    }

  // Register event handler for the ior multicast.
  if (reactor->register_handler (&this->ior_multicast_,
                                 ACE_Event_Handler::READ_MASK) == -1)
    ACE_DEBUG ((LM_DEBUG,
                "cannot register Event handler\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "The multicast server setup is done.\n"));

  // Other trader instances will bootstrap to us.
  this->bootstrapper_ = 1;

#endif /* ACE_HAS_IP_MULTICAST */
  return 0;
}

int
Trading_Service::bootstrap_to_federation (void)
{
  // If all traders follow this strategy, it creates a complete graph
  // of all known traders on a multicast network.
  CORBA::ORB_var orb = this->orb_manager_.orb ();

  ACE_DEBUG ((LM_DEBUG, "*** Bootstrapping to another Trading Service.\n"));
  CORBA::Object_var trading_obj =
    orb->resolve_initial_references ("TradingService");

  if (CORBA::is_nil (trading_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "We're all alone. "
                       "Unable to link to other traders.\n"),
                      -1);

  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG, "*** Narrowing the lookup interface.\n"));
      CosTrading::Lookup_var lookup_if =
        CosTrading::Lookup::_narrow (trading_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "*** Obtaining the link interface.\n"));
      CosTrading::Link_var link_if =  lookup_if->link_if (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      TAO_Trading_Components_i& trd_comp =
        this->trader_->trading_components ();
      CosTrading::Lookup_ptr our_lookup = trd_comp.lookup_if ();
      CosTrading::Link_ptr our_link = trd_comp.link_if ();

      ACE_DEBUG ((LM_DEBUG, "*** Linking found trader to self.\n"));
      link_if->add_link (this->name_.in (),
                         our_lookup,
                         CosTrading::always,
                         CosTrading::always,
                         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "*** Linking self to found trader.\n"));
      our_link->add_link ("Bootstrap",
                          lookup_if.in (),
                          CosTrading::always,
                          CosTrading::always,
                          TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "*** Retrieving list of known linked traders.\n"));
      CosTrading::LinkNameSeq_var link_name_seq =
        link_if->list_links (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "*** Linking self to all linked traders.\n"));
      for (int i = link_name_seq->length () - 1; i >= 0; i--)
        {
          // Avoid linking to ourselves.
          if (ACE_OS::strcmp (ACE_static_cast (const char*, link_name_seq[i]),
                              this->name_.in ()) != 0)
            {
              ACE_DEBUG ((LM_DEBUG, "*** Getting info for link %s.\n",
                          ACE_static_cast (const char*, link_name_seq[i])));
              CosTrading::Link::LinkInfo_var link_info =
                link_if->describe_link (link_name_seq[i], TAO_TRY_ENV);
              TAO_CHECK_ENV;

              CosTrading::Lookup_ptr remote_lookup;
              remote_lookup = link_info->target.in ();

              ACE_DEBUG ((LM_DEBUG, "*** Retrieving its link interface.\n"));
              CosTrading::Link_var remote_link =
                remote_lookup->link_if (TAO_TRY_ENV);
              TAO_CHECK_ENV;

              ACE_DEBUG ((LM_DEBUG, "*** Creating a link to me from it.\n"));
              remote_link->add_link (this->name_.in (),
                                     our_lookup,
                                     CosTrading::always,
                                     CosTrading::always,
                                     TAO_TRY_ENV);
              TAO_CHECK_ENV;

              ACE_DEBUG ((LM_DEBUG, "*** Creating a link to it from me.\n"));
              our_link->add_link (link_name_seq[i],
                                  remote_lookup,
                                  CosTrading::always,
                                  CosTrading::always,
                                  TAO_TRY_ENV);
              TAO_CHECK_ENV;
            }
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Trading Service");
    }
  TAO_ENDTRY;

  return 0;
}

int
Trading_Service::shutdown (void)
{
  CORBA::Environment TAO_IN_ENV;

  if (this->trader_.get () != 0)
    {
      TAO_Trading_Components_i& trd_comp
        = this->trader_->trading_components ();
      CosTrading::Link_ptr our_link = trd_comp.link_if ();

      CosTrading::LinkNameSeq_var link_name_seq =
        our_link->list_links (TAO_IN_ENV);

      ACE_DEBUG ((LM_DEBUG, "*** Unlinking from federated traders.\n"));
      for (int i = link_name_seq->length () - 1; i >= 0; i--)
        {
          TAO_TRY
            {
              ACE_DEBUG ((LM_DEBUG, "*** Describing the next link.\n"));
              CosTrading::Link::LinkInfo_var link_info =
                our_link->describe_link (link_name_seq[i], TAO_TRY_ENV);
              TAO_CHECK_ENV;

              ACE_DEBUG ((LM_DEBUG, "*** Removing link to %s.\n",
                          ACE_static_cast (const char*, link_name_seq[i])));
              our_link->remove_link (link_name_seq[i], TAO_TRY_ENV);
              TAO_CHECK_ENV;

              CosTrading::Lookup_ptr remote_lookup;
              remote_lookup = link_info->target.in ();

              ACE_DEBUG ((LM_DEBUG, "*** Retrieving its link interface.\n"));
              CosTrading::Link_var remote_link =
                remote_lookup->link_if (TAO_TRY_ENV);
              TAO_CHECK_ENV;

              ACE_DEBUG ((LM_DEBUG, "*** Removing its link to us.\n"));

              if (this->bootstrapper_)
                remote_link->remove_link ("Bootstrap", TAO_TRY_ENV);
              else
                remote_link->remove_link (this->name_.in (), TAO_TRY_ENV);
              TAO_CHECK_ENV;
            }
          TAO_CATCHANY
            {
              //              TAO_TRY_ENV.print_exception ("Trading Service");
            }
          TAO_ENDTRY;
        }
    }

  return 0;
}

int
Trading_Service::parse_args (int& argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      char *current_arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (current_arg, "-TSfederate") == 0)
        {
          arg_shifter.consume_arg ();
          this->federate_ = 1;
        }
      if (ACE_OS::strcmp (current_arg, "-TSdumpior") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {

              char* file_name = arg_shifter.get_current ();
              this->ior_output_file_ = ACE_OS::fopen (file_name, "w");

              if (this->ior_output_file_ == 0)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "Unable to open %s for writing: %p\n",
                                   file_name), -1);

              arg_shifter.consume_arg ();
            }
          else
            this->ior_output_file_ = ACE_OS::fdopen (ACE_STDOUT, "w");
        }

      else
        arg_shifter.ignore_arg ();
    }

  return 0;
}

int
main (int argc, char** argv)
{
  Trading_Service trader;

  if (trader.init (argc, argv) != -1)
    trader.run ();
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Failed to initialize the trader.\n"),
                        -1);
    }

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<TAO_Trader_Factory::TAO_TRADER>;
template class ACE_Auto_Basic_Ptr<TAO_Trader_Factory::TAO_TRADER>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr<TAO_Trader_Factory::TAO_TRADER>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Trader_Factory::TAO_TRADER>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
