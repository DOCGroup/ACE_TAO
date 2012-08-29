// $Id$

#include "tao/default_server.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Server_Strategy_Factory::TAO_Default_Server_Strategy_Factory (void)
  : activate_server_connections_ (0),
    thread_flags_ (THR_BOUND | THR_DETACHED),
    thread_per_connection_use_timeout_ (-1)
{
}

TAO_Default_Server_Strategy_Factory::~TAO_Default_Server_Strategy_Factory (void)
{
  // Perform appropriate cleanup.
}

int
TAO_Default_Server_Strategy_Factory::activate_server_connections (void)
{
  return this->activate_server_connections_;
}

int
TAO_Default_Server_Strategy_Factory::thread_per_connection_timeout (ACE_Time_Value &timeout)
{
  timeout = this->thread_per_connection_timeout_;
  return this->thread_per_connection_use_timeout_;
}

int
TAO_Default_Server_Strategy_Factory::server_connection_thread_flags (void)
{
  return this->thread_flags_;
}

int
TAO_Default_Server_Strategy_Factory::server_connection_thread_count (void)
{
  return 1;
}

// Evil macros b/c I'm lazy!
#define TAO_BEGINCHECK  if (0)
#define TAO_CHECKANDSET(sym) \
  else if (ACE_OS::strcmp (const_cast <const ACE_TCHAR *> ( flag), ACE_TEXT(#sym)) == 0) \
  ACE_SET_BITS (this->thread_flags_, sym)
#define TAO_ENDCHECK

void
TAO_Default_Server_Strategy_Factory::tokenize (ACE_TCHAR* flag_string)
{
  ACE_TCHAR* lasts = 0;

  for (ACE_TCHAR* flag = ACE_OS::strtok_r (flag_string,
                                      ACE_TEXT("|"),
                                      &lasts);
       flag != 0;
       flag = ACE_OS::strtok_r (0,
                                ACE_TEXT("|"),
                                &lasts))
    {
      TAO_BEGINCHECK;
      TAO_CHECKANDSET (THR_DETACHED);
      TAO_CHECKANDSET (THR_BOUND);
      TAO_CHECKANDSET (THR_NEW_LWP);
      TAO_CHECKANDSET (THR_SUSPENDED);
#if !defined (ACE_WIN32)
      TAO_CHECKANDSET (THR_DAEMON);
#endif /* ACE_WIN32 */
      TAO_ENDCHECK;
    }
}

int
TAO_Default_Server_Strategy_Factory::init (int argc, ACE_TCHAR* argv[])
{
  return this->parse_args (argc, argv);
}

int
TAO_Default_Server_Strategy_Factory::open (TAO_ORB_Core *)
{
  return 0;
}

int
TAO_Default_Server_Strategy_Factory::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_TRACE ("TAO_Default_Server_Strategy_Factory::parse_args");

  int curarg;

  for (curarg = 0; curarg < argc && argv[curarg]; ++curarg)
    if (ACE_OS::strcasecmp (argv[curarg],
                            ACE_TEXT("-ORBConcurrency")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("reactive")) == 0)
              this->activate_server_connections_ = 0;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("thread-per-connection")) == 0)
              this->activate_server_connections_ = 1;
            else
              this->report_option_value_error (ACE_TEXT("-ORBConcurrency"), name);
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBThreadPerConnectionTimeout")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("infinite")) == 0)
              {
                this->thread_per_connection_use_timeout_ = 0;
              }
            else
              {
                this->thread_per_connection_use_timeout_ = 1;
                int milliseconds = ACE_OS::atoi (name);
                this->thread_per_connection_timeout_.set (0,
                                                          1000 * milliseconds);
              }
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBTableSize")) == 0
             || ACE_OS::strcasecmp (argv[curarg],
                                    ACE_TEXT("-ORBActiveObjectMapSize")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          this->active_object_map_creation_parameters_.active_object_map_size_ =
            ACE_OS::strtoul (argv[curarg],
                             0,
                             10);
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBPOAMapSize")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          this->active_object_map_creation_parameters_.poa_map_size_ =
            ACE_OS::strtoul (argv[curarg],
                             0,
                             10);
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBActiveHintInIds")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* value = argv[curarg];

            this->active_object_map_creation_parameters_.use_active_hint_in_ids_ =
              ACE_OS::atoi (value);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBActiveHintInPOANames")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* value = argv[curarg];

            this->active_object_map_creation_parameters_.use_active_hint_in_poa_names_ =
              ACE_OS::atoi (value);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBAllowReactivationOfSystemids")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* value = argv[curarg];

            this->active_object_map_creation_parameters_.allow_reactivation_of_system_ids_ =
              ACE_OS::atoi (value);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBUseridPolicyDemuxStrategy")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            // Active demux not supported with user id policy.
            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("dynamic")) == 0)
              this->active_object_map_creation_parameters_.object_lookup_strategy_for_user_id_policy_ =
                TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("linear")) == 0)
              this->active_object_map_creation_parameters_.object_lookup_strategy_for_user_id_policy_ =
                TAO_LINEAR;
            else
              this->report_option_value_error (ACE_TEXT("-ORBUseridPolicyDemuxStrategy"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBSystemidPolicyDemuxStrategy")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("dynamic")) == 0)
              this->active_object_map_creation_parameters_.object_lookup_strategy_for_system_id_policy_ =
                TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("linear")) == 0)
              this->active_object_map_creation_parameters_.object_lookup_strategy_for_system_id_policy_ =
                TAO_LINEAR;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("active")) == 0)
              this->active_object_map_creation_parameters_.object_lookup_strategy_for_system_id_policy_ =
                TAO_ACTIVE_DEMUX;
            else
              this->report_option_value_error (ACE_TEXT("-ORBSystemidPolicyDemuxStrategy"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBPersistentidPolicyDemuxStrategy")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            // Active demux not supported with user id policy.
            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("dynamic")) == 0)
              this->active_object_map_creation_parameters_.poa_lookup_strategy_for_persistent_id_policy_ =
                TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("linear")) == 0)
              this->active_object_map_creation_parameters_.poa_lookup_strategy_for_persistent_id_policy_ =
                TAO_LINEAR;
            else
              this->report_option_value_error (ACE_TEXT("-ORBPersistentidPolicyDemuxStrategy"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBTransientidPolicyDemuxStrategy")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("dynamic")) == 0)
              this->active_object_map_creation_parameters_.poa_lookup_strategy_for_transient_id_policy_ =
                TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("linear")) == 0)
              this->active_object_map_creation_parameters_.poa_lookup_strategy_for_transient_id_policy_ =
                TAO_LINEAR;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("active")) == 0)
              this->active_object_map_creation_parameters_.poa_lookup_strategy_for_transient_id_policy_ =
                TAO_ACTIVE_DEMUX;
            else
              this->report_option_value_error (ACE_TEXT("-ORBTransientidPolicyDemuxStrategy"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBUniqueidPolicyReverseDemuxStrategy")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("dynamic")) == 0)
              this->active_object_map_creation_parameters_.reverse_object_lookup_strategy_for_unique_id_policy_ =
                TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("linear")) == 0)
              this->active_object_map_creation_parameters_.reverse_object_lookup_strategy_for_unique_id_policy_ =
                TAO_LINEAR;
            else
              this->report_option_value_error (ACE_TEXT("-ORBUniqueidPolicyReverseDemuxStrategy"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBThreadFlags")) == 0)
      {
        ++curarg;

        if (curarg < argc)
          this->tokenize (argv[curarg]);
      }

    else if (ACE_OS::strncmp (argv[curarg], ACE_TEXT("-ORB"), 4) == 0)
      {
        // Can we assume there is an argument after the option?
        // ++curarg;
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT("Server_Strategy_Factory - ")
                    ACE_TEXT("unknown option <%s>\n"),
                    argv[curarg]));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Server_Strategy_Factory - ")
                    ACE_TEXT("ignoring option <%s>\n"),
                    argv[curarg]));
      }

  return 0;
}

void
TAO_Default_Server_Strategy_Factory::report_option_value_error (
                                 const ACE_TCHAR* option_name,
                                 const ACE_TCHAR* option_value)
{
  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT ("Server_Strategy_Factory - unknown argument")
             ACE_TEXT (" <%s> for <%s>\n"),
             option_value, option_name));
}

ACE_STATIC_SVC_DEFINE (TAO_Default_Server_Strategy_Factory,
                       ACE_TEXT ("Server_Strategy_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Server_Strategy_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Server_Strategy_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

