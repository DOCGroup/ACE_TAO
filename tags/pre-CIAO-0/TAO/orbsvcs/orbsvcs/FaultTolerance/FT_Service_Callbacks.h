// $Id$
//
// ============================================================================
//
// = LIBRARY
//    FT
//
// = FILENAME
//    FT_Callbacks.h
//
// = DESCRIPTION
//   A concrete FT service callback implementation
//
// = AUTHOR
//   Bala Natarajan <bala@cs.wustl.edu>
// ============================================================================
#ifndef TAO_FT_CALLBACKS_H
#define TAO_FT_CALLBACKS_H
#include "ace/pre.h"

#include "fault_tol_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Service_Callbacks.h"
#include "orbsvcs/FT_CORBA_ORBC.h"

class TAO_Profile;
class TAO_MProfile;
class TAO_ORB_Core;

class TAO_FT_Export TAO_FT_Service_Callbacks : public TAO_Service_Callbacks
{
  // = TITLE
  //   A class to dynamically load the FT callback implementations in
  //   to the ORB.
  //
  // = DESCRIPTION
  //   An implementation of the service callbacks that allows the
  //   loading of the FT ORB level library in to the ORB
public:
  TAO_FT_Service_Callbacks (TAO_ORB_Core *orb_core);
  // Constructor

  virtual ~TAO_FT_Service_Callbacks (void);
  // Dtor

  /// @@todo: The following method has been deprecated. Will go after TAO
  /// 1.2.1.
  virtual CORBA::Boolean  select_profile (TAO_MProfile *mprofile,
                                          TAO_Profile *&pfile);
  // This method would search the list of <mprofile> to identify a
  // TAG_FT_PRIMARY and set that profile as <pfile>.

  /// @@todo: The following method has been deprecated. Will go after TAO
  /// 1.2.1.
  virtual CORBA::Boolean  reselect_profile (TAO_Stub* stub,
                                            TAO_Profile *&pfile);

  // This method would search the IOR list for the first non-primary
  // in case of primary failure.

  /// @@todo: The following method has been deprecated. Will go after TAO
  /// 1.2.1.
  virtual void reset_profile_flags (void);
  // Reset any of the local flags that we may have

  virtual CORBA::Boolean object_is_nil (CORBA::Object_ptr obj);
  // Check whether <obj> is nil or not. FT spec suggests some
  // extensions for a CORBA::is_nil () operation.

  // Need to do is_equivalent and hash also here

  virtual void service_context_list (TAO_Stub *&stub,
                                     IOP::ServiceContextList &service_list,
                                     CORBA::Boolean restart
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Add relevant stuff to the service context list

  virtual int raise_comm_failure (
      TAO_GIOP_Invocation *invoke,
      TAO_Profile *profile
      ACE_ENV_ARG_DECL);
  // Check whether we need to raise an exception or go for a
  // reinvocaton.

  virtual int raise_transient_failure (
      TAO_GIOP_Invocation *invoke,
      TAO_Profile *profile
      ACE_ENV_ARG_DECL);
  // Check whether we need to raise an exception or go for a
  // reinvocaton.

  virtual void service_log_msg_rcv (
      TAO_Message_State_Factory &state);
  // Log the message. The message state  is the message on the server
  // after it has been received in the GIOP layer

  virtual void service_log_msg_pre_upcall (
      TAO_ServerRequest &req);

  virtual void service_log_msg_post_upcall (
      TAO_ServerRequest &req);

private:

  void request_service_context (TAO_Stub *&stub,
                                IOP::ServiceContextList &service_list
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Makes  the  request service_context list

  void group_version_service_context (TAO_Stub *&stub,
                                      IOP::ServiceContextList &service_list
                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Make the group version service context list

  void get_object_group_version (TAO_Profile *profile);
  // Get the ObjectGroupRef version from the profile in use.

  CORBA::Boolean restart_policy_check (IOP::ServiceContextList &service_list,
                                       const TAO_Profile *profile);
  // Check whether the right flags are available so that we can issue a
  // restart.

private:
  TAO_ORB_Core *orb_core_;
  // The ORB core in which we have been activated

  ACE_Lock* profile_lock_;
  // Mutex to protect access to the profile that gets passed along

  CORBA::Boolean primary_failed_;
  // A flag that indicates that the primary has already failed. So any
  // more calls to select_profile () should not reset the profile
  // pointer to the primary.

  CORBA::Boolean secondary_set_;
  // A flag to indicate that a secondary has been selected for
  // invocation. We do this only once in the cycle. When we get there
  // again we dont select a primary again and again.

  FT::TagFTGroupTaggedComponent group_component_;
  // FT group component in the IOGR

  CORBA::Boolean group_component_flag_;
  // A flag to indicate whether we have extracted the
  // <group_component> before.
};

#if defined (__ACE_INLINE__)
# include "FT_Service_Callbacks.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_FT_CALLBACKS_H*/
