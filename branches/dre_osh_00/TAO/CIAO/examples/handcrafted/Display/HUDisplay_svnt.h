// $Id$

// ===========================================================
//
// @file HUDisplay_svnt.h
//
//    Servant Glue code (supposedly should be) generated using CIAO'S
//    CIDL compiler.  This file implement the servants that bridge the
//    executor and the container.
//
//    This file is "generated" using the template code under
//      $(CIAO_ROOT)/docs/templates/
//
//    Remember to refect any changes back to the code templates.
//
// @author Nanbor Wang <nanbor@cs.wustl.edu>
//
// ===========================================================

#ifndef CIAO_GLUE_SESSION_HUDISPLAYGS_H
#define CIAO_GLUE_SESSION_HUDISPLAYGS_H
#include "ace/pre.h"

#include "HUDisplayS.h"         // Source in the skeletons for component
                                // client-view equivalent interfaces
#include "HUDisplayEC.h"        // Source in the executor mapping
                                // that component implementations use
#include "ciao/Container_Base.h" //Source in the container interface definitions

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// @@@ Notice that all component and interface names may need to be
/// fully qualified as we are creating a new namespace for the CIAO's
/// container glue code.

namespace CIAO_GLUE_HUDisplay
{
  class HUDISPLAY_SVNT_Export position_Servant :
    public virtual POA_HUDisplay::position,
    public virtual PortableServer::RefCountServantBase
  {
  public:
    position_Servant (HUDisplay::CCM_position_ptr executor,
                      Components::CCMContext_ptr ctx_);

    ~position_Servant ();

    virtual CORBA::Long posx (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CORBA::Long posy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // get_component implementation.
    virtual CORBA::Object_ptr _get_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    // Facet executor.
    HUDisplay::CCM_position_var executor_;

    // Context object.
    Components::CCMContext_var ctx_;
  };
}

#if defined (__ACE_INLINE__)
# include "HUDisplay_svnt.inl"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /* CIAO_GLUE_SESSION_HUDISPLAYGS_H */
