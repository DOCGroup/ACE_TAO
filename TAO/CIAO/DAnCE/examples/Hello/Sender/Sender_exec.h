//$Id$
//============================================================
/**
 * @file Sender_exec.h
 *
 * Header file for the Executor implementation.
 */
//============================================================

#ifndef SENDER_EXEC_H
#define SENDER_EXEC_H

#include "SwapExecC.h"
#include "SenderEC.h"
#include "Sender_exec_export.h"
#include "tao/LocalObject.h"
#include "ace/DLL.h"
#include "ciao/CCM_EventC.h"

namespace Sender_Impl
{
  class SenderSwap_exec_i;


  typedef ::Components::EnterpriseComponent_ptr (*ExecFactory) (SenderSwap_exec_i *);

  /**
   * @class Sender_exec_i
   *
   * Sender executor implementation class.
   */

  class SENDER_EXEC_Export SenderSwap_exec_i :
      public virtual CIAO::Swap_Exec,
      public virtual TAO_Local_RefCounted_Object
  {
  public:
    SenderSwap_exec_i ();

    ~SenderSwap_exec_i ();

    virtual ::Components::EnterpriseComponent_ptr
    incarnate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::EnterpriseComponent_ptr
    etherealize (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void  consumers (::Components::ConsumerDescriptions *p)
    {
      this->consumers_ = p;
    }

    ::Components::ConsumerDescriptions *consumers (void)
    {
      return this->consumers_._retn ();
    }

  protected:
    int count_;

    ::Components::ConsumerDescriptions_var consumers_;
  };

  class SENDER_EXEC_Export SenderHome_exec_i :
    public virtual SenderHome_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    SenderHome_exec_i ();

    /// Default dtor.
    virtual ~SenderHome_exec_i ();

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));


  };

}

extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
createSenderHome_Impl (void);

#endif /* SENDER_EXEC_H */
