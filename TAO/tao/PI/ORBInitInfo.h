// -*- C++ -*-

// ===================================================================
/**
 *  @file   ORBInitInfo.h
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_ORB_INIT_INFO_H
#define TAO_ORB_INIT_INFO_H

#include /**/ "ace/pre.h"

#include "tao/PI/pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI_includeC.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
class TAO_ORBInitInfo;
typedef TAO_ORBInitInfo *TAO_ORBInitInfo_ptr;

typedef TAO_Objref_Var_T<TAO_ORBInitInfo>
                         TAO_ORBInitInfo_var;

typedef TAO_Objref_Out_T<TAO_ORBInitInfo>
                         TAO_ORBInitInfo_out;

/**
 * @class TAO_ORBInitInfo
 *
 * @brief An implementation of the PortableInterceptor::ORBInitInfo_3_1
 *        interface.
 *
 * This class encapsulates the data passed to ORBInitializers during
 * ORB initialization.
 */
class TAO_PI_Export TAO_ORBInitInfo
  : public virtual PortableInterceptor::ORBInitInfo_3_1,
    public virtual ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_ORBInitInfo (TAO_ORB_Core *orb_core,
                   int argc,
                   char *argv[],
                   PortableInterceptor::SlotId slotid);

  /**
   * @name PortableInterceptor::ORBInitInfo Methods
   *
   * These methods are exported by the
   * PortableInterceptor::ORBInitInfo interface.
   */
  //@{
  /// Return the argument vector for the ORB currently being
  /// initialized as a string sequence.
  virtual CORBA::StringSeq * arguments ();

  /// Return the ORBid for the ORB currently being initialized.
  virtual char * orb_id ();

  /// Return the CodecFactory for the ORB currently being
  /// initialized.
  virtual IOP::CodecFactory_ptr codec_factory ();

  /// Register a mapping between a string and a corresponding object
  /// reference with the ORB being initialized.
  /**
   * This method is particularly useful for registering references to
   * local (locality constrained) objects.  Note that this method
   * should be called in ORBInitializer::pre_init() so that the
   * registered reference will be available to the
   * resolve_initial_references() that may be called in the
   * ORBInitializer::post_init() call.
   */
  virtual void register_initial_reference (const char * id, CORBA::Object_ptr obj);

  /// Obtain a reference to an object that may not yet be available
  /// via the usual CORBA::ORB::resolve_initial_references() mechanism
  /// since the ORB may not be fully initialized yet.
  virtual CORBA::Object_ptr resolve_initial_references (const char * id);

  /// Register a client request interceptor with the ORB currently
  /// being initialized.
  virtual void add_client_request_interceptor (
      PortableInterceptor::ClientRequestInterceptor_ptr interceptor);

  /// Register a server request interceptor with the ORB currently
  /// being initialized.
  virtual void add_server_request_interceptor (
      PortableInterceptor::ServerRequestInterceptor_ptr interceptor);

  /// Register an IOR interceptor with the ORB currently being
  /// initialized.
  virtual void add_ior_interceptor (
      PortableInterceptor::IORInterceptor_ptr interceptor);

  /// Register a client request interceptor with the ORB currently
  /// being initialized, along with a list of policies.
  virtual void add_client_request_interceptor_with_policy (
      PortableInterceptor::ClientRequestInterceptor_ptr interceptor,
      const CORBA::PolicyList& policies);

  /// Register a server request interceptor with the ORB currently
  /// being initialized, along with a list of policies.
  virtual void add_server_request_interceptor_with_policy (
      PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
      const CORBA::PolicyList& policies);

  /// Register an IOR interceptor with the ORB currently being
  /// initialized, along with a list of policies.
  virtual void add_ior_interceptor_with_policy (
      PortableInterceptor::IORInterceptor_ptr interceptor,
      const CORBA::PolicyList& policies);

  /// Reserve a slot in table found within the
  /// PortableInterceptor::Current object.
  virtual PortableInterceptor::SlotId allocate_slot_id ();

  /// Register a policy factory of the given policy type with the ORB
  /// currently being initialized.
  virtual void register_policy_factory (
      CORBA::PolicyType type,
      PortableInterceptor::PolicyFactory_ptr policy_factory);

  virtual CORBA::ORB_ptr _get_orb ();
  //@}

  /**
   * @name TAO Extensions
   *
   * These methods are not part of the PortableInterceptor
   * specification, and are TAO-specific extensions.
   */
  //@{
  /// Allocate a slot in the ORB's TSS resources.
  /**
   * TAO uses a single TSS key for these resources, so it is useful to
   * place TSS objects in TAO's TSS resources on platforms where the
   * number of TSS keys is low.  The returned SlotId can be used to
   * index into the array stored in ORB's TSS resources structure.
   * @par
   * An accompanying cleanup function (e.g. a TSS destructor) can also
   * be registered.
   */
  size_t allocate_tss_slot_id (ACE_CLEANUP_FUNC cleanup);

  /// Return a pointer to the ORB Core associated with the ORB being
  /// initialized.
  /**
   * The ORB Core is essentialy fully initialized by the time
   * ORBInitializer::post_init() is invoked.  As such, it is generally
   * best if this method is used in that method.
   *
   * @note Only use this method if you know what you are doing.
   */
  TAO_ORB_Core *orb_core () const;

  /// Invalidate this ORBInitInfo instance.
  /**
   * @note This method is only meant to be called by the
   *       CORBA::ORB_init() function.
   */
  void invalidate ();

  /// Return the number of allocated slots in for the PICurrent
  /// object.
  PortableInterceptor::SlotId slot_count () const;
  //@}

  /**
   * @name Reference Related Methods
   *
   * TAO_ORBInitInfo-specific methods and types.
   */
  //@{
  typedef TAO_ORBInitInfo_ptr _ptr_type;
  typedef TAO_ORBInitInfo_var _var_type;

  static TAO_ORBInitInfo_ptr _duplicate (TAO_ORBInitInfo_ptr obj);

  static TAO_ORBInitInfo_ptr _narrow (CORBA::Object_ptr obj);


  static TAO_ORBInitInfo_ptr _unchecked_narrow (CORBA::Object_ptr obj);


  static TAO_ORBInitInfo_ptr _nil ()
    {
      return nullptr;
    }
  //@}

  virtual const char* _interface_repository_id () const;

protected:
  /// Destructor is protected to enforce proper memory management
  /// through the reference counting mechanism.
  ~TAO_ORBInitInfo ();

  /// Check if this ORBInitInfo instance is valid.  Once post_init()
  /// has been called on each of the ORBInitializers, this ORBInitInfo
  /// is no longer valid.  Throw an exception in that case.
  void check_validity ();

private:
  TAO_ORBInitInfo (const TAO_ORBInitInfo &) = delete;
  void operator= (const TAO_ORBInitInfo &) = delete;

private:
  /// Reference to the ORB Core.
  TAO_ORB_Core *orb_core_;

  /// The number of arguments in the argument vector passed to
  /// CORBA::ORB_init().
  int argc_;

  /// The argument vector passed to CORBA::ORB_init().
  char **argv_;

  /// Reference to the CodecFactory returned by
  /// ORBInitInfo::codec_factory().
  IOP::CodecFactory_var codec_factory_;

  /// The number of allocated slots.
  PortableInterceptor::SlotId slot_count_;
};

// Traits specializations.
namespace TAO
{
  // Hand crafted. Not forward declared, but used by PortableServer.
  template<>
  struct TAO_PI_Export Objref_Traits<TAO_ORBInitInfo>
  {
    static TAO_ORBInitInfo_ptr duplicate (TAO_ORBInitInfo_ptr);
    static void release (TAO_ORBInitInfo_ptr);
    static TAO_ORBInitInfo_ptr nil ();
    static CORBA::Boolean marshal (TAO_ORBInitInfo_ptr p, TAO_OutputCDR & cdr);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PI/ORBInitInfo.inl"
#endif  /* __ACE_INLINE__ */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_ORB_INIT_INFO_H */
