// -*- C++ -*-

//=============================================================================
/**
 *  @file    RTCosScheduling_ServerScheduler_i.h
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

#ifndef SERVER_SCHEDULERI_H
#define SERVER_SCHEDULERI_H

#include /**/ "ace/pre.h"

#include "ace/Map_T.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include <orbsvcs/RTCosSchedulingS.h>
#include <orbsvcs/RTCosScheduling/RTCosScheduling_export.h>
#include "orbsvcs/RTCosScheduling/RTCosScheduling_PCP_Manager.h"
#include "tao/PortableInterceptorC.h"
#include "tao/CodecFactory/CodecFactory.h"
#include "tao/LocalObject.h"



#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#ifndef RTCOSSCHEDULING_SERVER_CONTAINERS
#define RTCOSSCHEDULING_SERVER_CONTAINERS

/**
 * CosSchedulingObjectNameMap
 *
 * This typedef is used by the RTCORBA 1.0 ServerScheduler to map
 * CORBA object references to names
 */

typedef CORBA::Object_var COS_SCHEDULING_OBJECT_NAME_KEY;
typedef ACE_CString COS_SCHEDULING_OBJECT_NAME_VALUE;

template<>
class TAO_RTCosScheduling_Export ACE_Equal_To< COS_SCHEDULING_OBJECT_NAME_KEY >
{
  public:
  int operator () (const COS_SCHEDULING_OBJECT_NAME_KEY lhs,
                   const COS_SCHEDULING_OBJECT_NAME_KEY rhs) const;
};


namespace TAO {

class TAO_RTCosScheduling_Export CosSchedulingObjectMap_Hash_Key
{
public:
  u_long operator () (const COS_SCHEDULING_OBJECT_NAME_KEY &key) const;
};

typedef ACE_Hash_Map_Manager_Ex_Adapter<
  COS_SCHEDULING_OBJECT_NAME_KEY,
  COS_SCHEDULING_OBJECT_NAME_VALUE,
  CosSchedulingObjectMap_Hash_Key,
  ACE_Equal_To<COS_SCHEDULING_OBJECT_NAME_KEY>,
  ACE_Noop_Key_Generator<COS_SCHEDULING_OBJECT_NAME_KEY> > CosSchedulingObjectMap;

/**
 * This typedef is used in the ServerScheduler to hold a list of
 * priority ceiling protocol managers for each active method call
 * from a client.
 */
typedef int COS_SCHEDULING_INVOCATION_KEY;
typedef PCP_Manager * COS_SCHEDULING_INVOCATION_VALUE;

class CosSchedulingInvocation_Hash_key
{
public:
  u_long operator () (const COS_SCHEDULING_INVOCATION_KEY &key) const;
};

typedef ACE_Hash_Map_Manager_Ex_Adapter<
  COS_SCHEDULING_INVOCATION_KEY,
  COS_SCHEDULING_INVOCATION_VALUE,
  CosSchedulingInvocation_Hash_key,
  ACE_Equal_To<COS_SCHEDULING_INVOCATION_KEY>,
  ACE_Noop_Key_Generator<COS_SCHEDULING_INVOCATION_KEY> > CosSchedulingInvocationMap;


#endif /* RTCOSSCHEDULING_SERVER_CONTAINERS */

#if TAO_HAS_INTERCEPTORS
  /**
   * @class RTCosScheduling_ServerScheduler_Interceptor
   *
   * @brief  The RTCosScheduling_ServerScheduler_Interceptor intercepts CORBA
   * requests on behalf of the RTCORBA 1.0 scheduling service and
   * schedules the requests.
   */
  class RTCosScheduling_ServerScheduler_Interceptor
  : public PortableInterceptor::ServerRequestInterceptor
    {
  public :
    RTCosScheduling_ServerScheduler_Interceptor(CORBA::ORB_ptr orb,
      char *shared_file,
      CosSchedulingObjectMap *CosSchedulingObjectMap,
      CosSchedulingResourceMap *resourceMap);


    virtual ~RTCosScheduling_ServerScheduler_Interceptor();

    virtual char* name(void);

    virtual void destroy();

    virtual void receive_request(
        PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void receive_request_service_contexts(
        PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void send_reply(
        PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void send_exception(
        PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void send_other(
        PortableInterceptor::ServerRequestInfo_ptr ri);

    void finish_execution();


  private:

    /// The name of the interceptor
    const char* name_;

    /// reference to set local priority
    RTCORBA::Current_var current_;

    /// Factory reference to receive PCP_managers
    PCP_Manager_Factory * PCP_factory_;

    /// Map to match threadIDs with PCP_Managers
    CosSchedulingInvocationMap invocation_map_;

    //The orb
    CORBA::ORB_ptr orb_;

    /// Map to match CORBA::Object_vars to names
    CosSchedulingObjectMap *object_name_map_;

    /// Map to match Resource names to Ceilings
    CosSchedulingResourceMap *resource_map_;

    // The codec to decode incoming CORBA Priorities
    IOP::Codec_var codec_;


    // The factory to create the codec
    IOP::CodecFactory_var codec_factory_;
  };

#endif /* TAO_HAS_INTERCEPTORS */

  /**
   * @class RTCosScheduling_ServerScheduler_i
   *
   * @brief The ServerScheduler class handles server side
   * scheduling for the RTCORBA 1.0 Scheduling Service.
   */
  class TAO_RTCosScheduling_Export RTCosScheduling_ServerScheduler_i
  : public virtual RTCosScheduling::ServerScheduler,
    public virtual CORBA::LocalObject
  {
  public:
    // Constructor
    RTCosScheduling_ServerScheduler_i (char *node,
      char *file,
      char *shared_file,
      int numthreads);

    //Destructor
    virtual ~RTCosScheduling_ServerScheduler_i (void);

    /**
     * This creates and returns a RT POA for use on the server.
     * It accepts a set of non-RT policies and sets these as
     * well as RT policies (threadpooling and Server Declared).
     *
     * @param parent The poa to base the RT POA upon
     * @param adapter_name the adapter name for the poa
     * @param a_POAManager the poa_manager
     * @param policies the list of non real time policies to be set
     */
    virtual ::PortableServer::POA_ptr create_POA (
        PortableServer::POA_ptr parent,
        const char * adapter_name,
        PortableServer::POAManager_ptr a_POAManager,
        const CORBA::PolicyList & policies);

    /**
     * This maps a CORBA::Object_var with a name.  The names are
     * later associated with priority ceiling stored in the
     * scheduling config file.
     *
     * @param obj the CORBA object reference to associate with a name
     * @param name name to associate with the CORBA object reference
     */
    virtual void schedule_object (CORBA::Object_ptr obj,
        const char * name);

  private:

    /**
     * This sets the orb for the ServerScheduler,
     * it also resolves references to RT current and sets
     * priority mapping
     */
    int configure_ORB(TAO_ORB_Core *orb_core);

    /// RT Current, to change the priority of the thread

    /// RT Corba Priority Mapping , uses
    /// Linear Priority Mapping
    RTCORBA::PriorityMapping *pm_;

    /// The number of threads in the server threadpool
    int num_threads_;

    char *shared_file_;

    /// The map to match CORBA::Object_vars
    /// to names
    CosSchedulingObjectMap object_name_map_;

    /// The map to match resource names to corba priorities
    CosSchedulingResourceMap resource_map_;

    /*
     * resources populates a string/int map with a list of resources (keys)
     * and associated priority ceilings(values)
     *
     * @param node_name the name of the node the client resides on,
     * resources() only retrieves ceilings for the local node
     *
     * @param map resource_map a reference to the resource map to populate
     *
     */
    int resources(const char* node_name,
                  const char* file_name,
                  CosSchedulingResourceMap * resource_map);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/RTCosScheduling/RTCosScheduling_ServerScheduler_i.inl"
#endif /* __ACE_INLINE__ */


#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* SERVER_SCHEDULERI_H  */
