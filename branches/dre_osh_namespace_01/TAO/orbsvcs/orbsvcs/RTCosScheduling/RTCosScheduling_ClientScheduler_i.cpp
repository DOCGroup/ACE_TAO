/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    RTCosScheduling_ClientScheduler_i.cpp
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

#include "RTCosScheduling_ClientScheduler_i.h"

#if !defined (__ACE_INLINE__)
#include "RTCosScheduling_ClientScheduler_i.i"
#endif /* __ACE_INLINE__ */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 *  Note - Before making this file with Makefile.RTCosScheduling, please make
 *  sure that you first comment out the following line from the
 *  RTCosSchedulingS.h file:
 *  //#include "tao/PortableServer/PortableServerS.h"
 */

#include "tao/ORB_Core.h"
#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"

namespace TAO
{

  /// Constructor
  RTCosScheduling_ClientScheduler_i::RTCosScheduling_ClientScheduler_i (
      CORBA::ORB_var orb,
      char *node_name,
      char *file)
  {
    ACE_TRY_NEW_ENV
      {
        /// Read the resources and ceilings from the config file
        /// and put them into the activity_map_
        if ( !ACE_OS::strcmp(file,"") || file == NULL)
          {
            ACE_DEBUG((LM_DEBUG,
                       "No config supplied to the ServerScheduler, "
                       "Server will not scheudle object execution "
                       "(ServerScheduler interceptor not installed)"));
          }
        else if (!tasks(node_name, file, &activity_map_))
          {
            ACE_DEBUG((LM_DEBUG,"Invalid Filename given, aborting!\n"));
            ACE_OS::exit(1);
          }
        // The tasks were successfully read in, create the client interceptor
        else
          {
#ifdef TAO_HAS_INTERCEPTORS
            ACE_NEW_THROW_EX (this->client_interceptor_,
                              RTCosScheduling_ClientScheduler_Interceptor(orb),
                              CORBA::NO_MEMORY());
          ACE_CHECK;

          TAO_ORB_Core *orb_core = orb->orb_core();

          /// First, get a list of all interceptors currently registered
          TAO_ClientRequestInterceptor_List::TYPE &interceptors =
            orb_core->client_request_interceptors();


          /// Now check to see if the ServerScheduler Interceptor has already
          /// been registered
          u_int i;
          u_int unregistered = 1;
          for (i = 0; i < interceptors.size() && unregistered; ++i)
           {
             if (ACE_OS::strncmp(interceptors[i]->_interface_repository_id (),
                 this->client_interceptor_->_interface_repository_id(),
                 ACE_OS::strlen(
                   this->client_interceptor_->_interface_repository_id()-2))
               == 0)
               {
                 /// The interceptor is already registered,
                 /// don't try to register it again
                 unregistered = 0;
               }
           }

         /// if the ServerScheduler Interceptor was not registered by
         /// another POA then register it now
         if (unregistered)
           {
             orb_core->add_interceptor(this->client_interceptor_
                                       ACE_ENV_ARG_PARAMETER);
           }
#endif /* TAO_HAS_INTERCEPTORS */

         /// Now resolve a reference to the Real Time ORB
         CORBA::Object_var rt_obj =
           orb->resolve_initial_references("RTORB"
                                           ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK;

         // Get a reference to the real time orb
         RTCORBA::RTORB_var rt_orb =
           RTCORBA::RTORB::_narrow (rt_obj.in ()
                                    ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK;

         /// resolve a reference to RT Current
         rt_obj =
           orb->resolve_initial_references ("RTCurrent"
                                            ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK;

         this->current_ =
           RTCORBA::Current::_narrow (rt_obj.in () ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK

         /// Resolve a reference to the Linear Priority Mapping Manager
         rt_obj =
           orb->resolve_initial_references("PriorityMappingManager"
                                           ACE_ENV_ARG_PARAMETER);
         RTCORBA::PriorityMappingManager_var mapping_manager =
           RTCORBA::PriorityMappingManager::_narrow(rt_obj.in()
                                                    ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK;

         /// Create the Linear Priority Mapping Manager
         ACE_NEW_THROW_EX(this->pm_,
                          TAO_Linear_Priority_Mapping(
                            ACE_SCHED_FIFO),
                          CORBA::NO_MEMORY());
         // WHERE the parameter is one of SCHED_OTHER, SCHED_FIFO, or SCHED_RR

         mapping_manager->mapping(this->pm_);
         ACE_TRY_CHECK;
       }
    }
  ACE_CATCHANY
    {
        ACE_ERROR((LM_ERROR, "Could not configure the orb"));
        ACE_OS::exit(1);
    }
 ACE_ENDTRY;
}


/// Implementation skeleton destructor
RTCosScheduling_ClientScheduler_i::~RTCosScheduling_ClientScheduler_i (void)
{
  delete this->pm_;
#ifdef TAO_HAS_INTERCEPTORS
  delete this->client_interceptor_;
#endif /* TAO_HAS_INTERCEPTORS */
}


void  RTCosScheduling_ClientScheduler_i::schedule_activity (
  const char * activity_name
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    RTCosScheduling::UnknownName))
{
  /// Look up the priority using the activity name in the activity map
  COS_SCHEDULER_ACTIVITY_VALUE priority;
  CORBA::Short result =
  this->activity_map_.find(activity_name,
                           priority);

  /// If the activity/priority was found, set the current to the
  /// appropriate priority.
  if (result != -1)
    {
      this->current_->the_priority(priority
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  /// If the activity was not found, throw an UnknownName exception.
  else
    {
      ACE_THROW (RTCosScheduling::UnknownName());
    }
}


int
RTCosScheduling_ClientScheduler_i::tasks(
  const char  *node_name,
  const char  *file_name,
  CosSchedulerActivityMap *activity_map)
{
  /// get the activity list just for the particular node
  const unsigned int BUF_MAX = 256;

  FILE *fp = ACE_OS::fopen(file_name, "r");
  /// Make sure we can open the file
  if (fp == NULL)
    {
      /// Error return of we cannot open the file.
      ACE_ERROR_RETURN((LM_ERROR,
                        "Could not find the config file %s, aborting\n",
                        file_name),
                        0);
    }


  char line[BUF_MAX], key[16];
  ACE_OS::strsncpy(key, "Node ", sizeof(key));
  ACE_OS::strcat(key, node_name);

  /// Skip to the appropriate node
  ACE_OS::clearerr(fp);
  do
    {
      ACE_OS::fgets(line, BUF_MAX, fp);
    }
  while (ACE_OS::strncmp(line, key, ACE_OS::strlen(key)) != 0);

  /// Make sure we did not hit the end of file
  if (ACE_OS::last_error() == EOF)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "Node %s not found in config file\n",
                        node_name),
                        0);
    }

  /// Skip to the appropriate task section of the node
  ACE_OS::clearerr(fp);
  do
    {
      ACE_OS::fgets(line, BUF_MAX, fp);
    }
  while (ACE_OS::strncmp(line, "Tasks:", ACE_OS::strlen("Tasks:")) != 0);

  /// Make sure we did not hit the end of file
  if (ACE_OS::last_error() == EOF)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "Task list not found for node %s\n",
                        node_name),
                        0);
    }

  CORBA::Short done = 0;
  COS_SCHEDULER_ACTIVITY_KEY name;
  COS_SCHEDULER_ACTIVITY_VALUE priority;
  u_int delimiter;

  /// read each activity/priority pair from the config file
  while (!done)
    {
      /// get the activity name
      ACE_OS::fgets(line, BUF_MAX, fp);

      /// check to make sure we have not reached the end of the list.
      if (ACE_OS::strncmp(line, "END", ACE_OS::strlen("END")) != 0)
        {
          name      = ACE_CString(line);
          delimiter = name.find('\t');
          priority  = ACE_OS::atoi(ACE_OS::strchr(line, '\t'));
          if (delimiter < name.length() && delimiter > 0)
            {
              activity_map->bind(name.substr(0, delimiter), priority);
            }
          else
            {
              ACE_ERROR_RETURN((LM_ERROR,
                                "Error in reading activities from %s",
                                file_name),
                                0);
            }
        }
      else
        {
          done = 1;
        }
    }

  return 1;
}

RTCosScheduling_ClientScheduler_Interceptor::RTCosScheduling_ClientScheduler_Interceptor() : name_("RTCosScheduling_Client_Interceptor")
{
  ACE_TRY_NEW_ENV
    {
      /// resolve a reference to RT Current
      int argc = 0;
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            0,
                                            0
                                            ACE_ENV_ARG_PARAMETER);
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RTCurrent"
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil(obj.in()))
        {
          ACE_OS::exit(1);
        }
      else
        {
          this->current_ =
             RTCORBA::Current::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
        }
      ACE_CHECK;

      obj =
        orb->resolve_initial_references("CodecFactory"
                                        ACE_ENV_ARG_PARAMETER);

      IOP::CodecFactory_var codec_factory;
      if (CORBA::is_nil(obj.in()))
        {
          ACE_DEBUG((LM_DEBUG,
                     "Could not initialize client interceptor, aborting!\n"));
          ACE_OS::exit(1);
        }
      else
        {
          codec_factory = IOP::CodecFactory::_narrow(obj.in());
        }


      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 2;

      this->codec_ = codec_factory->create_codec(encoding);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION,
                          "There was an error constructing the "
                          "ClientScheduler Interceptor\n");
    }
  ACE_ENDTRY;
}


RTCosScheduling_ClientScheduler_Interceptor::RTCosScheduling_ClientScheduler_Interceptor(
  const CORBA::ORB_var orb)  : name_("RTCosScheduling_Client_Interceptor")
{
  ACE_TRY_NEW_ENV
    {
      /// resolve a reference to RT Current
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RTCurrent"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if (CORBA::is_nil(obj.in()))
        {
          ACE_OS::exit(1);
        }
      else
        {
          this->current_ =
             RTCORBA::Current::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
        }
      ACE_TRY_CHECK;

      obj =
        orb->resolve_initial_references("CodecFactory"
                                        ACE_ENV_ARG_PARAMETER);

      // set up the codec factory to create the codec necessary to
      // encode the octet stream for the service context
      IOP::CodecFactory_var codec_factory;
      if (CORBA::is_nil(obj.in()))
        {
          ACE_DEBUG((LM_DEBUG,
                     "Could not initialize client interceptor, aborting!\n"));
          ACE_OS::exit(1);
        }
      else
        {
          codec_factory = IOP::CodecFactory::_narrow(obj.in());
        }


      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 2;

      // Create the codec
      this->codec_ = codec_factory->create_codec(encoding);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION,
                          "Error in creating Client Interceptor\n");
    }
  ACE_ENDTRY;
}


void
RTCosScheduling_ClientScheduler_Interceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_UNUSED_ARG(ri);
  ACE_TRY
    {

      // Get the Corba priority that the activity is currently running at
      CORBA::Any the_priority_as_any;
      the_priority_as_any <<=
        this->current_->the_priority();
      ACE_TRY_CHECK;

      // Set up a service context to hold the priority
      IOP::ServiceContext sc;
      sc.context_id  = IOP::RTCorbaPriority;

      // Convert the priority to an octet stream
      // (that is how service contexts send data)
      sc.context_data =
        ACE_reinterpret_cast(
          IOP::ServiceContext::_tao_seq_CORBA_Octet_ &,
          *this->codec_->encode (the_priority_as_any));
      ACE_TRY_CHECK;

      // add the service context
      ri->add_request_service_context(sc, 0);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "ERROR - in Client interceptor\n");
      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
   ACE_ENDTRY;
}

}
