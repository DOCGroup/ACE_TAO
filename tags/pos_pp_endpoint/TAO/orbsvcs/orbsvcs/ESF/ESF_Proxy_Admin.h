/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_Proxy_Admin
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_ESF_PROXY_ADMIN_H
#define TAO_ESF_PROXY_ADMIN_H

#include "ESF_Proxy_Collection.h"
#include "ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class EVENT_CHANNEL, class PROXY, class INTERFACE>
class TAO_ESF_Proxy_Admin
{
  // = TITLE
  //   ESF_Proxy_Admin
  //
  // = DESCRIPTION
  //   Implement common tasks in the Admin interfaces.
  //
  // = REQUIREMENTS
  //
  //   The EVENT_CHANNEL interface must implement:
  //
  //   void create_proxy (PROXY*&);
  //   // create a new proxy
  //   void destroy_proxy (PROXY*);
  //   // destroy a proxy
  //
  //   void create_proxy_collection (TAO_ESF_Proxy_Collection<PROXY>*&);
  //   // create a proxy collection
  //   void destroy_proxy_collection (TAO_ESF_Proxy_Collection<PROXY>*&);
  //   // destroy a proxy collection
  //
  //  In addition to the requirements imposed by
  //  TAO_ESF_Proxy_Collection<>, the PROXY interface must define:
  //
  //  typename .... _ptr_type;
  //  // The T_ptr for the IDL interface implemented by the PROXY.
  //  typename .... _var_type;
  //  // The T_var for the IDL interface implemented by the PROXY.
  //
  //  PROXY::_ptr_type
  //      PROXY::activate (CORBA::Environment &) throw ();
  //  // activate the proxy and return the object reference
  //
public:
  TAO_ESF_Proxy_Admin (EVENT_CHANNEL *ec);
  // Constructor

  virtual ~TAO_ESF_Proxy_Admin (void);
  // destructor

  void for_each (TAO_ESF_Worker<PROXY> *worker,
                 CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC (());
  // Iterate over its internal collection.

  // @@ We should use INTERFACE::_ptr_type or PROXY::_ptr_type, but
  // the MSVC compiler (v6.0) gets confused when we do so.  So we have
  // to choose for the lesser evil.  The code works because TAO uses
  // pointers to implement the _ptr types, and that is OK because this
  // code is supposed to run under TAO only.
  virtual INTERFACE*
      obtain (CORBA::Environment &)
          ACE_THROW_SPEC (());
  // Create a new PROXY and activate it.

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC (());
  // The Event Channel that owns this Admin object is going
  // down. Invoke <shutdown> on all the proxies, cleanup the
  // collection and prepare to terminate.

  virtual void connected (PROXY *proxy,
                          CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC (());
  // A <proxy> has connected, this is invoked when the proxy's client
  // has invoked the connect_xxx_yyy() method.
  // The default implementation is a no-op.

  virtual void reconnected (PROXY *proxy,
                            CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC (());
  // A <proxy> has reconnected, i.e. its client has invoked the
  // connect_xxx_yyy() method, but the proxy was connected already.
  // The default implementation delegates on the collection
  // <reconnected> method

  virtual void disconnected (PROXY *proxy,
                             CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC (());
  // A <proxy> has been disconnected. The default implementation
  // removes the object from the collection and deactivates the
  // proxy.

protected:
  EVENT_CHANNEL *event_channel_;
  // The Event Channel we belong to

private:
  typedef TAO_ESF_Proxy_Collection<PROXY> Collection;

  Collection *collection_;
  // The supplier container.
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "ESF_Proxy_Admin.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Proxy_Admin.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Proxy_Admin.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PROXY_ADMIN_H */
