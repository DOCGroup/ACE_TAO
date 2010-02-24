// -*- C++ -*-

//=============================================================================
/**
 *  @file    RTCosScheduling_ClientScheduler_i.h
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

#ifndef CLIENT_SCHEDULER_I_H
#define CLIENT_SCHEDULER_I_H

#include /**/ "ace/pre.h"

#include "ace/Map_T.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "ace/SString.h"
#include <orbsvcs/RTCosSchedulingC.h>
#include <orbsvcs/RTCosScheduling/RTCosScheduling_export.h>
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/PortableInterceptorC.h"
#include "tao/CodecFactory/CodecFactory.h"
#include "tao/RTCORBA/Linear_Priority_Mapping.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO {

#ifndef COS_SCHEDULER_ACTIVITY_MAP
#define COS_SCHEDULER_ACTIVITY_MAP
  /**
   * This typedef is used in RTCosScheduling_ClientScheduler
   * to map activity names,stored as an ACE_CString,
   * to CORBA priorities
   */
  typedef ACE_CString COS_SCHEDULER_ACTIVITY_KEY;
  typedef CORBA::Long COS_SCHEDULER_ACTIVITY_VALUE;

  typedef ACE_Map_Manager_Adapter<
    COS_SCHEDULER_ACTIVITY_KEY,
    COS_SCHEDULER_ACTIVITY_VALUE,
    ACE_Noop_Key_Generator<COS_SCHEDULER_ACTIVITY_KEY> >
      CosSchedulerActivityMap;
#endif /* ACTIVITY_MAP */

#if TAO_HAS_INTERCEPTORS
/**
 * @class ClientRequestInterceptor
 *
 * @brief Simple concrete client request interceptor.
 */
class RTCosScheduling_ClientScheduler_Interceptor
  : public PortableInterceptor::ClientRequestInterceptor
{
public:

    RTCosScheduling_ClientScheduler_Interceptor();
    RTCosScheduling_ClientScheduler_Interceptor(const CORBA::ORB_var orb);

    virtual ~RTCosScheduling_ClientScheduler_Interceptor() { }
  /**
   * @name Methods Required by the Client Request Interceptor
   * Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all client request interceptors.
   */
  //@{
  /// Return the name of this ClientRequestInterceptor.
  //
  virtual char * name (void);

  virtual void destroy (void);

  virtual void send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void send_poll (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (
      PortableInterceptor::ClientRequestInfo_ptr ri);
  //@}

private:

  /// The name of the interceptor
  const char* name_;

  /// reference to set local priority
  RTCORBA::Current_var current_;

  IOP::Codec_var codec_;

};
#endif /* TAO_HAS_INTERCEPTORS */

/**
 * @class RTCosScheduling_ClientScheduler
 *
 * @brief Used in conjunction with the class ServerScheduler to
 * provide RTCORBA 1.0 compliant scheduling
 *
 * This class provides the framework necessary for a client node
 * to retrieve scheduling information from a config file and set
 * the local OS priority.
 *
 */
class TAO_RTCosScheduling_Export RTCosScheduling_ClientScheduler_i :
  public virtual RTCosScheduling::ClientScheduler,
  public virtual CORBA::LocalObject
{

  public:
//@{
    /*
     * Constructs a new ClientScheduler object for use on a client
     * that wishes to use the RTCORBA 1.0 Scheduling Service.
     *
     * @param orb The orb
     * @param node The name of the node the client resides on
     */
    RTCosScheduling_ClientScheduler_i (CORBA::ORB_var orb,
                                       char* node,
                                       char* file);

    ///Destructor
    virtual ~RTCosScheduling_ClientScheduler_i (void);


    /**
     * Called by clients to set the local thread priority to
     * that specified in the scheduling config file
     *
     * @param activity_name the name of the activity that signifies
     *        the desired priority
     */
    virtual void schedule_activity (
        const char *activity_name);
//@}

  private:
    /// The map to match activity names with corba priorities
    CosSchedulerActivityMap activity_map_;

    /// RT Current, to change the priority of the thread
    RTCORBA::Current_var current_;

    /// RT Corba Priority Mapping
    RTCORBA::PriorityMapping *pm_;

#if TAO_HAS_INTERCEPTORS
    /// The Server Interceptor that handles the PCP control
    RTCosScheduling_ClientScheduler_Interceptor *client_interceptor_;
#endif /* TAO_HAS_INTERCEPTORS */

    int tasks(const char  *node_name,
              const char  *file_name,
              CosSchedulerActivityMap *activity_map);

};

}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif  /* CLIENT_SCHEDULER_I */
