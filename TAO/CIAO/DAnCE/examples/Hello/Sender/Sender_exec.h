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

namespace Sender_Impl
{

  typedef ::Components::EnterpriseComponent_ptr (*ExecFactory) (void);

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

  protected:
    int count_;
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
