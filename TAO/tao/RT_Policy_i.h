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

#include "RTCORBAS.h"


#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


class Policy_Factory;

class TAO_Export TAO_PriorityModelPolicy :
  public RTCORBA::PriorityModelPolicy
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

  virtual RTCORBA::PriorityModel priority_model (CORBA::Environment
                                                 &ACE_TRY_ENV =
                                                 TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual RTCORBA::Priority server_priority (CORBA::Environment
                                             &ACE_TRY_ENV =
                                             TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::PolicyType policy_type (CORBA::Environment
                                         &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual CORBA::Boolean _tao_encode(TAO_OutputCDR &out_cdr);
  // This method writes a CDR representation of the current object
  
  virtual CORBA::Boolean _tao_decode(TAO_InputCDR &in_cdr);
  // This method reads the object state from a CDR representation

protected:

  friend class Policy_Factory;
  TAO_PriorityModelPolicy();
  // This constructor is needed to create and instance
  // and then restore its state from the CDR stream. In
  // fact the logic of how the policy is streamed out and
  // in a CDR stream is encapsulated in the class.


private:

  RTCORBA::PriorityModel priority_model_;
  RTCORBA::Priority server_priority_;
  // Attributes.
};

////////////////////////////////////////////////////////////////////////////

class TAO_Export TAO_ThreadpoolPolicy : public RTCORBA::ThreadpoolPolicy
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
                                         TAO_default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

private:

  RTCORBA::ThreadpoolId id_;
  // Attribute.
};

//////////////////////////////////////////////////////////////////////////////

class TAO_Export TAO_PrivateConnectionPolicy :
  public RTCORBA::PrivateConnectionPolicy
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
                                         TAO_default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());
};
////////////////////////////////////////////////////////////////////////////

class TAO_Export TAO_PriorityBandedConnectionPolicy :
  public  RTCORBA::PriorityBandedConnectionPolicy
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

  virtual RTCORBA::PriorityBands * priority_bands (CORBA::Environment
                                                   &ACE_TRY_ENV =
                                                   TAO_default_environment ())
  ACE_THROW_SPEC (());

  virtual CORBA::PolicyType policy_type (CORBA::Environment
                                         &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual CORBA::Boolean _tao_encode(TAO_OutputCDR &out_cdr);
  // This method writes a CDR representation of the current object
  
  virtual CORBA::Boolean _tao_decode(TAO_InputCDR &in_cdr);
  // This method reads the object state from a CDR representation

protected:

  friend class Policy_Factory;
  TAO_PriorityBandedConnectionPolicy();
  // This constructor is needed to create and instance
  // and then restore its state from the CDR stream. In
  // fact the logic of how the policy is streamed out and
  // in a CDR stream is encapsulated in the class.


private:

  RTCORBA::PriorityBands priority_bands_;
  // Attribute.
};
////////////////////////////////////////////////////////////////////////////

class TAO_Export TAO_ServerProtocolPolicy :
  public RTCORBA::ServerProtocolPolicy
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

  virtual RTCORBA::ProtocolList * protocols (CORBA::Environment
                                             &ACE_TRY_ENV =
                                             TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::PolicyType policy_type (CORBA::Environment
                                         &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual CORBA::Boolean _tao_encode(TAO_OutputCDR &out_cdr);
  // This method writes a CDR representation of the current object
  
  virtual CORBA::Boolean _tao_decode(TAO_InputCDR &in_cdr);
  // This method reads the object state from a CDR representation

protected:

  friend class Policy_Factory;
  TAO_ServerProtocolPolicy();
  // This constructor is needed to create and instance
  // and then restore its state from the CDR stream. In
  // fact the logic of how the policy is streamed out and
  // in a CDR stream is encapsulated in the class.

private:

  RTCORBA::ProtocolList protocols_;
  // Attribute.
};

////////////////////////////////////////////////////////////////////////////

class TAO_Export TAO_ClientProtocolPolicy :
  public  RTCORBA::ClientProtocolPolicy
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

  virtual RTCORBA::ProtocolList * protocols (CORBA::Environment
                                             &ACE_TRY_ENV =
                                             TAO_default_environment ())
    ACE_THROW_SPEC (());

  virtual CORBA::PolicyType policy_type (CORBA::Environment
                                         &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual CORBA::Policy_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV =
                                         TAO_default_environment ());

private:

  RTCORBA::ProtocolList protocols_;
  // Attribute.
};

////////////////////////////////////////////////////////////////////////////

class TAO_Export TAO_TCP_Properties : public RTCORBA::TCPProtocolProperties,
                                      public TAO_Encodable
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


  virtual CORBA::Boolean _tao_encode(TAO_OutputCDR &out_cdr);
  // This method writes a CDR representation of the current object
  
  virtual CORBA::Boolean _tao_decode(TAO_InputCDR &in_cdr);
  // This method reads the object state from a CDR representation


private:
  // = Attributes.

  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
  CORBA::Boolean keep_alive_;
  CORBA::Boolean dont_route_;
  CORBA::Boolean no_delay_;
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
