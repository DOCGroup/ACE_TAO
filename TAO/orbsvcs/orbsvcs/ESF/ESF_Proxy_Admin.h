/* -*- C++ -*- */
/**
 *  @file   ESF_Proxy_Admin.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_PROXY_ADMIN_H
#define TAO_ESF_PROXY_ADMIN_H

#include "ESF_Proxy_Collection.h"
#include "ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_ESF_Proxy_Admin
 *
 * @brief Implement common tasks in the Admin interfaces.
 *
 * <H2>Requirements</H2>
 *
 * The EVENT_CHANNEL interface must implement:
 *
 * @verbatim
 * void create_proxy (PROXY*&);
 * // create a new proxy
 *
 * void destroy_proxy (PROXY*);
 * // destroy a proxy
 *
 * void create_proxy_collection (TAO_ESF_Proxy_Collection<PROXY>*&);
 * // create a proxy collection
 *
 * void destroy_proxy_collection (TAO_ESF_Proxy_Collection<PROXY>*&);
 * // destroy a proxy collection
 * @endverbatim
 *
 * In addition to the requirements imposed by
 * TAO_ESF_Proxy_Collection<>, the PROXY interface must define:
 *
 * @verbatim
 * typename .... _ptr_type;
 * // The T_ptr for the IDL interface implemented by the PROXY.
 *
 * typename .... _var_type;
 * // The T_var for the IDL interface implemented by the PROXY.
 *
 * PROXY::_ptr_type
 * PROXY::activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED) throw ();
 * // activate the proxy and return the object reference
 * @endverbatim
 *
 */
template<class EVENT_CHANNEL, class PROXY, class INTERFACE>
class TAO_ESF_Proxy_Admin
{
public:
  /// Constructor, allocate the internal collection
  TAO_ESF_Proxy_Admin (EVENT_CHANNEL *ec);

  /// Cleanup internal resources, destroy the internal collection
  virtual ~TAO_ESF_Proxy_Admin (void);

  /// Iterate over its internal collection.
  void for_each (TAO_ESF_Worker<PROXY> *worker
                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  // @TODO We should use INTERFACE::_ptr_type or PROXY::_ptr_type, but
  // the MSVC compiler (v6.0) gets confused when we do so.  So we have
  // to choose for the lesser evil.  The code works because TAO uses
  // pointers to implement the _ptr types, and that is OK because this
  // code is supposed to run under TAO only.
  /// Create a new PROXY and activate it.
  virtual INTERFACE*
      obtain (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * The Event Channel that owns this Admin object is going
   * down. Invoke <shutdown> on all the proxies, cleanup the
   * collection and prepare to terminate.
   */
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * A <proxy> has connected, this is invoked when the proxy's client
   * has invoked the connect_xxx_yyy() method.
   * The default implementation is a no-op.
   */
  virtual void connected (PROXY *proxy
                          ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * A <proxy> has reconnected, i.e. its client has invoked the
   * connect_xxx_yyy() method, but the proxy was connected already.
   * The default implementation delegates on the collection
   * <reconnected> method
   */
  virtual void reconnected (PROXY *proxy
                            ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * A <proxy> has been disconnected. The default implementation
   * removes the object from the collection and deactivates the
   * proxy.
   */
  virtual void disconnected (PROXY *proxy
                             ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  /// The Event Channel we belong to
  EVENT_CHANNEL *event_channel_;

private:
  /// Shorthand for the Proxy collection
  typedef TAO_ESF_Proxy_Collection<PROXY> Collection;

  /// The proxy collection object
  Collection *collection_;
};

// ****************************************************************
template <class COUNTED>
class TAO_ESF_RefCountedRef
{
public:
  TAO_ESF_RefCountedRef (COUNTED *counted);
  ~TAO_ESF_RefCountedRef ();
private:
  TAO_ESF_RefCountedRef (TAO_ESF_RefCountedRef<COUNTED> const & );
  TAO_ESF_RefCountedRef& operator= (TAO_ESF_RefCountedRef<COUNTED> const & );

  COUNTED *counted_;
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
