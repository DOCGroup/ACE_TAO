// $Id$

// ===========================================================
//
// @file BasicSP_svnt.h
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
// @author Balachandran Natarajan <bala@cs.wustl.edu>
//
// ===========================================================

#ifndef CIAO_GLUE_SESSION_BASICSP_H
#define CIAO_GLUE_SESSION_BASICSP_H
#include /**/ "ace/pre.h"

///Source in the container interface definitions
#include "ciao/Container_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Source in the skeletons for component client-view equivalent
/// interfaces
#include "BasicSPS.h"

// Source in the executor mapping that component implementations use
#include "BasicSPEC.h"

/// @@@ Notice that all component and interface names may need to be
/// fully qualified as we are creating a new namespace for the CIAO's
/// container glue code.

namespace CIAO_GLUE_BasicSP
{
  class BASICSP_SVNT_Export ReadData_Servant :
    public virtual POA_BasicSP::ReadData,
    public virtual PortableServer::RefCountServantBase
  {
  public:

    ReadData_Servant  (BasicSP::CCM_ReadData_ptr executor,
                        Components::CCMContext_ptr ctx_);
    ~ReadData_Servant (void);

    /// Template method.
    virtual char *get_data (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // get_component implementation.
    virtual CORBA::Object_ptr _get_component (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:

    // Facet executor.
    BasicSP::CCM_ReadData_var executor_;

    // Context object.
    Components::CCMContext_var ctx_;
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_GLUE_SESSION_HUDISPLAYGS_H */
