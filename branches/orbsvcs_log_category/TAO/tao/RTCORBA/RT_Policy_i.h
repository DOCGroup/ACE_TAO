/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   RT_Policy_i.h
 *
 *  $Id$
 *
 *  @author Marina Spivak (marina@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_RT_POLICY_I_H
#define TAO_RT_POLICY_I_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTCORBA/RTCORBA_includeC.h"
#include "tao/LocalObject.h"
#include "tao/Basic_Types.h"

#include "ace/SString.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_PriorityModelPolicy
 *
 * @brief RTCORBA::PriorityModelPolicy implementation
 *
 * This policy controls how the priority at which a server handles
 * requests from clients is determined.
 */
class TAO_RTCORBA_Export TAO_PriorityModelPolicy
  : public RTCORBA::PriorityModelPolicy,
    public ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_PriorityModelPolicy (RTCORBA::PriorityModel priority_model,
                           RTCORBA::Priority server_priority);

  /// Copy constructor.
  TAO_PriorityModelPolicy (const TAO_PriorityModelPolicy &rhs);

  /**
   * Accessor for the <priority_model_> attribute.  This method is
   * used internally by the orb.  This is a more efficient
   * (non-virtual, no exception handling) relative of the idl interface
   * implementation <priority_model> below.
   */
  RTCORBA::PriorityModel get_priority_model (void);

  RTCORBA::PriorityModel priority_model (void);

  RTCORBA::Priority server_priority (void);

  CORBA::PolicyType policy_type (void);

  CORBA::Policy_ptr copy (void);

  void destroy (void);

  /// This method writes a CDR representation of the object state.
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /// This method reads the object state from a CDR representation.
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

  // Return the cached policy type for this policy.
  TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  TAO_Policy_Scope _tao_scope (void) const;

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any &val);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_PriorityModelPolicy (void);

  /// This constructor is used by TAO_RT_PolicyFactory when decoding
  /// policies from tagged components in an IOR.
  friend class TAO_RT_PolicyFactory;
  TAO_PriorityModelPolicy (void);

private:

  /// Attributes.
  RTCORBA::PriorityModel priority_model_;
  RTCORBA::Priority server_priority_;
};

//*************************************************************************

/**
 * @class TAO_ThreadpoolPolicy
 *
 * @brief RTCORBA::ThreadpoolPolicy implementation
 *
 * This policy controls which threadpool is associated with a POA.
 */
class TAO_RTCORBA_Export TAO_ThreadpoolPolicy
  : public RTCORBA::ThreadpoolPolicy,
    public ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_ThreadpoolPolicy (RTCORBA::ThreadpoolId id);

  /// Copy constructor.
  TAO_ThreadpoolPolicy (const TAO_ThreadpoolPolicy &rhs);

  RTCORBA::ThreadpoolId threadpool (void);

  CORBA::PolicyType policy_type (void);

  CORBA::Policy_ptr copy (void);

  void destroy (void);

  // Return the cached policy type for this policy.
  TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  TAO_Policy_Scope _tao_scope (void) const;

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any &val);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_ThreadpoolPolicy (void);

private:

  /// Attribute.
  RTCORBA::ThreadpoolId id_;
};

//*************************************************************************

/**
 * @class TAO_PrivateConnectionPolicy
 *
 * @brief RTCORBA::PrivateConnectionPolicy implementation
 *
 * This policy controls whether the transport connection for the
 * Object is private, i.e., not available for carrying out invocations on
 * other objects.
 */
class TAO_RTCORBA_Export TAO_PrivateConnectionPolicy
  : public RTCORBA::PrivateConnectionPolicy,
    public ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_PrivateConnectionPolicy (void);

  /// Copy constructor.
  TAO_PrivateConnectionPolicy (const TAO_PrivateConnectionPolicy &rhs);

  CORBA::PolicyType policy_type (void);

  CORBA::Policy_ptr copy (void);

  void destroy (void);

  // Return the cached policy type for this policy.
  TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  TAO_Policy_Scope _tao_scope (void) const;

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any &val);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_PrivateConnectionPolicy (void);

};

//*************************************************************************

/**
 * @class TAO_PriorityBandedConnectionPolicy
 *
 * @brief RTCORBA::PriorityBandedConnectionPolicy implementation
 *
 * RT CORBA provides facility for a client to communicate with a
 * server via multiple connections, aka bands, with each connection handling
 * invocations that are made at a different CORBA priority.
 * This policy allows the specification of the number of such
 * bands and their priorities.
 */
class TAO_RTCORBA_Export TAO_PriorityBandedConnectionPolicy
  : public RTCORBA::PriorityBandedConnectionPolicy,
    public ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_PriorityBandedConnectionPolicy (const RTCORBA::PriorityBands &bands);

  /// Copy constructor.
  TAO_PriorityBandedConnectionPolicy (const TAO_PriorityBandedConnectionPolicy &rhs);

  /**
   * Accessor to the underlying priority bands of the policy (does not
   * make a copy like the idl accessor <priority_bands> implementation
   * below.)
   */
  RTCORBA::PriorityBands &priority_bands_rep (void);

  RTCORBA::PriorityBands *priority_bands (void);

  CORBA::PolicyType policy_type (void);

  CORBA::Policy_ptr copy (void);

  void destroy (void);

  /// This method writes a CDR representation of the current object.
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /// This method reads the object state from a CDR representation.
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

  // Return the cached policy type for this policy.
  TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  TAO_Policy_Scope _tao_scope (void) const;

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any &val);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_PriorityBandedConnectionPolicy (void);

  /// This constructor is used by TAO_RT_PolicyFactory when decoding
  /// policies from tagged components in an IOR.
  friend class TAO_RT_PolicyFactory;
  TAO_PriorityBandedConnectionPolicy (void);

private:

  /// Attribute.
  RTCORBA::PriorityBands priority_bands_;
};

//*************************************************************************

/**
 * @class TAO_ServerProtocolPolicy
 *
 * @brief RTCORBA::ServerProtocolPolicy implementation
 *
 * This policy controls selection and configuration of
 * communication protocols on the server-side of the RT ORB.
 */
class TAO_RTCORBA_Export TAO_ServerProtocolPolicy
  : public RTCORBA::ServerProtocolPolicy,
    public ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_ServerProtocolPolicy (const RTCORBA::ProtocolList &protocols);

  /// Copy constructor.
  TAO_ServerProtocolPolicy (const TAO_ServerProtocolPolicy &rhs);

  /**
   * Accessor to the underlying protocols list of the policy (does not
   * make a copy like the idl accessor <protocols> implementation
   * below.)
   */
  RTCORBA::ProtocolList & protocols_rep (void);

  RTCORBA::ProtocolList *protocols (void);

  CORBA::PolicyType policy_type (void);

  CORBA::Policy_ptr copy (void);

  void destroy (void);

  // Return the cached policy type for this policy.
  TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  TAO_Policy_Scope _tao_scope (void) const;

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any &val);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_ServerProtocolPolicy (void);

private:

  /// Attribute.
  RTCORBA::ProtocolList protocols_;
};

//*************************************************************************

/**
 * @class TAO_ClientProtocolPolicy
 *
 * @brief RTCORBA::ClientProtocolPolicy implementation
 *
 * This policy controls selection and configuration of
 * communication protocols on the client-side of the RT ORB.
 */
class TAO_RTCORBA_Export TAO_ClientProtocolPolicy
  : public RTCORBA::ClientProtocolPolicy,
    public ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_ClientProtocolPolicy (const RTCORBA::ProtocolList &protocols);

  /// Copy constructor.
  TAO_ClientProtocolPolicy (const TAO_ClientProtocolPolicy &rhs);

  /**
   * Accessor to the underlying protocols list of the policy (does not
   * make a copy like the idl accessor <protocols> implementation
   * below.)
   */
  RTCORBA::ProtocolList & protocols_rep (void);

  RTCORBA::ProtocolList *protocols (void);

  CORBA::PolicyType policy_type (void);

  CORBA::Policy_ptr copy (void);

  void destroy (void);

  // = CDR Encoder/Decoder.

  /// This method writes a CDR representation of the current object.
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /// This method reads the object state from a CDR representation.
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

  // Return the cached policy type for this policy.
  TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  TAO_Policy_Scope _tao_scope (void) const;

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any &val);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_ClientProtocolPolicy (void);

  /// This constructor is used by TAO_RT_PolicyFactory when decoding
  /// policies from tagged components in an IOR.
  friend class TAO_RT_PolicyFactory;
  TAO_ClientProtocolPolicy (void);

private:

  /// Attribute.
  RTCORBA::ProtocolList protocols_;
};

//*************************************************************************

/**
 * @class TAO_TCP_Protocol_Properties
 *
 * @brief RTCORBA::TCPProtocolProperties implementation
 *
 * Stores TCP Protocol configuration properties.
 */
class TAO_RTCORBA_Export TAO_TCP_Protocol_Properties
  : public RTCORBA::TCPProtocolProperties,
    public ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_TCP_Protocol_Properties (CORBA::Long send_buffer_size,
                               CORBA::Long recv_buffer_size,
                               CORBA::Boolean keep_alive,
                               CORBA::Boolean dont_route,
                               CORBA::Boolean no_delay,
                               CORBA::Boolean enable_network_priority);

  CORBA::Long send_buffer_size (void);

  void send_buffer_size (CORBA::Long send_buffer_size);

  CORBA::Long recv_buffer_size (void);

  void recv_buffer_size (CORBA::Long recv_buffer_size);

  CORBA::Boolean keep_alive (void);

  void keep_alive (CORBA::Boolean keep_alive);

  CORBA::Boolean dont_route (void);

  void dont_route (CORBA::Boolean dont_route);

  CORBA::Boolean no_delay (void);

  void no_delay (CORBA::Boolean no_delay);

  CORBA::Boolean enable_network_priority (void);

  void enable_network_priority (CORBA::Boolean enable);

  /// This method writes a CDR representation of TCPProtocolProperties.
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /// This method reads the object state from a CDR representation.
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_TCP_Protocol_Properties (void);

private:
  // = Attributes.

  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
  CORBA::Boolean keep_alive_;
  CORBA::Boolean dont_route_;
  CORBA::Boolean no_delay_;
  CORBA::Boolean enable_network_priority_;
};

//*************************************************************************

/**
 * @class TAO_UnixDomain_Protocol_Properties
 *
 * @brief RTCORBA::UnixDomainProtocolProperties implementation.
 *
 * Stores Unix Domain Sockets (Local IPC) Protocol configuration
 * properties.
 */
class TAO_RTCORBA_Export TAO_UnixDomain_Protocol_Properties
  : public RTCORBA::UnixDomainProtocolProperties,
    public ::CORBA::LocalObject
{
public:

  /// Constructor
  TAO_UnixDomain_Protocol_Properties (CORBA::Long send_buffer_size,
                                      CORBA::Long recv_buffer_size);

  CORBA::Long send_buffer_size (void);

  void send_buffer_size (CORBA::Long send_buffer_size);

  CORBA::Long recv_buffer_size (void);

  void recv_buffer_size (CORBA::Long recv_buffer_size);

  /**
   * This method writes the CDR encapsulation of an instance of
   * UnixDomainProperties. This Protocol Property in TAO specific,
   * so there is no order of encapsulation specified in the
   * RT CORBA Spec. The current implementation encodes the field
   * according to the order of declaration (i.e. first is encoded
   * send_buffer_size and then recv_buffer_size).
   */
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /**
   * This method reads an instance of UnixDomainProperties from
   * a CDR encapsulation. This Protocol Property in TAO specific,
   * so there is no order of encapsulation specified in the
   * RT CORBA Spec. The current implementation expect the field
   * according to the order of declaration.
   */
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_UnixDomain_Protocol_Properties (void);

private:

  // = Attributes.
  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
};

//*************************************************************************

/**
 * @class TAO_SharedMemory_Protocol_Properties
 *
 * @brief RTCORBA::SharedMemoryProtocolProperties implementation.
 *
 * Stores Shared Memory Protocol configuration
 * properties.
 */
class TAO_RTCORBA_Export TAO_SharedMemory_Protocol_Properties
  : public RTCORBA::SharedMemoryProtocolProperties,
    public ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_SharedMemory_Protocol_Properties (CORBA::Long send_buffer_size,
                                        CORBA::Long recv_buffer_size,
                                        CORBA::Boolean keep_alive,
                                        CORBA::Boolean dont_route,
                                        CORBA::Boolean no_delay,
                                        CORBA::Long preallocate_buffer_size,
                                        const char *mmap_filename,
                                        const char *mmap_lockname);

  // = IDL interface methods.

  CORBA::Long send_buffer_size (void);

  void send_buffer_size (CORBA::Long send_buffer_size);

  CORBA::Long recv_buffer_size (void);

  void recv_buffer_size (CORBA::Long recv_buffer_size);

  CORBA::Boolean keep_alive (void);

  void keep_alive (CORBA::Boolean keep_alive);

  CORBA::Boolean dont_route (void);

  void dont_route (CORBA::Boolean dont_route);

  CORBA::Boolean no_delay (void);

  void no_delay (CORBA::Boolean no_delay);

  CORBA::Boolean enable_network_priority (void);

  CORBA::Long preallocate_buffer_size (void);

  void preallocate_buffer_size (CORBA::Long preallocate_buffer_size);

  char *mmap_filename (void);

  void mmap_filename (const char *mmap_filename);

  char *mmap_lockname (void);

  void mmap_lockname (const char *mmap_lockname);

  /**
   * This method writes the CDR encapsulation of an instance of
   * SharedMemoryProperties. This Protocol Property in TAO specific,
   * so there is no order of encapsulation specified in the
   * RT CORBA Spec. The current implementation encodes the field
   * according to the order of declaration.
   */
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /**
   * This method reads an instance of SharedMemoryProperties from
   * a CDR encapsulation. This Protocol Property in TAO specific,
   * so there is no order of encapsulation specified in the
   * RT CORBA Spec. The current implementation expect the field
   * according to the order of declaration.
   */
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_SharedMemory_Protocol_Properties (void);

private:

  // = Attributes.
  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
  CORBA::Boolean keep_alive_;
  CORBA::Boolean dont_route_;
  CORBA::Boolean no_delay_;
  CORBA::Long preallocate_buffer_size_;
  ACE_CString mmap_filename_;
  ACE_CString mmap_lockname_;
};

//*************************************************************************

/**
 * @class TAO_UserDatagram_Protocol_Properties
 *
 * @brief RTCORBA::UserDatagramProtocolProperties implementation
 *
 * Stores UserDatagram Protocol configuration properties.
 */
class TAO_RTCORBA_Export TAO_UserDatagram_Protocol_Properties
  : public RTCORBA::UserDatagramProtocolProperties,
    public ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_UserDatagram_Protocol_Properties (CORBA::Long send_buffer_size,
                                        CORBA::Long recv_buffer_size,
                                        CORBA::Boolean enable_network_priority);

  CORBA::Boolean enable_network_priority (void);

  void enable_network_priority (CORBA::Boolean enable);

  CORBA::Long send_buffer_size (void);

  void send_buffer_size (CORBA::Long send_buffer_size);

  CORBA::Long recv_buffer_size (void);

  void recv_buffer_size (CORBA::Long recv_buffer_size);

  /// This method writes a CDR representation of UserDatagramProtocolProperties.
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /// This method reads the object state from a CDR representation.
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_UserDatagram_Protocol_Properties (void);

private:
  // = Attributes.
  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;

  CORBA::Boolean enable_network_priority_;
};

//*************************************************************************

/**
 * @class TAO_StreamControl_Protocol_Properties
 *
 * @brief RTCORBA::StreamControlProtocolProperties implementation
 *
 * Stores StreamControl Protocol configuration properties.
 */
class TAO_RTCORBA_Export TAO_StreamControl_Protocol_Properties
  : public RTCORBA::StreamControlProtocolProperties,
    public ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_StreamControl_Protocol_Properties (CORBA::Long send_buffer_size,
                                         CORBA::Long recv_buffer_size,
                                         CORBA::Boolean keep_alive,
                                         CORBA::Boolean dont_route,
                                         CORBA::Boolean no_delay,
                                         CORBA::Boolean enable_network_priority);

  CORBA::Long send_buffer_size (void);

  void send_buffer_size (CORBA::Long send_buffer_size);

  CORBA::Long recv_buffer_size (void);

  void recv_buffer_size (CORBA::Long recv_buffer_size);

  CORBA::Boolean keep_alive (void);

  void keep_alive (CORBA::Boolean keep_alive);

  CORBA::Boolean dont_route (void);

  void dont_route (CORBA::Boolean dont_route);

  CORBA::Boolean no_delay (void);

  void no_delay (CORBA::Boolean no_delay);

  CORBA::Boolean enable_network_priority (void);

  void enable_network_priority (CORBA::Boolean enable);

  /// This method writes a CDR representation of StreamControlProtocolProperties.
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /// This method reads the object state from a CDR representation.
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_StreamControl_Protocol_Properties (void);

private:
  // = Attributes.

  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
  CORBA::Boolean keep_alive_;
  CORBA::Boolean dont_route_;
  CORBA::Boolean no_delay_;
  CORBA::Boolean enable_network_priority_;
};

//*************************************************************************

class TAO_RTCORBA_Export TAO_GIOP_Protocol_Properties
  : public RTCORBA::GIOPProtocolProperties,
    public ::CORBA::LocalObject
{
public:

  // = CDR encoding methods
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_GIOP_Protocol_Properties ();

  friend class TAO_Protocol_Properties_Factory;
  TAO_GIOP_Protocol_Properties (void);
};

//*************************************************************************

class TAO_RTCORBA_Export TAO_Protocol_Properties_Factory
{
public:

  /**
   * Creates the proper transport ProtocolProperties subclass that matches
   * the IOP::ProfileId.
   * NOTE: Each IOP::ProfileId corresponds to two sets of protocol
   * properties: one describes the transport protocol and the other
   * describes the ORB messaging protocol.
   */
  static RTCORBA::ProtocolProperties *create_transport_protocol_property (
      IOP::ProfileId id,
      TAO_ORB_Core *orb_core);

  /**
   * Creates the proper orb ProtocolProperties subclass for
   * IOP::ProfileId.
   * NOTE: Each IOP::ProfileId corresponds to two sets of protocol
   * properties: one describes the transport protocol and the other
   * describes the ORB messaging protocol.
   */
  static RTCORBA::ProtocolProperties *create_orb_protocol_property (IOP::ProfileId id);

private:

  TAO_Protocol_Properties_Factory (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_RT_POLICY_I_H */
