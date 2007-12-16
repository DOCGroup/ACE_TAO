//=============================================================================
/**
 *  @file    RTCosScheduling_ServerScheduler_i.cpp
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

#include "orbsvcs/RTCosScheduling/RTCosScheduling_ServerScheduler_i.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/RTCosScheduling/RTCosScheduling_ServerScheduler_i.inl"
#endif /* __ACE_INLINE__ */

#include "tao/ORB_Core.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/RTCORBA/Linear_Priority_Mapping.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/CodecFactory/CodecFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO {

RTCosScheduling_ServerScheduler_i::
  RTCosScheduling_ServerScheduler_i (char *node,
    char *file,
    char *shared_file,
    int numthreads
  )
: num_threads_(numthreads),
  shared_file_(shared_file)
{
  /// Read the Resources and ceilings from the config file
  /// and put them into the resource_map_
  if ( !ACE_OS::strcmp(file,"") || file == 0 || !TAO_HAS_INTERCEPTORS)
    {
      ACE_DEBUG((LM_DEBUG,
                 "Server will not schedule object execution "
                 "(ServerScheduler interceptor not installed)"));
    }
  else if (!resources(node, file, &this->resource_map_))
    {
      ACE_OS::exit(1);
    }
}


// Implementation skeleton destructor
RTCosScheduling_ServerScheduler_i::~RTCosScheduling_ServerScheduler_i (void)
{
  delete this->pm_;
}


int
RTCosScheduling_ServerScheduler_i::configure_ORB(TAO_ORB_Core *orb_core)
{
  try
    {

     // Get an object reference to orb from the orb core
     CORBA::ORB_ptr orb = orb_core->orb();

#if TAO_HAS_INTERCEPTORS
      /// Construct the interceptor that the Scheduling service uses
      RTCosScheduling_ServerScheduler_Interceptor *server_interceptor = 0;;
      ACE_NEW_THROW_EX(server_interceptor,
        RTCosScheduling_ServerScheduler_Interceptor(orb,
          this->shared_file_,
          &this->object_name_map_,
          &this->resource_map_),
          CORBA::NO_MEMORY());

      /// First, get a list of all interceptors currently registered
      TAO::ServerRequestInterceptor_List::TYPE &interceptors =
        orb_core->server_request_interceptors();


      /// Now check to see if the ServerScheduler Interceptor has already
      /// been registered
      u_int i;
      u_int unregistered = 1;
      for (i = 0; i < interceptors.size() && unregistered; ++i)
         {
          if (ACE_OS::strncmp(
                interceptors[i]->_interface_repository_id (),
                server_interceptor->_interface_repository_id(),
                ACE_OS::strlen(
                  server_interceptor->_interface_repository_id())-2)
            == 0)
            {
              /// The interceptor is already registered,
              /// don't try to register it again
              unregistered = 0;
            }
        }

      if (unregistered)
        {
          orb_core->add_interceptor(server_interceptor);
        }
#endif /*  TAO_HAS_INTERCEPTORS */

      /// Resolve a reference to the Linear Priority Mapping Manager
     CORBA::Object_var rt_obj =
        orb->resolve_initial_references("PriorityMappingManager");
      if (CORBA::is_nil(rt_obj.in()))
        {
          ACE_DEBUG((LM_DEBUG,
                     "Priority Mapping Manager not available, "
                    "RT not used on server!\n"));
          return 0;
        }
      RTCORBA::PriorityMappingManager_var mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow(rt_obj.in());

      /// Create the Linear Priority Mapping Manager
      ACE_NEW_THROW_EX(this->pm_,
                       TAO_Linear_Priority_Mapping(
                         ACE_SCHED_FIFO),
                      CORBA::NO_MEMORY());

      mapping_manager->mapping(this->pm_);

   }
 catch (const CORBA::Exception& ex)
   {
        ACE_ERROR((LM_ERROR, "Could not configure the orb"));
        ACE_OS::exit(1);
    }
 return 1;
}


::PortableServer::POA_ptr RTCosScheduling_ServerScheduler_i::create_POA (
    PortableServer::POA_ptr parent,
    const char * adapter_name,
    PortableServer::POAManager_ptr a_POAManager,
    const CORBA::PolicyList & policies)
{
  try
    {
        // We should hopefully be using more than one thread
#if defined (ACE_HAS_THREADS)
      u_int has_threads = 2;
#else
      u_int has_threads = 1;
#endif /* ACE_HAS_THREADS */

      /// Get the ORB core from the POA Manager
      TAO_Root_POA *tao_poa = dynamic_cast<TAO_Root_POA*>(parent);

      TAO_ORB_Core &orb_core = tao_poa->orb_core();

      /// configure the orb (linear mapping, register interceptor, etc.)
      configure_ORB(&orb_core);

      /// Get an object reference to orb from the orb core
      CORBA::ORB_ptr orb = orb_core.orb();

      /// Now resolve a reference to the Real Time ORB
      CORBA::Object_var rt_obj =
        orb->resolve_initial_references("RTORB");
      if (CORBA::is_nil(rt_obj.in()))
        {
          ACE_DEBUG((LM_DEBUG,
                     "RTORB not available, "
                     "not using RT on the server!\n"));
          return 0;
        }

      /// Get the reference to the RT ORB
      RTCORBA::RTORB_var rt_orb =
         RTCORBA::RTORB::_narrow (rt_obj.in ());

      if (CORBA::is_nil(rt_orb.in ()))
        {
          ACE_DEBUG((LM_DEBUG,
                     "Could not initialize orb for the server interceptor, "
                      "RT will not be used!\n"));
          return 0;
        }


      // Copy the non realtime policy list
      CORBA::PolicyList poa_policy_list = policies;
      poa_policy_list.length (policies.length()+has_threads);

      /// Set the server to run at max priority so it will immediately
      /// process intercepts and place new requests into the approprate queue
      poa_policy_list[policies.length()] =
        rt_orb->create_priority_model_policy (
          RTCORBA::SERVER_DECLARED,
          RTCORBA::maxPriority);

      // Set up the threadpool
#if defined (ACE_HAS_THREADS)
      CORBA::ULong stacksize                 = 0;

      /// ideally 1, but not implemented in TAO.  This is fine as long
      /// the system is not overloaded and can accept all requests
      CORBA::Boolean allow_request_buffering = 0;

      // ideally max_concurrent - num_threads_, but no request buffer in TAO
      CORBA::ULong max_buffered_requests     = 0;

      /// TAO does not have, nor plan to implement, request buffering
      CORBA::ULong max_request_buffer_size   = 0;

      /// This comes from the model of the system
      CORBA::ULong static_threads            = this->num_threads_;

      CORBA::ULong dynamic_threads           = 0;

      /// Set it to max so there will be no priority inversions
      /// while the request is accepted
      CORBA::ULong default_thread_priority   = RTCORBA::maxPriority;

      /// Create the threadpool the server uses to execute requests
      RTCORBA::ThreadpoolId threadpool =
        rt_orb->create_threadpool (stacksize,
                                          static_threads,
                                          dynamic_threads,
                                          default_thread_priority,
                                          allow_request_buffering,
                                          max_buffered_requests,
                                          max_request_buffer_size);

      poa_policy_list[policies.length()+1] =
        rt_orb->create_threadpool_policy (threadpool);
#endif /* ACE_HAS_THREADS */


      /// Create the RT POA
      PortableServer::POA_var poa =
        parent->create_POA (adapter_name,
                            a_POAManager,
                            poa_policy_list);

      RTPortableServer::POA_var rt_poa =
        RTPortableServer::POA::_narrow(poa.in());

      /// return the reference to the RT POA
      return rt_poa.in();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "ERROR: Could not create a Scheduling Service POA\n");
      return parent;
    }
  return parent;
}

void
RTCosScheduling_ServerScheduler_i::schedule_object (
    CORBA::Object_ptr obj,
    const char * name)
{
  /// Check and see if the object name is in the resource map,
  /// if it is, then there is a ceiling for it and we can add it to the
  /// object name map.  If there is no name in the resource map, then there
  /// is no valid ceiling for it, throw an UnknownName exception
  if (this->resource_map_.find(name) == -1)
    {
      throw RTCosScheduling::UnknownName();
    }
  else
    {
     this->object_name_map_.rebind(obj, name);
    }
}



#if TAO_HAS_INTERCEPTORS
RTCosScheduling_ServerScheduler_Interceptor::RTCosScheduling_ServerScheduler_Interceptor(
  CORBA::ORB_ptr orb,
  char *shared_file,
  CosSchedulingObjectMap *ObjectMap,
  CosSchedulingResourceMap *resourceMap)
: name_("RTCosScheduling_ServerScheduler_Interceptor"),
  orb_(orb),
  object_name_map_(ObjectMap),
  resource_map_(resourceMap)
{
  try
    {
      // Create a new Priority Ceiling protocol manager factory
      ACE_NEW_THROW_EX(this->PCP_factory_,
                       PCP_Manager_Factory(shared_file),
                       CORBA::NO_MEMORY());


      /// Now resolve a reference to the Real Time ORB
      CORBA::Object_var obj =
        this->orb_->resolve_initial_references("RTORB");
      RTCORBA::RTORB_var rt_orb;
      if (CORBA::is_nil(obj.in ()))
        {
          ACE_DEBUG((LM_DEBUG,
                     "RTORB not available, "
                     "RT ServerScheduler not used!\n"));
          return;
        }
      else
        {
          rt_orb =RTCORBA::RTORB::_narrow (obj.in ());
        }

      // Now get a reference to the RTCurrent
      // for the PCP manager to control
      obj = this->orb_->resolve_initial_references ("RTCurrent");
      if (CORBA::is_nil(obj.in()))
        {
          ACE_DEBUG((LM_DEBUG,
                     "RTCurrent not available, "
                     "RT ServerScheduler not used!\n"));
          return;
        }
      else
        {
          this->current_ =
            RTCORBA::Current::_narrow (obj.in ());
        }

      // Now get a reference to the codec factory to create a codec that will
      // decode the client priority sent in the service context
      obj =
        this->orb_->resolve_initial_references ("CodecFactory");

      if (CORBA::is_nil(obj.in()))
        {
          ACE_DEBUG((LM_DEBUG,
                     "Could not initalize the server interceptor Codec, "
                     "RT ServerScheduler not used!\n"));
          return;
        }
      else
        {
          this->codec_factory_ = IOP::CodecFactory::_narrow(obj.in());
        }


      // Set up the codec
      IOP::Encoding encoding;
      encoding.format = IOP::ENCODING_CDR_ENCAPS;
      encoding.major_version = 1;
      encoding.minor_version = 2;

      this->codec_ = this->codec_factory_->create_codec(encoding);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Error in installing the interceptor for the ServerScheduler\n");
    }
}


// Delete the objects we have made in the interceptor
RTCosScheduling_ServerScheduler_Interceptor::~RTCosScheduling_ServerScheduler_Interceptor()
{
  this->object_name_map_ = 0;
  this->resource_map_ = 0;
  delete this->PCP_factory_;
}

void
RTCosScheduling_ServerScheduler_Interceptor::receive_request(
  PortableInterceptor::ServerRequestInfo_ptr ri)
{
  try
    {

      ACE_CString name = "";
      COS_SCHEDULING_RESOURCE_VALUE ceiling, base_priority = 0;

      // Now get a reference to the POA, this is used to get a reference
      // to the target object
      PortableInterceptor::AdapterName *adapter_seq = ri->adapter_name();
      PortableServer::POA_var poa;
      const char *adapter_name =
        (*adapter_seq)[adapter_seq->length() - 1];

      CORBA::Object_var obj =
        this->orb_->resolve_initial_references("RootPOA");
      if (CORBA::is_nil(obj.in()))
        {
          ACE_DEBUG((LM_DEBUG,
                     "Could not get root POA, "
                     "RT scheduling not used on server!\n"));
          return;
        }
      else
        {
          PortableServer::POA_var root_poa =
            PortableServer::POA::_narrow(obj.in());
          if (CORBA::is_nil(root_poa.in()))
            {
              ACE_ERROR((LM_ERROR,
                         "No POA found in Interceptor prio not set\n"));
              return;
            }
          poa = root_poa->find_POA(adapter_name, 0);
        }

      // decode the Client priority sent in the service context
      IOP::ServiceId id = IOP::RTCorbaPriority;
      IOP::ServiceContext_var sc;
      try
        {
          sc = ri->get_request_service_context(id);
        }
      catch (const CORBA::Exception& ex)
        {
          // The RTCorbaPriority was not sent, do not use real time
          // Here we cannot let the server continue to run at
          // RTCORBA::maxPriority, so change to RTCORBA::minPriority
          // and things will run best effort
          this->current_->the_priority(RTCORBA::minPriority);
          return;
        }

      CORBA::OctetSeq ocSeq = CORBA::OctetSeq(
        sc->context_data.length(),
        sc->context_data.length(),
        sc->context_data.get_buffer(),
        0);

      CORBA::Any the_priority_as_any;
      the_priority_as_any = *this->codec_->decode(ocSeq);

      RTCORBA::Priority the_client_priority;
      the_priority_as_any >>= the_client_priority;

      // get the object from the object ID that is passed
      CORBA::OctetSeq_var oseq = ri->object_id();
      PortableServer::ObjectId oid(oseq -> length(),
                                   oseq -> length(),
                                   oseq -> get_buffer(),
                                    0);

      CORBA::Object_var target_object = poa->id_to_reference(oid);

      // Check to make sure we have the object as scheduled by the
      // ServerScheduler
      if (this->object_name_map_->find(target_object, name) == -1 )
        {
          ACE_DEBUG((LM_DEBUG,
                     "Object not found in config file, "
                     "RT ServerScheduler not used!\n"));
          // Here we cannot let the server continue to run at
          // RTCORBA:: maxPriority, so it will run at minPriority
          // and things will run best effort
          this->current_->the_priority(RTCORBA::minPriority);
          return;
        }

      /// If the object name if found, get its priority ceiling
      if (this->resource_map_->find(name, ceiling) == -1 )
        {
          /// We could not find the Object's priority ceiling
          /// given its name
          ACE_DEBUG((LM_DEBUG,
                     "Object Resource Ceiling not found in config file, "
                     "RT ServerScheduler not used!\n"));
          // Here we cannot let the server continue to run at
          // RTCORBA:: maxPriority, so it will run at minPriority
          // and things will run best effort
          this->current_->the_priority(RTCORBA::minPriority);
          return;
        }

      /// Get the base priority of the server
      if (this->resource_map_->find("BP", base_priority) == -1 )
        {
          ACE_DEBUG((LM_DEBUG,
                     "Server Base Priority not found in config file, "
                     "RTServerScheduler not used!\n"));
          // Here we cannot let the server continue to run at
          // RTCORBA:: maxPriority, so it will run at minPriority
          // and things will run best effort
          this->current_->the_priority(RTCORBA::minPriority);
          return;
        }

      /// Create a new PCP Manager to manage the priority control
      COS_SCHEDULING_INVOCATION_VALUE p = 0;
      ACE_NEW_THROW_EX(p,
                       PCP_Manager(
                         PCP_factory_->New_PCP_Manager(this->current_)),
                       CORBA::NO_MEMORY());

      // Get the lock on the resource, using MPCP

      p->lock(ceiling + base_priority,
              the_client_priority +  base_priority);

      /// store the thread in the invocation list
      this->invocation_map_.bind(
        p->threadID(),
        p);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unknown exception in the receive request\n");
      ACE_OS::exit(1);
    }
}

void
RTCosScheduling_ServerScheduler_Interceptor::send_reply(
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  finish_execution();
}

void
RTCosScheduling_ServerScheduler_Interceptor::finish_execution()
{
  PCP_Manager *p = 0;
  this->invocation_map_.unbind((ACE_OS::getpid() << 16) + int(ACE_Thread::self()),p);
  if (p)
    {
      /// Release the Lock on the Thread
      p->release_lock();
      /// Delete the PCP Manager
      delete p;
    }
}

char*
RTCosScheduling_ServerScheduler_Interceptor::name(void)
{
  return CORBA::string_dup(this->name_);
}

void
RTCosScheduling_ServerScheduler_Interceptor::destroy(void)
{
}

void
RTCosScheduling_ServerScheduler_Interceptor::receive_request_service_contexts(
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
}

void
RTCosScheduling_ServerScheduler_Interceptor::send_exception(
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  finish_execution();
}

void
RTCosScheduling_ServerScheduler_Interceptor::send_other(
    PortableInterceptor::ServerRequestInfo_ptr ri)
  {
    ACE_UNUSED_ARG(ri);
    finish_execution();
  }

#endif /* TAO_HAS_INTERCEPTORS */

int
RTCosScheduling_ServerScheduler_i::resources(
  const char  *node_name,
  const char  *file_name,
  CosSchedulingResourceMap *resource_map)
{
  /// get the resource list list just for the particular node
  const unsigned int BUF_MAX = 256;
  FILE *fp = ACE_OS::fopen(file_name, "r");
  if (fp == 0)
    {
      /// Error return of we cannot open the file.
      ACE_ERROR_RETURN((LM_ERROR,
                        "Could not find the config file %s, aborting\n",
                        file_name),
                        0);
    }


  char line[BUF_MAX], key[64];
  ACE_OS::strncpy(key, "Node ", sizeof("Node "));
  ACE_OS::strcat(key, node_name);

#ifndef ACE_LACKS_CLEARERR
  ACE_OS::clearerr(fp);
#else
# warning ACE_OS::clearerr() is unimplemented on this platform.
# warning This code may not function properly.
#endif  /* !ACE_LACKS_CLEARERR */
  do
    {
      ACE_OS::fgets(line, BUF_MAX, fp);
  if (ACE_OS::last_error() == EOF)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                            "Node %s not found in config file\n",
                        node_name),
                        0);
          break;
    }

    }
  while (ACE_OS::strncmp(line,key,ACE_OS::strlen(key)) != 0);

  /// Skip to the appropriate Task section of the node
#ifndef ACE_LACKS_CLEARERR
  ACE_OS::clearerr(fp);
#else
# warning ACE_OS::clearerr() is unimplemented on this platform.
# warning This code may not function properly.
#endif  /* !ACE_LACKS_CLEARERR */
  do
    {
      ACE_OS::fgets(line, BUF_MAX, fp);
  /// Make sure we did not hit the end of file
  if (ACE_OS::last_error() == EOF)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                            "Task list not found for node %s\n",
                        node_name),
                        0);
          break;
        }

    }
  while (ACE_OS::strncmp(line,
                         "Resources:",
                         ACE_OS::strlen("Resources:")
                        ) != 0);

  CORBA::Short done = 0;
  COS_SCHEDULING_RESOURCE_KEY name;
  COS_SCHEDULING_RESOURCE_VALUE priority = 0;
  u_int delimiter;

  /// read each activity/priority pair from the config file
  while (!done)
    {
      /// get the activity name
      ACE_OS::fgets(line, BUF_MAX, fp);

      /// check to make sure we have not reached the end of the list.
      if (ACE_OS::strncmp(line, "END", ACE_OS::strlen(line)-1) != 0)
        {
          name      = ACE_CString(line);
          delimiter = name.find('\t');
          char *p = ACE_OS::strchr(line, '\t');
          if (p)
              priority  = ACE_OS::atoi(p);
          if (priority == 0)
              priority = RTCORBA::minPriority;
          if (delimiter < name.length() && delimiter > 0)
            {
              resource_map->bind(name.substr(0, delimiter), priority);
            }
          else
            {
              ACE_ERROR_RETURN((LM_ERROR,
                                "Error in reading resources from %s, aborting",
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

}

TAO_END_VERSIONED_NAMESPACE_DECL
