// $Id$

#include "tao/ORB_Core.h"
#include "tao/ORB.h"

#include "ace/Env_Value_T.h"
#include "ace/Arg_Shifter.h"

#include "tao/TAO_Internal.h"
#include "tao/default_client.h"
#include "tao/default_server.h"
#include "tao/default_resource.h"
#include "tao/debug.h"
#include "tao/IOR_LookupTable.h"

#if !defined (__ACE_INLINE__)
# include "tao/ORB_Core.i"
#endif /* ! __ACE_INLINE__ */

#include "tao/Connector_Registry.h"
#include "tao/Acceptor_Registry.h"

#include "tao/POA.h"

#include "ace/Object_Manager.h"
#include "ace/Env_Value_T.h"
#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/INET_Addr.h"

#if defined(ACE_MVS)
#include "ace/Codeset_IBM1047.h"
#endif /* ACE_MVS */

ACE_RCSID(tao, ORB_Core, "$Id$")

// ****************************************************************

CORBA::Environment &
TAO_default_environment ()
{
  return *TAO_TSS_RESOURCES::instance ()->default_environment_;
}

// ****************************************************************

TAO_ORB_Core::TAO_ORB_Core (const char* orbid)
  : connector_registry_ (0),
    acceptor_registry_ (0),
    protocol_factories_ (0),
    root_poa_ (0),
    orb_params_ (0),
    orbid_ (ACE_OS::strdup (orbid?orbid:"")),
    resource_factory_ (0),
    resource_factory_from_service_config_ (0),
    // @@ This is not needed since the default resource factory, fredk
    //    is staticaly added to the service configurator.
    client_factory_ (0),
    client_factory_from_service_config_ (0),
    // @@ This is not needed since the default client factory, fredk
    //    is staticaly added to the service configurator.
    server_factory_ (0),
    server_factory_from_service_config_ (0),
    // @@ This is not needed since the default server factory, fredk
    //    is staticaly added to the service configurator.
    opt_for_collocation_ (1),
    use_global_collocation_ (1),
    collocation_strategy_ (THRU_POA),
    poa_current_ (0),
    object_adapter_ (0),
    tm_ (),
    from_iso8859_ (0),
    to_iso8859_ (0),
    from_unicode_ (0),
    to_unicode_ (0)
{
  ACE_NEW (this->poa_current_,
           TAO_POA_Current);

  // Make sure that the thread manager does not wait for threads
  this->tm_.wait_on_exit (0);

#if defined(ACE_MVS)
  ACE_NEW (this->from_iso8859_, ACE_ISO8859_IBM1047);
  ACE_NEW (this->to_iso8859_,   ACE_IBM1047_ISO8859);
#endif /* ACE_MVS */
}

TAO_ORB_Core::~TAO_ORB_Core (void)
{
  // Allocated in init()
  delete this->orb_params_;

  ACE_OS::free (this->orbid_);

  delete this->poa_current_;

  delete this->object_adapter_;

  delete this->from_iso8859_;
  delete this->to_iso8859_;

  if (this->reactor_ != 0)
    {
      delete this->reactor_;
      this->reactor_ = 0;
    }
}

int
TAO_ORB_Core::add_to_ior_table (ACE_CString init_ref,
                                TAO_IOR_LookupTable &table)
{
  int slot = init_ref.find ("=");
  if (slot == ACE_CString::npos)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to parse -ORBInitRef parameter\n"),
                      -1);

  ACE_CString object_id = init_ref.substr (0, slot);
  ACE_CString ior = init_ref.substr (slot + 1);

  // Add the objectID-IOR to the table and return the status.
  return table.add_ior (object_id, ior);

}

int
TAO_ORB_Core::init (int &argc, char *argv[])
{
  // Right now, this code expects to begin parsing in argv[1] rather
  // than argv[0].  I don't think that's wise.  I think we need to
  // change that convention to argv[0] and let the initializing code
  // make any necessary shifts.
  //
  // Parse arguments to the ORB.  Typically the ORB is passed
  // arguments straight from the command line, so we will simply pass
  // through them and respond to the ones we understand and ignore
  // those we don't.
  //
  // In some instances, we may actually build another vector of
  // arguments and stash it for use initializing other components such
  // as the ACE_Service_Config or the RootPOA.
  //
  // Prepare a copy of the argument vector for the service configurator.

  char **svc_config_argv;
  // @@ depricated
  int old_style_endpoint = 0;

  int svc_config_argc = 0;
  ACE_NEW_RETURN (svc_config_argv, char *[argc + 1], 0);

  // Be certain to copy the program name so that service configurator
  // has something to skip!
  ACE_Arg_Shifter arg_shifter (argc, argv);
  const char* argv0 = "";
  if (argc > 0 && argv != 0)
    argv0 = argv[0];
  svc_config_argv[svc_config_argc++] = CORBA::string_dup (argv0);

  // Initialize the container for the ORB parameters.
  // orb_params_ must be initialized before the command line parsing loop
  // since some of the parsing code expects it to have been already
  // initialized.
  if (this->orb_params_ == 0)
    ACE_NEW_RETURN (this->orb_params_, TAO_ORB_Parameters, 0);

  // @@ This should be an IIOP default, more generally each
  //    loaded protocol should have it's own default defined by the
  //    implemention.  This is currently defined to be a zero, fredk
  ACE_Env_Value<int> defport ("TAO_DEFAULT_SERVER_PORT",
                              TAO_DEFAULT_SERVER_PORT);
  ACE_CString host;
  CORBA::UShort port = defport;

  // @@ GIOPLite should be an alternative ORB Messaging protocols, fredk
  int giop_lite = 0;

  CORBA::Boolean use_ior = 1;
  int cdr_tradeoff = ACE_DEFAULT_CDR_MEMCPY_TRADEOFF;

  // The following things should be changed to use the ACE_Env_Value<>
  // template sometime.

  // Name Service IOR string.
  ACE_CString ns_ior;

  // New <ObjectID>:<IOR> mapping that is used by the
  // resolve_initial_references ()
  ACE_CString init_ref;

  // Table for <ObjectID>:<IOR> mapping specified on commandline
  // using ORBInitRef.
  TAO_IOR_LookupTable *ior_lookup_table;

  ACE_NEW_RETURN (ior_lookup_table,
                  TAO_IOR_LookupTable,
                  -1);

  // List of comma separated prefixes from ORBDefaultInitRef.
  ACE_CString default_init_ref;

  // Name Service port use for Multicast
  u_short ns_port = 0;

  // Trading Service IOR string.
  ACE_CString ts_ior;

  // Trading Service port used for Multicast
  u_short ts_port = 0;

  // Buffer sizes for kernel socket buffers
  // @@ should be a default defined for each protocol implementation?
  //    since we may have protocols loaded which use shared memory of
  //    some form, fredk
  size_t rcv_sock_size = 0;
  size_t snd_sock_size = 0;

  // Should we skip the <ACE_Service_Config::open> method, e.g., if we
  // already being configured by the ACE Service Configurator.
  int skip_service_config_open = 0;

  // Use dotted decimal addresses
  // @@ This option will be treated as a suggestion to each loaded protocol to
  // @@ use a character representation for the numeric address, otherwise
  // @@ use a logical name. fredk
#if defined (TAO_USE_DOTTED_DECIMAL_ADDRESSES)
  int dotted_decimal_addresses = 1;
#else
  int dotted_decimal_addresses = 0;
#endif /* TAO_USE_DOTTED_DECIMAL_ADDRESSES */

#if defined (TAO_STD_PROFILE_COMPONENTS)
  int std_profile_components = 1;
#else
  int std_profile_components = 0;
#endif /* TAO_STD_PROFILE_COMPONENTS */

  int use_tss_resources = -1;
  // -1 is unknown, default to what the resource factory sets.
  // @@ This is just for backwards compatibility.

  while (arg_shifter.is_anything_left ())
    {
      char *current_arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (current_arg,
                              "-ORBSvcConf") == 0)
        {
          // Specify the name of the svc.conf file to be used.
          svc_config_argv[svc_config_argc++] =
            CORBA::string_dup ("-f");
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              svc_config_argv[svc_config_argc++] =
                CORBA::string_dup (arg_shifter.get_current ());
              arg_shifter.consume_arg();
            }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBDaemon") == 0)
        {
          // Be a daemon
          svc_config_argv[svc_config_argc++] =
            CORBA::string_dup ("-b");
          arg_shifter.consume_arg ();
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBDottedDecimalAddresses") == 0)
        {
          // Use dotted decimal addresses
          // @@ this should be renamed.  See above comment. fredk
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              dotted_decimal_addresses = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBDebug") == 0)
        {
          // Turn on debugging
          ACE::debug (1);
          TAO_orbdebug = 1;
          arg_shifter.consume_arg ();
        }
      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBDebugLevel") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next())
            {
              TAO_debug_level =
                ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBEndpoint") == 0)
        {
          // Each "endpoint" is of the form:
          //
          //   protocol://V.v@addr1,addr2,...,addrN
          //
          // or:
          //
          //   protocol://addr1,addr2,...,addrN
          //
          // where "V.v" is an optional protocol version for each
          // addr.  All preconnect or endpoint strings should be of
          // the above form(s).
          //
          // Multiple sets of endpoint may be seperated by a semi-colon `;'.
          // For example:
          //
          //   iioploc://space:2001,1.2@odyssey:2010;uiop://foo,bar
          //
          // All preconnect or endpoint strings should be of the above form(s).

          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next())
            {
              ACE_CString endpts (arg_shifter.get_current ());

              if (this->orb_params ()->endpoints (endpts) != 0)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%P|%t)\n"
                                     "Invalid endpoint(s) specified:\n%s\n",
                                     endpts.c_str ()),
                                    -1);
                }

              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBHost") == 0)
        {
          // @@ Fred&Carlos: This option now has the same effect as specifying
          //                 an extra -ORBendpoint.  Ideally, this option
          //                 should be removed so that all INET specific
          //                 stuff can be removed from the ORB core but I
          //                 guess we need to leave it here for backward
          //                 compatibility.  C'est la vie.

          old_style_endpoint = 1;
          // Specify the name of the host (i.e., interface) on which
          // the server should listen.
          arg_shifter.consume_arg ();

          // Issue a warning since this backward compatibilty support
          // may be dropped in future releases.

          ACE_DEBUG ((LM_WARNING,
                      "(%P|%t) \nWARNING: The `-ORBhost' option is obsolete.\n"
                      "In the future, use the `-ORBendpoint' option.\n"));

          if (arg_shifter.is_parameter_next())
            {
              host = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBNameServiceIOR") == 0)
        {
          // Specify the IOR of the NameService.

          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              ns_ior = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBNameServicePort") == 0)
        {
          // Specify the port number for the NameService.
          // Unrelated to ORB Protocols, this is used for multicast.

          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              ns_port = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBTradingServiceIOR") == 0)
        {
          // Specify the IOR of the NameService.

          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              ts_ior = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBTradingServicePort") == 0)
        {
          // Specify the port number for the NameService.

          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              ts_port = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBPort") == 0)
        {
          // Issue a warning since this backward compatibilty support
          // may be dropped in future releases.

          old_style_endpoint = 1;
          ACE_DEBUG ((LM_WARNING,
                      "(%P|%t) \nWARNING: The `-ORBPort' option is obsolete.\n"
                      "In the future, use the `-ORBEndpoint' option.\n"));

          // Specify the port number/name on which we should listen
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              // We really shouldn't limit this to being specified as
              // an int, but oh well for now.
              port = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBRcvSock") == 0)
        {
          // @@ All protocol implementation may not use sockets, so
          //    this can either be a generic I/O Buffer size or
          //    Buffer info can be a per protocol specification, fredk

          arg_shifter.consume_arg ();
          // Specify the size of the socket's receive buffer

          if (arg_shifter.is_parameter_next ())
            {
              rcv_sock_size = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBSndSock") == 0)
        {
          // @@ All protocol implementation may not use sockets, so
          //    this can either be a generic I/O Buffer size or
          //    Buffer info can be a per protocol specification, fredk

          arg_shifter.consume_arg ();
          // Specify the size of the socket's send buffer
          if (arg_shifter.is_parameter_next ())
            {
              snd_sock_size = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBObjRefStyle") == 0)
        {
          // Specifies the style of printed objrefs: URL or IOR
          //
          // BEGIN COMMENTS FROM IIOP-1.4 On Win32, we should be
          // collecting information from the Registry such as what
          // ORBs are configured, specific configuration details like
          // whether they generate IOR or URL style stringified
          // objrefs and which addresses they listen to (e.g. allowing
          // multihomed hosts to implement firewalls), user-meaningful
          // orb names (they will normally indicate domains), and
          // more.
          //
          // On UNIX, we should collect that from some private config
          // file.
          //
          // Instead, this just treats the "internet" ORB name
          // specially and makes it always use URL-style stringified
          // objrefs, where the hostname and TCP port number are
          // explicit (and the whole objref is readable by mortals).
          // BEGIN COMMENTS FROM IIOP-1.4
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt,
                                      "URL") == 0)
                use_ior = 0;

              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBCollocation") == 0)
        // Specify whether we want to optimize against collocation
        // objects.  Valid arguments are: "yes" and "no".  Default is
        // yes.
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char *opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt,
                                      "YES") == 0)
                this->opt_for_collocation_ = 1;
              else if (ACE_OS::strcasecmp (opt,
                                           "NO") == 0)
                this->opt_for_collocation_ = 0;

              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBCollocationStrategy") == 0)
        // Specify which collocation policy we want to use.
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char *opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "thru_poa") == 0)
                this->collocation_strategy_ = THRU_POA;
              else if (ACE_OS::strcasecmp (opt, "direct") == 0)
                this->collocation_strategy_ = DIRECT;

              arg_shifter.consume_arg ();
            }
        }

      // @@ Ossama: could you add this option to the Options.html
      //    file?  And could you also remove from the .html file the
      //    stuff we took out of the default server strategy factory
      //    and the default resource factory?
      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBGlobalCollocation") == 0)
        // Specify whether we want to use collocation across ORBs;
        // i.e. all the ORBs in the same address space use collocated
        // calls.
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char *opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt,
                                      "YES") == 0)
                this->use_global_collocation_ = 1;
              else if (ACE_OS::strcasecmp (opt,
                                           "NO") == 0)
                this->use_global_collocation_ = 0;

              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBPreconnect") == 0)
        {
          arg_shifter.consume_arg ();

          // Get a string which describes the connections we want to
          // cache up-front, thus reducing the latency of the first call.
          //
          // For example,  specify -ORBpreconnect once for each
          // protocol:
          //
          //   -ORBpreconnect iiop://tango:10015,watusi:10016
          //   -ORBpreconnect busX_iop://board1:0x07450000,board2,0x08450000
          //
          // Or chain all possible endpoint designations together:
          //
          //   -ORBpreconnect iiop://tango:10015,watusi:10016/;
          //              busX_iop://board1:0x07450000,board2,0x08450000/
          //
          // The old style command line only works for IIOP:
          //    -ORBpreconnect tango:10015,tango:10015,watusi:10016

          if (arg_shifter.is_parameter_next ())
            {
              ACE_CString preconnections (arg_shifter.get_current ());

              if (this->orb_params ()->preconnects (preconnections) != 0)
                {
                  // Handle old style preconnects for backward compatibility.
                  // The old style preconnects only work for IIOP!

                  // Issue a warning since this backward compatibilty support
                  // may be dropped in future releases.

                  ACE_DEBUG ((LM_WARNING,
                              "(%P|%t) \nWARNING: The `host:port' pair style "
                              "for `-ORBpreconnect' is obsolete.\n"
                              "In the future, use the URL style.\n"));

                  preconnections =
                    ACE_CString ("iiop://") +
                    ACE_CString (preconnections) +
                    ACE_CString ("/");

                  ACE_DEBUG ((LM_WARNING,
                              "(%P|%t) \nWARNING: The following preconnection "
                              "will be used:\n%s\n",
                              preconnections.c_str()));

                  this->orb_params ()->preconnects (preconnections);
                }
            }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBCDRTradeoff") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              cdr_tradeoff = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

       else if (ACE_OS::strcasecmp (current_arg,
                                    "-ORBSvcConfDirective") == 0)
         {
           // This is used to pass arguments to the Service
           // Configurator using the "command line" to provide
           // configuration information rather than using a svc.conf
           // file.  Pass the "-S" to the service configurator.
           svc_config_argv[svc_config_argc++] =
             CORBA::string_dup ("-S");
           arg_shifter.consume_arg ();

           if (arg_shifter.is_parameter_next ())
             {
               // Pass the next argument.
               svc_config_argv[svc_config_argc++] =
                 CORBA::string_dup (arg_shifter.get_current ());
               arg_shifter.consume_arg ();
             }
         }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBGIOPlite") == 0)
        {
          // @@ This will have to change since gioplite will be considered
          //    as an alternate ORB messaging protocols.
          arg_shifter.consume_arg ();
          giop_lite = 1;
        }

      // A new <ObjectID>:<IOR> mapping has been specified. This will be
      // used by the resolve_initial_references ().

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBInitRef") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              init_ref = arg_shifter.get_current ();
              if (this->add_to_ior_table (init_ref,
                                          *ior_lookup_table) != 0)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "Unable to add IOR to the Table\n"),
                                  -1);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBDefaultInitRef") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              default_init_ref = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBSkipServiceConfigOpen") == 0)
        {
          arg_shifter.consume_arg ();
          skip_service_config_open = 1;
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBStdProfileComponents") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              std_profile_components =
                ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
           }
        }

      else if (ACE_OS::strcasecmp (current_arg,
                                   "-ORBResources") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char *opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "global") == 0)
                use_tss_resources = 0;
              else if (ACE_OS::strcasecmp (opt, "tss") == 0)
                use_tss_resources = 1;

              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strncasecmp (current_arg,
                                    "-ORB",
                                    4) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "warning: unknown -ORB arg %s, consuming arg\n",
                       current_arg));
          arg_shifter.consume_arg ();
        }
      else
        // Any arguments that don't match are ignored so that the
        // caller can still use them.
        arg_shifter.ignore_arg ();
    }

#if defined (DEBUG)
  // Make it a little easier to debug programs using this code.
  {
    TAO_debug_level = ACE_Env_Value<u_int> ("TAO_ORB_DEBUG", 0);

    char *value = ACE_OS::getenv ("TAO_ORB_DEBUG");

    if (value != 0)
      {
        TAO_debug_level = ACE_OS::atoi (value);
        if (TAO_debug_level <= 0)
          TAO_debug_level = 1;
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_debug_level == %d", TAO_debug_level));
      }
  }
#endif  /* DEBUG */

#if defined (SIGPIPE) && !defined (ACE_LACKS_UNIX_SIGNALS)
  // There's really no way to deal with this in a portable manner, so
  // we just have to suck it up and get preprocessor conditional and
  // ugly.
  //
  // Impractical to have each call to the ORB protect against the
  // implementation artifact of potential writes to dead connections,
  // as it'd be way expensive.  Do it here; who cares about SIGPIPE in
  // these kinds of applications, anyway?
  (void) ACE_OS::signal (SIGPIPE, SIG_IGN);
#endif /* SIGPIPE */

  // Initialize the Service Configurator -check for return values.
  // Load the resource factory, connector registry, acceptor registry
  // and protocols.  Will need to call the open () method on
  // the registries!
  int result = TAO_Internal::open_services (svc_config_argc,
                                            svc_config_argv,
                                            0,
                                            skip_service_config_open);

  // Make sure to free up all the dynamically allocated memory.  If we
  // decide we don't need to allocate this stuff dynamically then we
  // can remove this.
  for (int i = 0; i < svc_config_argc; i++)
    CORBA::string_free (svc_config_argv[i]);

  delete [] svc_config_argv;

  // Check for errors returned from <TAO_Internal::open_services>.
  if (result != 0 && errno != ENOENT)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "ORB Core unable to initialize the Service Configurator"),
                      -1);

  // Initialize the pointers to resources in the ORB Core instance,
  // e.g., reactor, connector, etc.  Must do this after we open
  // services because we'll load the factory from there.
  TAO_Resource_Factory *trf = this->resource_factory ();

  if (trf == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "ORB Core unable to find a Resource Factory instance"),
                      -1);

  (void) this->reactor ();
  // Make sure the reactor is initialized...

  TAO_Server_Strategy_Factory *ssf = this->server_factory ();

  if (ssf == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "ORB Core unable to find a Server Strategy Factory instance"),
                      -1);

  ssf->open (this);

  // Inititalize the "ORB" pseudo-object now.
  ACE_NEW_RETURN (this->orb_, CORBA_ORB (this), 0);

  // This should probably move into the ORB Core someday rather then
  // being done at this level.
  this->orb_->_use_omg_ior_format (use_ior);

  // Set the <shutdown_lock_> for the ORB.
  this->orb_->shutdown_lock_ = ssf->create_event_loop_lock ();

  // @@ Michael: I don't know if this is the best spot,
  // we might have to discuss that.
  //this->leader_follower_lock_ptr_ =  this->client_factory ()
  //                                       ->create_leader_follower_lock ();

  // Set all kinds of orb parameters whose setting needed to be
  // deferred until after the service config entries had been
  // determined.

  // @@ Set the endpoint string to iiop://host:port/
  //    Add a string to hold the endpoint desgination for this ORB
  //    for now it will be IIOP://host:port/  fredk
  if (old_style_endpoint)
    {
      ACE_CString iiop_endpoint;
      if (this->set_iiop_endpoint (dotted_decimal_addresses,
                                   port,
                                   host,
                                   iiop_endpoint) == -1)
        return -1;
      // Add the endpoint
      this->orb_params ()->endpoints (iiop_endpoint);
    }

  // Set the init_ref.
  this->orb_params ()->init_ref (init_ref);

  // Set the IOR Table.
  this->orb_params ()->ior_lookup_table (ior_lookup_table);

  // Set the list of prefixes from -ORBDefaultInitRef.
  this->orb_params ()->default_init_ref (default_init_ref);

  this->orb_params ()->name_service_ior (ns_ior);
  this->orb_params ()->name_service_port (ns_port);
  this->orb_params ()->trading_service_ior (ts_ior);
  this->orb_params ()->trading_service_port (ts_port);
  this->orb_params ()->use_dotted_decimal_addresses (dotted_decimal_addresses);
  if (rcv_sock_size != 0)
    this->orb_params ()->sock_rcvbuf_size (rcv_sock_size);
  if (snd_sock_size != 0)
    this->orb_params ()->sock_sndbuf_size (snd_sock_size);
  if (cdr_tradeoff >= 0)
    this->orb_params ()->cdr_memcpy_tradeoff (cdr_tradeoff);

  this->orb_params ()->use_lite_protocol (giop_lite);

  this->orb_params ()->std_profile_components (std_profile_components);

  // ** Set up the pluggable protocol infrastructure.  First get a
  // pointer to the protocol factories set, then obtain pointers to
  // all factories loaded by the service configurator.
  // Load all protocol factories!
  if (trf->init_protocol_factories () == -1)
    return -1;

  // init the ORB core's pointer
  this->protocol_factories_ = trf->get_protocol_factories ();

  // Now that we have a complete list of available protocols and their
  // related factory objects, initial;ize the registries!

  // Init the connector registry and create a connector for each
  // configured protocol.
  if (this->connector_registry ()->open (this) != 0)
    return -1;

  // Have registry parse the preconnects
  if (this->orb_params ()->preconnects ().is_empty () == 0)
    this->connector_registry ()->preconnect (this->orb_params ()->preconnects ());

  if (use_tss_resources == -1)
    this->use_tss_resources_ = 0; // @@ What is the default?
  else
    this->use_tss_resources_ = use_tss_resources;

  return 0;
}


int
TAO_ORB_Core::set_iiop_endpoint (int dotted_decimal_addresses,
                                 CORBA::UShort port,
                                 ACE_CString &host,
                                 ACE_CString &endpoint)
{
  // No host specified; find it
  if (host.length () == 0)
    {
      ASYS_TCHAR name[MAXHOSTNAMELEN + 1];
      if (ACE_OS::hostname (name, MAXHOSTNAMELEN + 1) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Failed to look up local host name.\n"),
                       -1);
        }
      host.set (name, 1);
    }

  // @@ For compatibility (ug) with how things were done before,
  //    get the local host name in the correct format.  This will be
  //    stored away in the ORB!  fredk
  ACE_INET_Addr rendezvous;
  rendezvous.set (port, host.c_str ());

  char buffer[MAXHOSTNAMELEN + 1];

  if (rendezvous.addr_to_string (buffer,
                                 MAXHOSTNAMELEN,
                                 dotted_decimal_addresses) == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) failed in addr_to_string () %p.\n"),
                          -1);
    }

  // endpoint == iiop://host:port/
  endpoint.set ("iiop://", 1);
  endpoint += buffer;
  endpoint += ACE_CString("/");

  return 0;
}

int
TAO_ORB_Core::fini (void)
{
  if (TAO_debug_level >= 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Destroying ORB <%s>\n",
                  this->orbid_));
    }

  // Close connectors before acceptors!
  // Ask the registry to close all registered connectors.
  if (this->connector_registry_ != 0)
    {
      this->connector_registry_->close_all ();
      delete this->connector_registry_;
    }

  // Ask the registry to close all registered acceptors.
  if (this->acceptor_registry_ != 0)
    {
      this->acceptor_registry_->close_all ();
      delete this->acceptor_registry_;
    }

  TAO_Internal::close_services ();

  // @@ This is not needed since the default resource factory
  //    is staticaly added to the service configurator, fredk
  if (!this->resource_factory_from_service_config_)
    delete resource_factory_;

  // @@ This is not needed since the default client factory
  //    is staticaly added to the service configurator, fredk
  if (!this->client_factory_from_service_config_)
    delete client_factory_;

  // @@ This is not needed since the default server factory
  //    is staticaly added to the service configurator, fredk
  if (!this->server_factory_from_service_config_)
    delete server_factory_;

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                              *ACE_Static_Object_Lock::instance (), 0));
    TAO_ORB_Table::instance ()->unbind (this->orbid_);
  }

  delete this;

  return 0;
}

TAO_Resource_Factory *
TAO_ORB_Core::resource_factory (void)
{
  if (this->resource_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->resource_factory_ =
        ACE_Dynamic_Service<TAO_Resource_Factory>::instance ("Resource_Factory");
      // @@ Not needed!
      this->resource_factory_from_service_config_ = 1;
    }

  //@@ None of this stuff is needed since the default resource factory
  //   is statically adde to the Service Configurator!
  if (this->resource_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.
      if (TAO_orbdebug)
        ACE_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No Resource Factory found in Service Repository."
                    "  Using default instance with GLOBAL resource source specifier.\n"));

      TAO_Default_Resource_Factory* default_factory;
      ACE_NEW_RETURN (default_factory,
                      TAO_Default_Resource_Factory,
                      0);

      // @@ Not needed.
      this->resource_factory_from_service_config_ = 0;
      this->resource_factory_ = default_factory;

      // @@ At this point we need to register this with the
      // Service_Repository in order to get it cleaned up properly.
      // But, for now we let it leak.
    }
  return this->resource_factory_;
}

TAO_Client_Strategy_Factory *
TAO_ORB_Core::client_factory (void)
{
  if (this->client_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->client_factory_ =
        ACE_Dynamic_Service<TAO_Client_Strategy_Factory>::instance ("Client_Strategy_Factory");
      // @@ Not needed!
      this->client_factory_from_service_config_ = 1;
    }

  //@@ None of this stuff is needed since the default client factory
  //   is statically added to the Service Configurator, fredk
  if (this->client_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.
      if (TAO_orbdebug)
        ACE_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No Client Strategy Factory found in Service Repository."
                    "  Using default instance.\n"));

      ACE_NEW_RETURN (this->client_factory_,
                      TAO_Default_Client_Strategy_Factory,
                      0);

      this->client_factory_from_service_config_ = 0;
      // At this point we need to register this with the
      // Service_Repository in order to get it cleaned up properly.
      // But, for now we let it leak.
    }
  return this->client_factory_;
}

TAO_Server_Strategy_Factory *
TAO_ORB_Core::server_factory (void)
{
  if (this->server_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->server_factory_ =
        ACE_Dynamic_Service<TAO_Server_Strategy_Factory>::instance ("Server_Strategy_Factory");
      // @@ Not needed!
      this->server_factory_from_service_config_ = 1;
    }

  //@@ None of this stuff is needed since the default server factory
  //   is statically adde to the Service Configurator, fredk
  // If the <server_factory_> isn't found it's usually because the ORB
  // hasn't been intialized correctly...
  if (this->server_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.
      if (TAO_orbdebug)
        ACE_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "Server Strategy Factory"));

      ACE_NEW_RETURN (this->server_factory_,
                      TAO_Default_Server_Strategy_Factory,
                      0);

      // @@ Not needed!
      this->server_factory_from_service_config_ = 0;
      // At this point we need to register this with the
      // <Service_Repository> to get it cleaned up properly.  But, for
      // now we let it leak.
    }

  return this->server_factory_;
}

CORBA::ORB_ptr
TAO_ORB_Core::orb (CORBA::ORB_ptr op)
{
  // Shouldn't need to check for ptr validity at this point b/c we
  // already did in ::init()
  CORBA::ORB_ptr old_orb = this->orb_;
  this->orb_ = op;
  return old_orb;
}

PortableServer::POA_ptr
TAO_ORB_Core::root_poa_reference (CORBA::Environment &ACE_TRY_ENV,
                                  const char *adapter_name,
                                  TAO_POA_Manager *poa_manager,
                                  const TAO_POA_Policies *policies)
{
  // @@ Double check??
  if (CORBA::is_nil (this->root_poa_reference_.in ()))
    {
      TAO_POA *poa = this->root_poa (ACE_TRY_ENV,
                                     adapter_name,
                                     poa_manager,
                                     policies);
      ACE_CHECK_RETURN (PortableServer::POA::_nil ());

      this->root_poa_reference_ = poa->_this (ACE_TRY_ENV);
      ACE_CHECK_RETURN (PortableServer::POA::_nil ());
    }

  return PortableServer::POA::_duplicate (this->root_poa_reference_.in ());
}

int
TAO_ORB_Core::inherit_from_parent_thread (TAO_ORB_Core_TSS_Resources *tss_resources)
{
  // Inherit properties/objects used in ORB_Core from the
  // parent thread.  Stuff inherited here must already exist
  // in the "parent" orbcore.
  // This is used in the thread-per-connection concurrency model where
  // each ORB spawned thread must use the resources of the spawning
  // thread...

  if (tss_resources->reactor_ != 0)
    {
      // We'll use the spawning thread's reactor.
      TAO_ORB_Core_TSS_Resources* tss = this->get_tss_resources ();
      if (tss->reactor_ != 0 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - non nil reactor on thread startup!\n"));
          if (tss->owns_reactor_ != 0)
            delete tss->reactor_;
        }
      tss->reactor_ = tss_resources->reactor_;
      tss->owns_reactor_ = 0;
    }

  // this->connection_cache (tss_resources->connection_cache_);
  // Inherit connection cache?

  return 0;
}

void
TAO_ORB_Core::create_and_set_root_poa (const char *adapter_name,
                                       TAO_POA_Manager *poa_manager,
                                       const TAO_POA_Policies *policies,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Locking??

  int delete_policies = 0;

  // Need to do double-checked locking here to cover the case of
  // multiple threads using a global resource policy.
  if (poa_manager == 0)
    poa_manager = new TAO_POA_Manager (*this->object_adapter ());

  TAO_POA_Policies *root_poa_policies = 0;
  if (policies == 0)
    {
      root_poa_policies = new TAO_POA_Policies;
      // RootPOA policies defined in spec
      root_poa_policies->implicit_activation (PortableServer::IMPLICIT_ACTIVATION);

      delete_policies = 1;
      policies = root_poa_policies;
    }

  // Construct a new POA
  this->root_poa_ = new TAO_POA (adapter_name,
                                 *poa_manager,
                                 *policies,
                                 0,
                                 this->object_adapter ()->lock (),
                                 this->object_adapter ()->thread_lock (),
                                 *this,
                                 ACE_TRY_ENV);
  ACE_CHECK;

  if (delete_policies)
    delete root_poa_policies;
}

TAO_Object_Adapter *
TAO_ORB_Core::object_adapter (void)
{
  // @@ Double check??
  if (this->object_adapter_ == 0)
    {
      ACE_NEW_RETURN (this->object_adapter_,
                      TAO_Object_Adapter (this->server_factory ()->active_object_map_creation_parameters (),
                                          *this),
                      0);
    }

  return this->object_adapter_;
}

int
TAO_ORB_Core::is_collocated (const TAO_MProfile& mprofile)
{
  if (this->acceptor_registry_ == 0)
    return 0;

  // @@ Lots of issues arrise when dealing with collocation.  What about
  //    forwarding or what if this is a multi-profile IOR where the order is
  //    significant and only one of the profiles is collocated.  For example
  //    when using a multiple servers for fault tolerance.  For now, we just
  //    look through all profiles and if any are colocated then we assume
  //    the object is collocated.
  // @@ Note, if collocated we can not be forwarded!
  //    Also, acceptor_registry_->is_collocated (...) will check the
  //    address (ORB Host) but not the object_key.  This should be checked
  //    also.

  return this->acceptor_registry_->is_collocated (mprofile);
}

int
TAO_ORB_Core::leader_available (void)
  // returns the value of the flag indicating if a leader
  // is available in the leader-follower model
{
  return this->orb ()->leader_follower_info ().leaders_;
}

int
TAO_ORB_Core::I_am_the_leader_thread (void)
  // returns 1 if we are the leader thread,
  // else 0
{
  TAO_Leader_Follower_Info &lf_info = this->orb ()->leader_follower_info ();
  if (lf_info.leaders_)
    return ACE_OS::thr_equal (lf_info.leader_thread_ID_,
                              ACE_Thread::self ());
  return 0;
}

void
TAO_ORB_Core::set_leader_thread (void)
  // sets the thread ID of the leader thread in the leader-follower
  // model
{
  TAO_Leader_Follower_Info &lf_info = this->orb ()->leader_follower_info ();
  ACE_ASSERT ((lf_info.leaders_ >= 1
               && ACE_OS::thr_equal (lf_info.leader_thread_ID_,
                                     ACE_Thread::self ()))
              || lf_info.leaders_ == 0);
  lf_info.leaders_++;
  lf_info.leader_thread_ID_ = ACE_Thread::self ();
}

int
TAO_ORB_Core::unset_leader_wake_up_follower (void)
  // sets the leader_available flag to false and tries to wake up a follower
{
  //  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon,
  //                    this->leader_follower_lock (), -1);

  this->unset_leader_thread ();

  if (this->follower_available () && !this->leader_available ())
    // do it only if a follower is available and no leader is available
    {
      ACE_SYNCH_CONDITION* condition_ptr = this->get_next_follower ();
      if (condition_ptr == 0 || condition_ptr->signal () == -1)
        return -1;
    }
  return 0;
}


void
TAO_ORB_Core::unset_leader_thread (void)
  // sets the flag in the leader-follower model to false
{
  TAO_Leader_Follower_Info &lf_info = this->orb ()->leader_follower_info ();
  ACE_ASSERT ((lf_info.leaders_ > 1
               && ACE_OS::thr_equal (lf_info.leader_thread_ID_,
                                     ACE_Thread::self ()))
              || lf_info.leaders_ == 1);
  lf_info.leaders_--;
}


ACE_SYNCH_MUTEX &
TAO_ORB_Core::leader_follower_lock (void)
  // returns the leader-follower lock
{
  return this->orb ()->leader_follower_info ().leader_follower_lock_;
}

int
TAO_ORB_Core::add_follower (ACE_SYNCH_CONDITION *follower_ptr)
  // adds the a follower to the set of followers in the leader-
  // follower model
  // returns 0 on success, -1 on failure
{
  if (this->orb ()->leader_follower_info ().follower_set_.insert (follower_ptr) != 0)
    return -1;
  return 0;
}

int
TAO_ORB_Core::follower_available (void)
// checks for the availablity of a follower
  // returns 1 on available, 0 else
{
  return !this->orb ()->leader_follower_info ().follower_set_.is_empty ();
}

int
TAO_ORB_Core::remove_follower (ACE_SYNCH_CONDITION *follower_ptr)
  // removes a follower from the leader-follower set
  // returns 0 on success, -1 on failure
{
  return this->orb ()->leader_follower_info ().follower_set_.remove (follower_ptr);
}

ACE_SYNCH_CONDITION*
TAO_ORB_Core::get_next_follower (void)
  // returns randomly a follower from the leader-follower set
  // returns follower on success, else 0
{
  ACE_Unbounded_Set_Iterator<ACE_SYNCH_CONDITION *> iterator (
    this->orb ()->leader_follower_info ().follower_set_);
  if (iterator.first () == 0)
    // means set is empty
    return 0;
  return *iterator;
}

ACE_Allocator*
TAO_ORB_Core::input_cdr_dblock_allocator (void)
{
#if 0
  if (this->input_cdr_dblock_allocator_ == 0)
    {
      this->input_cdr_dblock_allocator_ =
        this->resource_factory ()->input_cdr_dblock_allocator ();
    }
  return this->input_cdr_dblock_allocator_;
#else
  return this->resource_factory ()->input_cdr_dblock_allocator ();
#endif
}

ACE_Allocator*
TAO_ORB_Core::input_cdr_buffer_allocator (void)
{
#if 0
  if (this->input_cdr_buffer_allocator_ == 0)
    {
      this->input_cdr_buffer_allocator_ =
        this->resource_factory ()->input_cdr_buffer_allocator ();
    }
  return this->input_cdr_buffer_allocator_;
#else
  return this->resource_factory ()->input_cdr_buffer_allocator ();
#endif
}

ACE_Allocator*
TAO_ORB_Core::output_cdr_dblock_allocator (void)
{
  TAO_ORB_Core_TSS_Resources* tss = this->get_tss_resources ();

  if (tss->output_cdr_dblock_allocator_ == 0)
    {
      tss->output_cdr_dblock_allocator_ =
        this->resource_factory ()->output_cdr_dblock_allocator ();
    }

  return tss->output_cdr_dblock_allocator_;
}

ACE_Allocator*
TAO_ORB_Core::output_cdr_buffer_allocator (void)
{
  TAO_ORB_Core_TSS_Resources* tss = this->get_tss_resources ();

  if (tss->output_cdr_buffer_allocator_ == 0)
    {
      tss->output_cdr_buffer_allocator_ =
        this->resource_factory ()->output_cdr_dblock_allocator ();
    }

  return tss->output_cdr_buffer_allocator_;
}

ACE_Reactor *
TAO_ORB_Core::reactor (void)
{
  TAO_ORB_Core_TSS_Resources* tss = this->get_tss_resources ();

  if (tss->reactor_ == 0)
    {
      if (this->use_tss_resources_)
        {
          tss->reactor_ = this->resource_factory ()->get_reactor ();
          tss->owns_reactor_ = 1;
        }
      else if (this->reactor_ == 0)
        {
          // @@ Double checked locking!
          this->reactor_ =
            this->resource_factory ()->get_reactor ();
          tss->reactor_ = this->reactor_;
        }
    }

  return tss->reactor_;
}

TAO_POA_Current &
TAO_ORB_Core::poa_current (void) const
{
  return *this->poa_current_;
}

CORBA_Environment*
TAO_ORB_Core::default_environment (void) const
{
  return TAO_TSS_RESOURCES::instance ()->default_environment_;
}

void
TAO_ORB_Core::default_environment (CORBA_Environment* env)
{
  TAO_TSS_RESOURCES::instance ()->default_environment_ = env;
}

#if defined (TAO_HAS_CORBA_MESSAGING)

TAO_Policy_Current &
TAO_ORB_Core::policy_current (void)
{
  return this->policy_current_;
}

#endif /* TAO_HAS_CORBA_MESSAGING */

// ****************************************************************

TAO_ORB_Core_TSS_Resources::TAO_ORB_Core_TSS_Resources (void)
  :  owns_reactor_ (0),
     reactor_ (0),
     output_cdr_dblock_allocator_ (0),
     output_cdr_buffer_allocator_ (0),
     output_cdr_msgblock_allocator_ (0),
     owns_connection_cache_ (0),
     connection_cache_ (0)
{
}

TAO_ORB_Core_TSS_Resources::~TAO_ORB_Core_TSS_Resources (void)
{
  if (this->owns_reactor_)
    {
      delete this->reactor_;
      this->reactor_ = 0;
      this->owns_reactor_ = 0;
    }

  delete this->output_cdr_dblock_allocator_;
  this->output_cdr_dblock_allocator_ = 0;
  delete this->output_cdr_buffer_allocator_;
  this->output_cdr_buffer_allocator_ = 0;
  delete this->output_cdr_msgblock_allocator_;
  this->output_cdr_msgblock_allocator_ = 0;

  if (this->owns_connection_cache_)
    {
      // unimplemented delete this->connection_cache_;
      this->connection_cache_ = 0;
      this->owns_connection_cache_ = 0;
    }
}

// ****************************************************************

TAO_TSS_Resources::TAO_TSS_Resources (void)
  :  poa_current_impl_ (0),
     default_environment_ (&this->tss_environment_)
#if defined (TAO_HAS_CORBA_MESSAGING)
     , policy_current_ (&this->initial_policy_current_)
#endif /* TAO_HAS_CORBA_MESSAGING */
{
}

TAO_TSS_Resources::~TAO_TSS_Resources (void)
{
}

// ****************************************************************

TAO_ORB_Table::TAO_ORB_Table (void)
{
}

TAO_ORB_Table::~TAO_ORB_Table (void)
{
  for (Iterator i = this->begin ();
       i != this->end ();
       i = this->begin ())
    {
      CORBA::release ((*i).int_id_->orb ());
    }
  this->table_.close ();
}

TAO_ORB_Table::Iterator
TAO_ORB_Table::begin (void)
{
  return this->table_.begin ();
}

TAO_ORB_Table::Iterator
TAO_ORB_Table::end (void)
{
  return this->table_.end ();
}

int
TAO_ORB_Table::bind (const char* orb_id,
                     TAO_ORB_Core* orb_core)
{
  ACE_CString id (orb_id);
  return this->table_.bind (id, orb_core);
}

TAO_ORB_Core*
TAO_ORB_Table::find (const char* orb_id)
{
  TAO_ORB_Core* found = 0;
  ACE_CString id (orb_id);
  this->table_.find (id, found);
  return found;
}

int
TAO_ORB_Table::unbind (const char* orb_id)
{
  ACE_CString id (orb_id);
  return this->table_.unbind (id);
}

// ****************************************************************

// This function exists because of Win32's proclivity for expanding
// templates at link time.  Since DLLs are just executables, templates
// get expanded and instantiated at link time.  Thus, if there are
// references to the same template in an application AND in a DLL,
// you're screwed.  Using this function, we workaround this by
// insuring that everybody ALWAYS accesses the same instantiation.
//
// There's room for optimizations by making this inline for the ORB
// core and non-inlined elsewhere, but that can be done later--after
// it works.

TAO_Export TAO_ORB_Core *
TAO_ORB_Core_instance (void)
{
  // @@ This is a slight violation of layering, we should use
  //    TAO_ORB_Core_instance(), but that breaks during startup.
  TAO_ORB_Table* orb_table = TAO_ORB_Table::instance ();
  TAO_ORB_Table::Iterator begin = orb_table->begin ();
  TAO_ORB_Table::Iterator end = orb_table->end ();
  if (begin == end)
    {
      int argc = 0;
      return CORBA::ORB_init (argc, 0, 0)->orb_core_;
    }
  return (*begin).int_id_;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Env_Value<int>;
template class ACE_Env_Value<u_int>;

template class ACE_TSS_Singleton<TAO_TSS_Resources, ACE_SYNCH_MUTEX>;
template class ACE_TSS<TAO_TSS_Resources>;
template class ACE_TSS<TAO_ORB_Core_TSS_Resources>;

template class ACE_Node<ACE_SYNCH_CONDITION*>;
template class ACE_Unbounded_Set<ACE_SYNCH_CONDITION*>;
template class ACE_Unbounded_Set_Iterator<ACE_SYNCH_CONDITION*>;

template class ACE_Singleton<TAO_ORB_Table,ACE_SYNCH_MUTEX>;
template class ACE_Map_Entry<ACE_CString,TAO_ORB_Core*>;
template class ACE_Map_Manager<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Env_Value<int>
#pragma instantiate ACE_Env_Value<u_int>

#pragma instantiate ACE_TSS_Singleton<TAO_TSS_Resources, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS<TAO_TSS_Resources>
#pragma instantiate ACE_TSS<TAO_ORB_Core_TSS_Resources>

#pragma instantiate ACE_Node<ACE_SYNCH_CONDITION*>
#pragma instantiate ACE_Unbounded_Set<ACE_SYNCH_CONDITION*>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_SYNCH_CONDITION*>

#pragma instantiate ACE_Singleton<TAO_ORB_Table,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Entry<ACE_CString,TAO_ORB_Core*>
#pragma instantiate ACE_Map_Manager<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
