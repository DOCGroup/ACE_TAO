/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO
//
// = DESCRIPTION
//   Implementation classes for the RT CORBA 1.0 policies.
//
// = FILENAME
//   RT_Policy_i.h
//
// = AUTHOR
//   Marina Spivak (marina@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_RT_POLICY_I_H
#define TAO_RT_POLICY_I_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_RT_CORBA == 1)

#include "tao/RTCORBAS.h"
#include "tao/LocalObject.h"

#include "ace/SString.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


class TAO_Export TAO_PriorityModelPolicy :  public RTCORBA::PriorityModelPolicy, public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   RTCORBA::PriorityModelPolicy implementation
  //
  // = DESCRIPTION
  //   This policy controls how the priority at which a server handles
  //   requests from clients is determined.
  //
public:
  TAO_PriorityModelPolicy (RTCORBA::PriorityModel priority_model,
                           RTCORBA::Priority server_priority);
  // Constructor.

  TAO_PriorityModelPolicy (const TAO_PriorityModelPolicy &rhs);
  // Copy constructor.

  virtual ~TAO_PriorityModelPolicy (void);
  // Destructor.

  RTCORBA::PriorityModel get_priority_model (void);
  // Accessor for the <priority_model_> attribute.  This method is
  // used internally by the orb.  This is a more efficient
  // (non-virtual, no exception handling) relative of the idl interface
  // implementation <priority_model> below.

  virtual RTCORBA::PriorityModel priority_model (CORBA::Environment
                                                 &ACE_TRY_ENV =
                                                 TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RTCORBA::Priority server_priority (CORBA::Environment
                                             &ACE_TRY_ENV =
                                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (CORBA::Environment
                                         &ACE_TRY_ENV =
                                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ())
        ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);
  // This method writes a CDR representation of the object state.

  virtual CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);
  // This method reads the object state from a CDR representation.

protected:

  friend class TAO_RT_PolicyFactory;
  TAO_PriorityModelPolicy (void);
  // This constructor is used by TAO_RT_PolicyFactory when decoding
  // policies from tagged components in an IOR.

private:

  RTCORBA::PriorityModel priority_model_;
  RTCORBA::Priority server_priority_;
  // Attributes.
};

//*************************************************************************

class TAO_Export TAO_ThreadpoolPolicy : public RTCORBA::ThreadpoolPolicy, public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   RTCORBA::ThreadpoolPolicy implementation
  //
  // = DESCRIPTION
  //   This policy controls which threadpool is associated with a POA.
  //
public:
  TAO_ThreadpoolPolicy (RTCORBA::ThreadpoolId id);
  // Constructor.

  TAO_ThreadpoolPolicy (const TAO_ThreadpoolPolicy &rhs);
  // Copy constructor.

  virtual ~TAO_ThreadpoolPolicy (void);
  // Destructor.

  virtual RTCORBA::ThreadpoolId threadpool (CORBA::Environment
                                            &ACE_TRY_ENV =
                                            TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::PolicyType policy_type (CORBA::Environment
                                         &ACE_TRY_ENV =
                                         TAO_default_environment ())
        ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  RTCORBA::ThreadpoolId id_;
  // Attribute.
};

//*************************************************************************

class TAO_Export TAO_PrivateConnectionPolicy : public RTCORBA::PrivateConnectionPolicy, public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   RTCORBA::PrivateConnectionPolicy implementation
  //
  // = DESCRIPTION
  //   This policy controls whether the transport connection for the
  //   Object is private, i.e., not available for carrying out invocations on
  //   other objects.
  //
public:
  TAO_PrivateConnectionPolicy (void);
  // Constructor.

  TAO_PrivateConnectionPolicy (const TAO_PrivateConnectionPolicy &rhs);
  // Copy constructor.

  virtual ~TAO_PrivateConnectionPolicy (void);
  // Destructor.

  virtual CORBA::PolicyType policy_type (CORBA::Environment
                                         &ACE_TRY_ENV =
                                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
};

//*************************************************************************

class TAO_Export TAO_PriorityBandedConnectionPolicy : public RTCORBA::PriorityBandedConnectionPolicy, public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   RTCORBA::PriorityBandedConnectionPolicy implementation
  //
  // = DESCRIPTION
  //   RT CORBA provides facility for a client to communicate with a
  //   server via multiple connections, aka bands, with each connection handling
  //   invocations that are made at a different CORBA priority.
  //   This policy allows the specification of the number of such
  //   bands and their priorities.
  //
public:
  TAO_PriorityBandedConnectionPolicy (const RTCORBA::PriorityBands &bands);
  // Constructor.

  TAO_PriorityBandedConnectionPolicy (const
                                      TAO_PriorityBandedConnectionPolicy &rhs);
  // Copy constructor.

  virtual ~TAO_PriorityBandedConnectionPolicy (void);
  // Destructor.

  RTCORBA::PriorityBands &priority_bands_rep (void);
  // Accessor to the underlying priority bands of the policy (does not
  // make a copy like the idl accessor <priority_bands> implementation
  // below.)

  virtual RTCORBA::PriorityBands * priority_bands (CORBA::Environment
                                                   &ACE_TRY_ENV =
                                                   TAO_default_environment ())
  ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (CORBA::Environment
                                         &ACE_TRY_ENV =
                                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);
  // This method writes a CDR representation of the current object.

  virtual CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);
  // This method reads the object state from a CDR representation.

protected:

  friend class TAO_RT_PolicyFactory;
  TAO_PriorityBandedConnectionPolicy (void);
  // This constructor is used by TAO_RT_PolicyFactory when decoding
  // policies from tagged components in an IOR.

private:

  RTCORBA::PriorityBands priority_bands_;
  // Attribute.
};

//*************************************************************************

class TAO_Export TAO_ServerProtocolPolicy : public RTCORBA::ServerProtocolPolicy, public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   RTCORBA::ServerProtocolPolicy implementation
  //
  // = DESCRIPTION
  //   This policy controls selection and configuration of
  //   communication protocols on the server-side of the RT ORB.
  //
public:
  TAO_ServerProtocolPolicy (const RTCORBA::ProtocolList &protocols);
  // Constructor.

  TAO_ServerProtocolPolicy (const TAO_ServerProtocolPolicy &rhs);
  // Copy constructor.

  virtual ~TAO_ServerProtocolPolicy (void);
  // Destructor.

  RTCORBA::ProtocolList & protocols_rep (void);
  // Accessor to the underlying protocols list of the policy (does not
  // make a copy like the idl accessor <protocols> implementation
  // below.)

  virtual RTCORBA::ProtocolList * protocols (CORBA::Environment
                                             &ACE_TRY_ENV =
                                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (CORBA::Environment
                                         &ACE_TRY_ENV =
                                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  RTCORBA::ProtocolList protocols_;
  // Attribute.
};

//*************************************************************************

class TAO_Export TAO_ClientProtocolPolicy : public RTCORBA::ClientProtocolPolicy, public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   RTCORBA::ClientProtocolPolicy implementation
  //
  // = DESCRIPTION
  //   This policy controls selection and configuration of
  //   communication protocols on the client-side of the RT ORB.
  //
public:
  TAO_ClientProtocolPolicy (const RTCORBA::ProtocolList &protocols);
  // Constructor.

  TAO_ClientProtocolPolicy (const TAO_ClientProtocolPolicy &rhs);
  // Copy constructor.

  virtual ~TAO_ClientProtocolPolicy (void);
  // Destructor.

  RTCORBA::ProtocolList & protocols_rep (void);
  // Accessor to the underlying protocols list of the policy (does not
  // make a copy like the idl accessor <protocols> implementation
  // below.)

  virtual RTCORBA::ProtocolList * protocols (CORBA::Environment
                                             &ACE_TRY_ENV =
                                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (CORBA::Environment
                                         &ACE_TRY_ENV =
                                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  // = CDR Encoder/Decoder.

  virtual CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);
  // This method writes a CDR representation of the current object.

  virtual CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);
  // This method reads the object state from a CDR representation.

protected:

  friend class TAO_RT_PolicyFactory;
  TAO_ClientProtocolPolicy (void);
  // This constructor is used by TAO_RT_PolicyFactory when decoding
  // policies from tagged components in an IOR.

private:

  RTCORBA::ProtocolList protocols_;
  // Attribute.
};

//*************************************************************************

class TAO_Export TAO_TCP_Properties : public RTCORBA::TCPProtocolProperties, public TAO_Local_RefCounted_Object

{
  // = TITLE
  //   RTCORBA::TCPProtocolProperties implementation
  //
  // = DESCRIPTION
  //   Stores TCP Protocol configuration properties.
  //
public:
  TAO_TCP_Properties (CORBA::Long send_buffer_size =
                      ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                      CORBA::Long recv_buffer_size =
                      ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                      CORBA::Boolean keep_alive = 1,
                      CORBA::Boolean dont_route = 0,
                      CORBA::Boolean no_delay = 1);
  // Constructor.

  virtual ~TAO_TCP_Properties (void);
  // Destructor.

  virtual CORBA::Long send_buffer_size (CORBA::Environment
                                        &ACE_TRY_ENV =
                                        TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual void send_buffer_size (CORBA::Long send_buffer_size,
                                 CORBA::Environment &ACE_TRY_ENV =
                                 TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::Long recv_buffer_size (CORBA::Environment
                                        &ACE_TRY_ENV =
                                        TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual void recv_buffer_size (CORBA::Long recv_buffer_size,
                                 CORBA::Environment &ACE_TRY_ENV =
                                 TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::Boolean keep_alive (CORBA::Environment &ACE_TRY_ENV =
                                     TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual void keep_alive (CORBA::Boolean keep_alive,
                           CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::Boolean dont_route (CORBA::Environment &ACE_TRY_ENV =
                                     TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual void dont_route (CORBA::Boolean dont_route,
                           CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::Boolean no_delay (CORBA::Environment &ACE_TRY_ENV =
                                   TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual void no_delay (CORBA::Boolean no_delay,
                         CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ())
    ACE_THROW_SPEC (());


  virtual CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);
  // This method writes a CDR representation of TCPProtocolProperties.

  virtual CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);
  // This method reads the object state from a CDR representation.

private:
  // = Attributes.

  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
  CORBA::Boolean keep_alive_;
  CORBA::Boolean dont_route_;
  CORBA::Boolean no_delay_;
};

//*************************************************************************

class TAO_Export TAO_Unix_Domain_Properties : public RTCORBA::UnixDomainProtocolProperties, public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   RTCORBA::UnixDomainProtocolProperties implementation.
  //
  // = DESCRIPTION
  //   Stores Unix Domain Sockets (Local IPC) Protocol configuration
  //   properties.
  //
public:
  TAO_Unix_Domain_Properties (CORBA::Long send_buffer_size =
                              ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                              CORBA::Long recv_buffer_size =
                              ACE_DEFAULT_MAX_SOCKET_BUFSIZ);

  // Constructor.

  virtual ~TAO_Unix_Domain_Properties (void);
  // Destructor.

  virtual CORBA::Long send_buffer_size (CORBA::Environment
                                        &ACE_TRY_ENV =
                                        TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual void send_buffer_size (CORBA::Long send_buffer_size,
                                 CORBA::Environment &ACE_TRY_ENV =
                                 TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::Long recv_buffer_size (CORBA::Environment
                                        &ACE_TRY_ENV =
                                        TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual void recv_buffer_size (CORBA::Long recv_buffer_size,
                                 CORBA::Environment &ACE_TRY_ENV =
                                 TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);
  // This method writes the CDR encapsulation of an instance of
  // UnixDomainProperties. This Protocol Property in TAO specific,
  // so there is no order of encapsulation specified in the
  // RT CORBA Spec. The current implementation encodes the field
  // according to the order of declaration (i.e. first is encoded
  // send_buffer_size and then recv_buffer_size).

  virtual CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);
  // This method reads an instance of UnixDomainProperties from
  // a CDR encapsulation. This Protocol Property in TAO specific,
  // so there is no order of encapsulation specified in the
  // RT CORBA Spec. The current implementation expect the field
  // according to the order of declaration.

private:

  // = Attributes.
  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
};

//*************************************************************************

class TAO_Export TAO_SMEM_Properties : public RTCORBA::SharedMemoryProtocolProperties, public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   RTCORBA::SharedMemoryProtocolProperties implementation.
  //
  // = DESCRIPTION
  //   Stores Shared Memory Protocol configuration
  //   properties.
  //
public:
  TAO_SMEM_Properties (void);
  // Constructor.

  virtual ~TAO_SMEM_Properties (void);
  // Destructor.

  // = IDL interface methods.

  virtual CORBA::Long preallocate_buffer_size (CORBA::Environment
                                               &ACE_TRY_ENV =
                                               TAO_default_environment
                                               ())
    ACE_THROW_SPEC (());

  virtual void preallocate_buffer_size (CORBA::Long preallocate_buffer_size,
                                        CORBA::Environment
                                        &ACE_TRY_ENV = TAO_default_environment
                                        ())
    ACE_THROW_SPEC (());

  virtual char * mmap_filename (CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual void mmap_filename (const char * mmap_filename,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual char * mmap_lockname (CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual void mmap_lockname (const char * mmap_lockname,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);
  // This method writes the CDR encapsulation of an instance of
  // SMEMProperties. This Protocol Property in TAO specific,
  // so there is no order of encapsulation specified in the
  // RT CORBA Spec. The current implementation encodes the field
  // according to the order of declaration.

  virtual CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);
  // This method reads an instance of SMEMProperties from
  // a CDR encapsulation. This Protocol Property in TAO specific,
  // so there is no order of encapsulation specified in the
  // RT CORBA Spec. The current implementation expect the field
  // according to the order of declaration.

private:

  // = Attributes.
  CORBA::Long preallocate_buffer_size_;
  ACE_CString mmap_filename_;
  ACE_CString mmap_lockname_;
};

//*************************************************************************

class TAO_Export TAO_GIOP_Properties : public RTCORBA::GIOPProtocolProperties, public TAO_Local_RefCounted_Object
{
public:
  virtual ~TAO_GIOP_Properties ();

  // = CDR encoding methods

  virtual CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  virtual CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

protected:
  friend class TAO_Protocol_Properties_Factory;
  TAO_GIOP_Properties (void);
};

//*************************************************************************

class TAO_Export TAO_Protocol_Properties_Factory
{
public:

  static RTCORBA::ProtocolProperties*
  create_transport_protocol_property (IOP::ProfileId id);
  // Creates the proper transport ProtocolProperties subclass that matches
  // the IOP::ProfileId.
  // NOTE: Each IOP::ProfileId corresponds to two sets of protocol
  // properties: one describes the transport protocol and the other
  // describes the ORB messaging protocol.

  static RTCORBA::ProtocolProperties*
  create_orb_protocol_property (IOP::ProfileId id);
  // Creates the proper orb ProtocolProperties subclass for
  // IOP::ProfileId.
  // NOTE: Each IOP::ProfileId corresponds to two sets of protocol
  // properties: one describes the transport protocol and the other
  // describes the ORB messaging protocol.

protected:
  TAO_Protocol_Properties_Factory (void);
};

#if defined (__ACE_INLINE__)
#include "tao/RT_Policy_i.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/post.h"
#endif /* TAO_RT_POLICY_I_H */
