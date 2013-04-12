// $Id$

#include "orbsvcs/PortableGroup/miop_resource.h"
#include "orbsvcs/PortableGroup/miopconf.h"
#include "orbsvcs/PortableGroup/Fragments_Cleanup_Strategy.h"

#include "tao/debug.h"

#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_MIOP_Resource_Factory::TAO_MIOP_Resource_Factory (void)
  : fragments_cleanup_strategy_type_ (TAO_MIOP_CLEANUP_TIME_BOUND)
  , fragments_cleanup_bound_   (-1)
  , fragments_cleanup_strategy_ (0)
  , max_fragments_       (TAO_DEFAULT_MIOP_MAX_FRAGMENTS)
  , max_fragment_size_   (TAO_DEFAULT_MIOP_FRAGMENT_SIZE)
  , max_fragment_rate_   (0u) // Zero uses max_fragment_size_ instead.
  , send_hi_water_mark_  (0u) // Zero sets this to actual -ORBSndSock
  , send_buffer_size_    (0u) // Zero is unspecified (-ORBSndSock).
  , receive_buffer_size_ (0u) // Zero is unspecified (-ORBRcvSock).
  , enable_throttling_    (!!(TAO_DEFAULT_MIOP_SEND_THROTTLING))  // Client-side SendRate throttling enabled.
  , enable_eager_dequeue_ (!!(TAO_DEFAULT_MIOP_EAGER_DEQUEUEING)) // Server-side Multiple message dequeueing.
{
}

TAO_MIOP_Resource_Factory::~TAO_MIOP_Resource_Factory (void)
{
  delete this->fragments_cleanup_strategy_;
}

int
TAO_MIOP_Resource_Factory::init (int argc, ACE_TCHAR *argv[])
{
  ACE_TRACE ("TAO_MIOP_Resource_Factory::init");

  for (int curarg = 0; curarg < argc; ++curarg)
    {
      if (ACE_OS::strcasecmp (argv[curarg],
                              ACE_TEXT ("-ORBFragmentsCleanupStrategy")) == 0)
        {
          if (++curarg < argc)
            {
              ACE_TCHAR* name = argv[curarg];

              if (ACE_OS::strcasecmp (name,
                                      ACE_TEXT ("delay")) == 0)
                {
                  this->fragments_cleanup_strategy_type_ =
                    TAO_MIOP_CLEANUP_TIME_BOUND;
                }
              else if (ACE_OS::strcasecmp (name,
                                           ACE_TEXT ("number")) == 0)
                {
                  this->fragments_cleanup_strategy_type_ =
                    TAO_MIOP_CLEANUP_NUMBER_BOUND;
                }
              else if (ACE_OS::strcasecmp (name,
                                           ACE_TEXT ("memory")) == 0)
                {
                  this->fragments_cleanup_strategy_type_ =
                    TAO_MIOP_CLEANUP_MEMORY_BOUND;
                }
              else
                ACE_DEBUG ((LM_ERROR,
                            ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                            ACE_TEXT ("-ORBFragmentsCleanupStrategy %s is unknown.\n"),
                            name));
            }
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                        ACE_TEXT ("-ORBFragmentsCleanupStrategy missing type.\n")));
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBFragmentsCleanupBound")) == 0)
        {
          if (++curarg < argc)
            this->fragments_cleanup_bound_ = ACE_OS::atoi (argv[curarg]);
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                        ACE_TEXT ("-ORBFragmentsCleanupBound missing value.\n")));
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBMaxFragments")) == 0)
        {
          if (++curarg < argc)
            {
              int const max= ACE_OS::atoi (argv[curarg]);
              if (max < 0)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                              ACE_TEXT ("-ORBMaxFragments %d ")
                              ACE_TEXT ("is invalid (using %u).\n"),
                              max,
                              TAO_DEFAULT_MIOP_MAX_FRAGMENTS));
                  this->max_fragments_ = TAO_DEFAULT_MIOP_MAX_FRAGMENTS;
                }
              else
                this->max_fragments_ = static_cast<u_long> (max);
            }
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                        ACE_TEXT ("-ORBMaxFragments missing limit.\n")));
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBMaxFragmentSize")) == 0)
        {
          if (++curarg < argc)
            {
              int const size= ACE_OS::atoi (argv[curarg]);
              if (size < static_cast<int> (MIOP_MAX_HEADER_SIZE) ||
                  size > static_cast<int> (MIOP_MAX_DGRAM_SIZE)    )
                {
                  ACE_DEBUG ((LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                              ACE_TEXT ("-ORBMaxFragmentSize %d is not within ")
                              ACE_TEXT ("range %u to %u (using %u).\n"),
                              size,
                              MIOP_MAX_HEADER_SIZE,
                              MIOP_MAX_DGRAM_SIZE,
                              TAO_DEFAULT_MIOP_FRAGMENT_SIZE));
                  this->max_fragment_size_ = TAO_DEFAULT_MIOP_FRAGMENT_SIZE;
                }
              else
                this->max_fragment_size_ = static_cast<u_long> (size);
            }
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                        ACE_TEXT ("-ORBMaxFragmentSize missing limit.\n")));
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBMaxFragmentRate")) == 0)
        {
          if (++curarg < argc)
            {
              int const tx_time= ACE_OS::atoi (argv[curarg]);
              if (tx_time <= 0)
                {
                  ACE_DEBUG ((LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                              ACE_TEXT ("-ORBMaxFragmentRate %d is invalid ")
                              ACE_TEXT ("(using -ORBMaxFragmentSize in micro seconds).\n"),
                              tx_time));
                  this->max_fragment_rate_= 0u; // Zero uses configured max_fragment_size_
                }
              else
                this->max_fragment_rate_= static_cast<u_long> (tx_time);
            }
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                        ACE_TEXT ("-ORBMaxFragmentRate missing micro-seconds.\n")));
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBSendHighWaterMark")) == 0 ||
               ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBSendHiWaterMark")) == 0 ||
               ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBSendHWM")) == 0 ||
               ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBSndHighWaterMark")) == 0 ||
               ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBSndHiWaterMark")) == 0 ||
               ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBSndHWM")) == 0)
        {
          if (++curarg < argc)
            {
              int const hwm= ACE_OS::atoi (argv[curarg]);
              if (hwm <= 0)
                {
                  ACE_DEBUG ((LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                              ACE_TEXT ("%s %d is invalid ")
                              ACE_TEXT ("(using actual -ORBSndSock size).\n"),
                              argv[curarg-1],
                              hwm,
                              TAO_DEFAULT_MIOP_MAX_FRAGMENTS));
                  this->send_hi_water_mark_ = 0u; // Zero sets this to actual -ORBSndSock
                }
              else
                this->send_hi_water_mark_ = static_cast<u_long> (hwm);
            }
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                        ACE_TEXT ("%s missing limit.\n"),
                        argv[curarg-1]));
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBSndSock")) == 0)
        {
          if (++curarg < argc)
            {
              int const bytes= ACE_OS::atoi (argv[curarg]);
              if (bytes <= 0)
                {
                  ACE_DEBUG ((LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                              ACE_TEXT ("-ORBSndSock %d is invalid.\n"),
                              bytes));
                  this->send_buffer_size_= 0u; // Zero is unspecified
                }
              else
                this->send_buffer_size_= static_cast<u_long> (bytes);
            }
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                        ACE_TEXT ("-ORBSndSock missing size in bytes.\n")));
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBRcvSock")) == 0)
        {
          if (++curarg < argc)
            {
              int const bytes= ACE_OS::atoi (argv[curarg]);
              if (bytes <= 0)
                {
                  ACE_DEBUG ((LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                              ACE_TEXT ("-ORBRcvSock %d is invalid.\n"),
                              bytes));
                  this->receive_buffer_size_= 0u; // Zero is unspecified
                }
              else
                this->receive_buffer_size_= static_cast<u_long> (bytes);
            }
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                        ACE_TEXT ("-ORBRcvSock missing size in bytes.\n")));
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBSendThrottling")) == 0 ||
               ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBSendThrottle")) == 0)
        {
          if (++curarg < argc)
            this->enable_throttling_= static_cast<bool> (ACE_OS::atoi (argv[curarg]));
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                        ACE_TEXT ("%s missing 0 or 1 parameter.\n"),
                        argv[curarg-1]));
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBEagerDequeueing")) == 0 ||
               ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT ("-ORBEagerDequeue")) == 0)
        {
          if (++curarg < argc)
            this->enable_eager_dequeue_= static_cast<bool> (ACE_OS::atoi (argv[curarg]));
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory ")
                        ACE_TEXT ("%s missing 0 or 1 parameter.\n"),
                        argv[curarg-1]));
        }
      else if (ACE_OS::strncmp (argv[curarg], ACE_TEXT ("-ORB"), 4) == 0)
        {
          // Can we assume there is an argument after the option?
          // ++curarg;
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory - ")
                      ACE_TEXT ("unknown option <%s>.\n"),
                      argv[curarg]));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - MIOP_Resource_Factory - ")
                      ACE_TEXT ("ignoring option <%s>.\n"),
                      argv[curarg]));
        }
    }

  return 0;
}

TAO_PG::Fragments_Cleanup_Strategy *
TAO_MIOP_Resource_Factory::fragments_cleanup_strategy (void) const
{
  if (this->fragments_cleanup_strategy_ == 0)
    {
      if (this->fragments_cleanup_strategy_type_ ==
          TAO_MIOP_CLEANUP_TIME_BOUND)
        {
          int bound = this->fragments_cleanup_bound_;
          if (bound == -1)
            bound = TAO_DEFAULT_MIOP_FRAGMENTS_CLEANUP_DELAY;

          ACE_NEW_RETURN (this->fragments_cleanup_strategy_,
                          TAO_PG::Time_Bound_Fragments_Cleanup_Strategy (
                            bound),
                          0);
        }
      else if (this->fragments_cleanup_strategy_type_ ==
               TAO_MIOP_CLEANUP_NUMBER_BOUND)
        {
          int bound = this->fragments_cleanup_bound_;
          if (bound == -1)
            bound = TAO_DEFAULT_MIOP_FRAGMENTS_CLEANUP_NUMBER;

          ACE_NEW_RETURN (this->fragments_cleanup_strategy_,
                          TAO_PG::Number_Bound_Fragments_Cleanup_Strategy (
                            bound),
                          0);
        }
      else if (this->fragments_cleanup_strategy_type_ ==
               TAO_MIOP_CLEANUP_MEMORY_BOUND)
        {
          int bound = this->fragments_cleanup_bound_;
          if (bound == -1)
            bound = TAO_DEFAULT_MIOP_FRAGMENTS_CLEANUP_MEMORY;

          ACE_NEW_RETURN (this->fragments_cleanup_strategy_,
                          TAO_PG::Memory_Bound_Fragments_Cleanup_Strategy (
                            bound),
                          0);
        }
    }

  return this->fragments_cleanup_strategy_;
}

u_long
TAO_MIOP_Resource_Factory::max_fragment_size (void) const
{
  return this->max_fragment_size_;
}

u_long
TAO_MIOP_Resource_Factory::max_fragments (void) const
{
  return this->max_fragments_;
}

u_long
TAO_MIOP_Resource_Factory::max_fragment_rate (void) const
{
  // If "max_fragment_rate_" is not specified (i.e. zero)
  // use the same value as "max_fragment_size_".
  return this->max_fragment_rate_ ?
         this->max_fragment_rate_ :
         this->max_fragment_size_ ;
}

u_long
TAO_MIOP_Resource_Factory::send_hi_water_mark (void) const
{
  return this->send_hi_water_mark_;
}

u_long
TAO_MIOP_Resource_Factory::send_buffer_size (void) const
{
  return send_buffer_size_;
}

u_long
TAO_MIOP_Resource_Factory::receive_buffer_size (void) const
{
  return receive_buffer_size_;
}

bool
TAO_MIOP_Resource_Factory::enable_throttling (void) const
{
  return enable_throttling_;
}

bool
TAO_MIOP_Resource_Factory::enable_eager_dequeue (void) const
{
  return enable_eager_dequeue_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_MIOP_Resource_Factory,
                       ACE_TEXT ("MIOP_Resource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_MIOP_Resource_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_PortableGroup, TAO_MIOP_Resource_Factory)
