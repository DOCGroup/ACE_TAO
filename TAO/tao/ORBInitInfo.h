// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ORBInitInfo.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_ORB_INIT_INFO_H
#define TAO_ORB_INIT_INFO_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/StringSeqC.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


class TAO_ORB_Core;

/// This class encapsulates the data passed to ORBInitializers during
/// ORB initialization.
class TAO_Export TAO_ORBInitInfo :
  public virtual PortableInterceptor::ORBInitInfo,
  public virtual TAO_Local_RefCounted_Object
{
  friend CORBA::ORB_ptr CORBA::ORB_init (int &,
                                         char *argv[],
                                         const char *,
                                         CORBA_Environment &);

public:

  ~TAO_ORBInitInfo (void);
  ///< Destructor

  virtual CORBA::StringSeq * arguments (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * orb_id (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void register_initial_reference (
      const char * id,
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ORBInitInfo::InvalidName));

  virtual CORBA::Object_ptr resolve_initial_references (
      const char * id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ORBInitInfo::InvalidName));

  virtual void add_client_request_interceptor (
      PortableInterceptor::ClientRequestInterceptor_ptr interceptor,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ORBInitInfo::DuplicateName));

  virtual void add_server_request_interceptor (
      PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ORBInitInfo::DuplicateName));

  virtual void add_ior_interceptor (
      PortableInterceptor::IORInterceptor_ptr interceptor,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ORBInitInfo::DuplicateName));

  virtual PortableInterceptor::SlotId allocate_slot_id (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void register_policy_factory (
      CORBA::PolicyType type,
      PortableInterceptor::PolicyFactory_ptr policy_factory,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  TAO_ORBInitInfo (TAO_ORB_Core *orb_core,
                   int argc,
                   char *argv[]);
  ///< Only allow this class to be instantiated on the heap since it
  ///< is reference counted.

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  ACE_UNIMPLEMENTED_FUNC (
    TAO_ORBInitInfo (const TAO_ORBInitInfo &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_ORBInitInfo &))

private:

  TAO_ORB_Core *orb_core_;
  ///< Reference to the ORB Core.

  CORBA::StringSeq arguments_;
  ///< A copy of the argument vector (argv) passed to ORB_init() call,
  ///< but in the form of a string sequence. 

  // IOP_N::CodecFactory_var codec_factory_;
  ///< Reference to CodecFactory in the ORB.  Normally
  ///< ORB::resolve_initial_references("CodecFactory") is used to get
  ///< a reference to the CodecFactory, but the ORB has not been
  ///< initialized by the time the ORBInitializers are called.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_ORB_INIT_INFO_H */

