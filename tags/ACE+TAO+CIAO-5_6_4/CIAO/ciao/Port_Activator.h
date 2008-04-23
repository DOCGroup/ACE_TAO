// -*- C++ -*-
//=============================================================================
/**
 *  @file    Port_Activator.h
 *
 *  $Id$
 *
 *  @authors Bala Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_PORT_ACTIVATOR_H
#define CIAO_PORT_ACTIVATOR_H

#include /**/ "ace/pre.h"

#include "ciao/CIAO_Server_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"

namespace CIAO
{
  /**
   * @class Port_Activator
   *
   * @brief Base class for port activations
   *
   * This is a ABC for the Port_Activator_T <> template class where
   * the bulk of the work is done. This class is simply a place holder
   * so that we could use the pointer to this class to achieve
   * polymorphism for facet and consumer servant activations.
   *
   */
  class CIAO_SERVER_Export Port_Activator
  {
  public:
    enum Type
      {
        Facet,
        Sink
      };

    Port_Activator (const char *oid, const char *name, Type t);

    virtual ~Port_Activator (void);

    /// Return the oid of port that this activator encapulates.
    const char* oid (void) const;

    /// Set the oid
    void oid (const char* oid);

    /// Return the oid of port that this activator encapulates.
    const char* name (void) const;

    /// Activate the servant reponsible for this port.
    /**
     * There are intentionally no throw specifications. It is the
     * caller's responsibility to handle CORBA and C++
     * specifications.
     */
    virtual PortableServer::Servant activate (
        const PortableServer::ObjectId &oid) = 0;

    virtual void deactivate (const PortableServer::Servant servant) = 0;

  protected:
    /// The object ID that is used to activate the servant within the
    /// POA.
    CORBA::String_var oid_;

    /// Name of the port
    CORBA::String_var name_;

    /// What type of port is it, a consumer or facet.
    Type const t_;
  };
}

#if defined (__ACE_INLINE__)
# include "Port_Activator.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /*CIAO_PORT_ACTIVATOR_H*/
