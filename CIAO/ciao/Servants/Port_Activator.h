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

#include "CIAO_Servant_Impl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "ciao/Servants/CIAO_Port_ActivatorC.h"

namespace CIAO
{
  /**
   * @class Port_Activator_i
   *
   * @brief Base class for port activations
   *
   * This is a ABC for the Port_Activator_T <> template class where
   * the bulk of the work is done. This class is simply a place holder
   * so that we could use the pointer to this class to achieve
   * polymorphism for facet and consumer servant activations.
   *
   */
  class CIAO_Servant_Impl_Export Port_Activator_i :
    public virtual Port_Activator
  {
  public:
    Port_Activator_i (const char *oid,
                      const char *name,
                      Port_Activator_Types::Type t);

    virtual ~Port_Activator_i (void);

    /// Return the oid of port that this activator encapulates.
    virtual char* oid (void);

    /// Set the oid
    virtual void oid (const char* oid);

    /// Return the oid of port that this activator encapulates.
    virtual char* name (void);

    /// Activate the servant reponsible for this port.
    /**
     * There are intentionally no throw specifications. It is the
     * caller's responsibility to handle CORBA and C++
     * specifications.
     */
    virtual PortableServer::Servant activate (
        const PortableServer::ObjectId &oid) = 0;

    virtual void deactivate (::PortableServer::Servant servant,
                             CORBA::Booolean remaining_activations) = 0;

  protected:
    /// The object ID that is used to activate the servant within the POA.
    CORBA::String_var oid_;

    /// Name of the port
    CORBA::String_var name_;

    /// What type of port is it, a consumer or facet.
    Port_Activator_Types::Type const t_;
  };
}

#include /**/ "ace/post.h"

#endif /*CIAO_PORT_ACTIVATOR_H*/
