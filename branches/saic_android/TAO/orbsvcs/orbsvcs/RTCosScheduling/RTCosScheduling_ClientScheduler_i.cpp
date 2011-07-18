// -*- C++ -*-

//=============================================================================
/**
 *  @file    RTCosScheduling_ClientScheduler_i.cpp
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

#include "orbsvcs/RTCosScheduling/RTCosScheduling_ClientScheduler_i.h"

#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_stdio.h"

#include "tao/ORB_Core.h"
#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"
#include "tao/CodecFactory/CodecFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /// Constructor
  RTCosScheduling_ClientScheduler_i::RTCosScheduling_ClientScheduler_i (
      CORBA::ORB_var orb,
      char *node_name,
      char *file)
  {
    try
      {
        /// Read the resources and ceilings from the config file
        /// and put them into the activity_map_
        if ( !ACE_OS::strcmp(file,"") || file == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                       "No config supplied to the ServerScheduler, "
                       "Server will not scheudle object execution "
                       "(ServerScheduler interceptor not installed)"));
          }
        else if (!tasks(node_name, file, &activity_map_))
          {
            ACE_DEBUG ((LM_DEBUG,"Invalid Filename given, aborting!\n"));
            ACE_OS::exit (1);
          }
        // The tasks were successfully read in, create the client interceptor
        else
          {
#if (TAO_HAS_INTERCEPTORS == 1)
            ACE_NEW_THROW_EX (this->client_interceptor_,
                              RTCosScheduling_ClientScheduler_Interceptor(orb),
                              CORBA::NO_MEMORY ());

          TAO_ORB_Core *orb_core = orb->orb_core ();

          // @@ (OO) Why isn't an ORBInitializer being used to
          //         register the interceptor?  In fact, all of the
          //         following code should be done in an
          //         ORBInitializer, except for the interceptor list
          //         iteration.  That is already done by the ORB when
          //         using an ORBInitializer to register an
          //         interceptor.

          /// First, get a list of all interceptors currently registered
          TAO::ClientRequestInterceptor_List::TYPE &interceptors =
            orb_core->client_request_interceptors ();


          /// Now check to see if the ServerScheduler Interceptor has already
          /// been registered
          u_int i;
          u_int unregistered = 1;
          for (i = 0; i < interceptors.size () && unregistered; ++i)
           {
             if (ACE_OS::strncmp(interceptors[i]->_interface_repository_id (),
                 this->client_interceptor_->_interface_repository_id (),
                 ACE_OS::strlen (
                   this->client_interceptor_->_interface_repository_id ()-2))
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
             orb_core->add_interceptor (this->client_interceptor_);
           }
#endif /* TAO_HAS_INTERCEPTORS == 1 */

         /// Now resolve a reference to the Real Time ORB
         CORBA::Object_var rt_obj =
           orb->resolve_initial_references ("RTORB");

         // Get a reference to the real time orb
         RTCORBA::RTORB_var rt_orb =
           RTCORBA::RTORB::_narrow (rt_obj.in ());

         /// resolve a reference to RT Current
         rt_obj =
           orb->resolve_initial_references ("RTCurrent");

         this->current_ =
           RTCORBA::Current::_narrow (rt_obj.in ());

         /// Resolve a reference to the Linear Priority Mapping Manager
         rt_obj =
           orb->resolve_initial_references ("PriorityMappingManager");
         RTCORBA::PriorityMappingManager_var mapping_manager =
           RTCORBA::PriorityMappingManager::_narrow (rt_obj.in ());

         /// Create the Linear Priority Mapping Manager
         ACE_NEW_THROW_EX (this->pm_,
                          TAO_Linear_Priority_Mapping (
                            ACE_SCHED_FIFO),
                          CORBA::NO_MEMORY ());
         // WHERE the parameter is one of SCHED_OTHER, SCHED_FIFO, or SCHED_RR

         mapping_manager->mapping (this->pm_);
       }
    }
  catch (const CORBA::Exception& ex)
    {
        ACE_ERROR ((LM_ERROR, "Could not configure the orb"));
        ACE_OS::exit (1);
    }
}


/// Implementation skeleton destructor
RTCosScheduling_ClientScheduler_i::~RTCosScheduling_ClientScheduler_i (void)
{
  delete this->pm_;
#if (TAO_HAS_INTERCEPTORS == 1)
  delete this->client_interceptor_;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
}


void  RTCosScheduling_ClientScheduler_i::schedule_activity (
  const char * activity_name)
{
  /// Look up the priority using the activity name in the activity map
  COS_SCHEDULER_ACTIVITY_VALUE priority = 0;
  CORBA::Short result =
  this->activity_map_.find (activity_name, priority);

  /// If the activity/priority was found, set the current to the
  /// appropriate priority.
  if (result != -1)
    {
      this->current_->the_priority (priority);
    }
  /// If the activity was not found, throw an UnknownName exception.
  else
    {
      throw RTCosScheduling::UnknownName ();
    }
}


int
RTCosScheduling_ClientScheduler_i::tasks (
  const char  *node_name,
  const char  *file_name,
  CosSchedulerActivityMap *activity_map)
{
  /// get the activity list just for the particular node
  const unsigned int BUF_MAX = 256;

  FILE *fp = ACE_OS::fopen (file_name, "r");
  /// Make sure we can open the file
  if (fp == 0)
    {
      /// Error return of we cannot open the file.
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Could not find the config file %C, aborting\n",
                        file_name),
                        0);
    }


  char line[BUF_MAX], key[64];
  ACE_OS::strsncpy (key, "Node ", sizeof (key));
  ACE_OS::strcat (key, node_name);

  /// Skip to the appropriate node
#ifndef ACE_LACKS_CLEARERR
  ACE_OS::clearerr (fp);
#else
# warning ACE_OS::clearerr() is unimplemented on this platform.
# warning This code may not function properly.
#endif  /* !ACE_LACKS_CLEARERR */
  do
    {
      ACE_OS::fgets (line, BUF_MAX, fp);
  /// Make sure we did not hit the end of file
  if (ACE_OS::last_error () == EOF)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Node %C not found in config file\n",
                        node_name),
                        0);
          break;
        }
    }
  while (ACE_OS::strncmp (line, key, ACE_OS::strlen (key)) != 0);

  /// Skip to the appropriate task section of the node
#ifndef ACE_LACKS_CLEARERR
  ACE_OS::clearerr (fp);
#else
# warning ACE_OS::clearerr() is unimplemented on this platform.
# warning This code may not function properly.
#endif  /* !ACE_LACKS_CLEARERR */
  do
    {
      ACE_OS::fgets (line, BUF_MAX, fp);
  /// Make sure we did not hit the end of file
      if (ACE_OS::last_error () == EOF)
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            "Task list not found for node %C\n",
                            node_name),
                            0);
              break;
        }
    }
  while (ACE_OS::strncmp (line, "Tasks:", ACE_OS::strlen ("Tasks:")) != 0);

  CORBA::Short done = 0;
  COS_SCHEDULER_ACTIVITY_KEY name;
  COS_SCHEDULER_ACTIVITY_VALUE priority = 0;
  u_int delimiter;

  /// read each activity/priority pair from the config file
  while (!done)
    {
      /// get the activity name
      ACE_OS::fgets (line, BUF_MAX, fp);

      /// Make sure we did not hit the end of file
      if (ACE_OS::last_error () == EOF)
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            "Task list not found for node %C\n",
                            node_name),
                            0);
          break;
        }

      /// check to make sure we have not reached the end of the list.
      if (ACE_OS::strncmp (line, "END", ACE_OS::strlen ("END")) != 0)
        {
          name      = ACE_CString (line);
          delimiter = name.find ('\t');
          char *p = ACE_OS::strchr (line, '\t');
          if (p)
            priority  = ACE_OS::atoi (p);
          if (priority == 0)
              priority = RTCORBA::minPriority;
          if (delimiter < name.length () && delimiter > 0)
            {
              activity_map->bind (name.substr (0, delimiter), priority);
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                "Error in reading activities from %C",
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


#if (TAO_HAS_INTERCEPTORS == 1)

RTCosScheduling_ClientScheduler_Interceptor::RTCosScheduling_ClientScheduler_Interceptor () : name_ ("RTCosScheduling_Client_Interceptor")
{
  try
    {
      /// resolve a reference to RT Current
      int argc = 0;
      ACE_TCHAR **argv= 0;
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RTCurrent");
      if (CORBA::is_nil (obj.in ()))
        {
          ACE_OS::exit (1);
        }
      else
        {
          this->current_ =
             RTCORBA::Current::_narrow (obj.in ());
        }

      obj =
        orb->resolve_initial_references ("CodecFactory");

      IOP::CodecFactory_var codec_factory;
      if (CORBA::is_nil (obj.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Could not initialize client interceptor, aborting!\n"));
          ACE_OS::exit (1);
        }
      else
        {
          codec_factory = IOP::CodecFactory::_narrow (obj.in ());
        }


      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 2;

      this->codec_ = codec_factory->create_codec (encoding);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "There was an error constructing the "
        "ClientScheduler Interceptor\n");
    }
}


RTCosScheduling_ClientScheduler_Interceptor::RTCosScheduling_ClientScheduler_Interceptor (
  const CORBA::ORB_var orb)  : name_ ("RTCosScheduling_Client_Interceptor")
{
  try
    {
      /// resolve a reference to RT Current
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RTCurrent");
      if (CORBA::is_nil (obj.in ()))
        {
          ACE_OS::exit (1);
        }
      else
        {
          this->current_ =
             RTCORBA::Current::_narrow (obj.in ());
        }

      obj =
        orb->resolve_initial_references ("CodecFactory");

      // set up the codec factory to create the codec necessary to
      // encode the octet stream for the service context
      IOP::CodecFactory_var codec_factory;
      if (CORBA::is_nil (obj.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Could not initialize client interceptor, aborting!\n"));
          ACE_OS::exit (1);
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
      this->codec_ = codec_factory->create_codec (encoding);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Error in creating Client Interceptor\n");
    }
}

char *
RTCosScheduling_ClientScheduler_Interceptor::name ()
{
return CORBA::string_dup (this->name_);
}

void
RTCosScheduling_ClientScheduler_Interceptor::destroy ()
{
}

void
RTCosScheduling_ClientScheduler_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  try
    {

      // Get the Corba priority that the activity is currently running at
      CORBA::Any the_priority_as_any;
      the_priority_as_any <<=
        this->current_->the_priority ();

      // Set up a service context to hold the priority
      IOP::ServiceContext sc;
      sc.context_id  = IOP::RTCorbaPriority;

      // Convert the priority to an octet stream
      // (that is how service contexts send data)
      sc.context_data =
        reinterpret_cast<CORBA::OctetSeq &> (*this->codec_->encode (the_priority_as_any));

      // add the service context
      ri->add_request_service_context (sc, 0);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ERROR - in Client interceptor\n");
      throw CORBA::INTERNAL ();
    }
}

void
RTCosScheduling_ClientScheduler_Interceptor::send_poll (
  PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
RTCosScheduling_ClientScheduler_Interceptor::receive_reply (
  PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
RTCosScheduling_ClientScheduler_Interceptor::receive_exception (
  PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
RTCosScheduling_ClientScheduler_Interceptor::receive_other (
  PortableInterceptor::ClientRequestInfo_ptr)
{
}

#endif /* TAO_HAS_INTERCEPTORS == 1 */

}

TAO_END_VERSIONED_NAMESPACE_DECL
