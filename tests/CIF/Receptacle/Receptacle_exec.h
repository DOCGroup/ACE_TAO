// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEPTACLE_EXEC_H_
#define CIAO_RECEPTACLE_EXEC_H_

#include /**/ "ace/pre.h"

#include "ReceptacleEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Receptacle_exec_export.h"
#include "tao/LocalObject.h"


namespace CIAO_Receptacle_Impl
{

  class RECEPTACLE_EXEC_Export Receptacle_exec_i
    : public virtual Receptacle_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receptacle_exec_i (void);
    virtual ~Receptacle_exec_i (void);

    //@{
    /** Operations from Components::SessionComponent. */

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}


  private:
    ::CCM_Receptacle_Context_var context_;
  };

  extern "C" RECEPTACLE_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Receptacle_Impl (void);
}

namespace CIAO_Provider_Impl
{

  class RECEPTACLE_EXEC_Export Provider_exec_i
    : public virtual Provider_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Provider_exec_i (void);
    virtual ~Provider_exec_i (void);

    virtual ::CCM_foo_ptr
    get_prov_foo (void);

    //@{
    /** Operations from Components::SessionComponent. */

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}


  private:
    ::CCM_Provider_Context_var context_;
  };

  extern "C" RECEPTACLE_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Provider_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

