// -*- C++ -*-

//=============================================================================
/**
 *  @file    Home_Servant_Impl_Base.h
 *
 *  $Id$
 *
 *  This file contains the non-template declaration of a base class for
 *  the template mixin for the generated home servant class.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_HOME_SERVANT_IMPL_BASE_H
#define CIAO_HOME_SERVANT_IMPL_BASE_H

#include /**/ "ace/pre.h"

#include "CIAO_Servant_Impl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <ccm/CCM_HomeS.h>

namespace CIAO
{
  class Session_Container;

  /**
   * @class Home_Servant_Impl_Base
   *
   * @brief Non-template base class for Home_Servant_Impl.
   *
   * Holds the non-template parts of its child class
   * Home_Servant_Impl.
   */
  class CIAO_Servant_Impl_Export Home_Servant_Impl_Base
    : public virtual POA_Components::CCMHome
  {
  public:
    Home_Servant_Impl_Base (Session_Container * c);

    virtual ~Home_Servant_Impl_Base (void);

    // Operations for CCMHome interface.

    virtual ::CORBA::IRObject_ptr  get_component_def (void);

    virtual ::CORBA::IRObject_ptr get_home_def (void);

    virtual void
    update_component_map (PortableServer::ObjectId &oid) = 0;

  protected:
    Session_Container *container_;
    
  private:
    /// Not to be used
    Home_Servant_Impl_Base (void);
  };
}

#include /**/ "ace/post.h"

#endif /* CIAO_HOME_SERVANT_IMPL_BASE_H */
