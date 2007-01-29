//$Id$
/*
 * @file Distributor_exec.h
 *
 * @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */



#ifndef CIAO_DISTRIBUTOR_EXEC_H
#define CIAO_DISTRIBUTOR_EXEC_H

#include /**/ "ace/pre.h"
#include "ace/Thread_Manager.h"
#include "ace/Task.h"


#include "Distributor_svnt.h"
#include "DistributorEC.h"
#include "Distributor_exec_export.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIDL_StockDistributor_Impl
{
  class StockDistributor_exec_i;

  /**
   * @class StockQuoter_exec_i
   *
   * @brief Executor implementation
   *
   * This class implements Stock::StockQuoter interface. We use this interface
   * to acquire the information of the stock
   */
  class DISTRIBUTOR_EXEC_Export StockQuoter_exec_i
  : public virtual ::Stock::CCM_StockQuoter,
  public virtual TAO_Local_RefCounted_Object
  {
    public:
    StockQuoter_exec_i (StockDistributor_exec_i &distributor)
                :distributor_ (distributor)
    {
    }

    // Operations from ::Stock::StockQuoter

    virtual ::Stock::StockInfo *
    get_stock_info (
    const char * stock_name)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Stock::Invalid_Stock));

    /// User defined

    private:
    /// Maintains a handle to the distributor so that the quoter can refer to it for latest quote
    StockDistributor_exec_i &distributor_;
  };

  class StockDistributor_exec_i;

  /**
  * @class pulse generator
  *
  * @brief an active object used by StockDistributor to perform periodical pulse generation
  *
  */
  class pulse_Generator : public ACE_Task_Base
  {
  public:
    pulse_Generator (StockDistributor_exec_i *callback);

    ~pulse_Generator ();

    /// Hook method that performs application-defined initialization activities
    int open_h (void);

    /// Hook method that performs application-defined destruction activites
    int close_h (void);

    /// appliation-defined method for starting the pulse-generation service
    int start (CORBA::Long hertz);

    /// application-defined method for stopping the pulse-generation service
    int stop (void);

    /// Indicate whether the current object is active
    int active (void);

    /// Handle the timeout.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);

    /// Called when timer handler is removed.
    virtual int handle_close (ACE_HANDLE handle,
                              ACE_Reactor_Mask close_mask);

    /// Hook methods implemnting the task's service processing,
    /// invoked by all threads activated by activate () method
    virtual int svc (void);

  private:
    /// Tracking whether we are actively generating pulse or not.
    int active_;

    /// Maintains a handle that actually process the event
    StockDistributor_exec_i *pulse_callback_;

  };


  /**
   * @class StockDistributor_exec_i
   *
   * @brief Executor implementation
   *
   * This class implements Stock::StockDistributor component
   */
  class DISTRIBUTOR_EXEC_Export StockDistributor_exec_i
  : public virtual StockDistributor_Exec,
  public virtual TAO_Local_RefCounted_Object
  {
    public:
    StockDistributor_exec_i (void);
    virtual ~StockDistributor_exec_i (void);

    // Supported or inherited operations.

    virtual void
    start ()
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    stop ()
    ACE_THROW_SPEC ((CORBA::SystemException));

    // Attribute operations.

    virtual ::CORBA::Long
    rate ()
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    rate (
    ::CORBA::Long)
    ACE_THROW_SPEC ((CORBA::SystemException));

    // Port operations.

    virtual ::Stock::CCM_StockQuoter_ptr
    get_push_quoter ()
    ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (
    ::Components::SessionContext_ptr ctx)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ciao_preactivate ()
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ciao_postactivate ()
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ccm_activate ()
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ccm_passivate ()
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ccm_remove ()
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    protected:
    StockDistributor_Context *context_;

    /// User defined members
    public:

    /// push event to all subscribers
    void push_notify_out ()
    ACE_THROW_SPEC ((
	::CORBA::SystemException
    ));

    private:
    /// pulse generation rate
    CORBA::Long rate_;

    /// pulser object
    pulse_Generator pulser_;

    /// "real-time" stock value
    static CORBA::Long msft_;

    /// "real-tiime" stock value
    static CORBA::Long ibm_;

    friend class StockQuoter_exec_i;
  };


  /**
   * @class StockDistributorHome_exec_i
   *
   * @brief Executor implementation
   *
   * This class implements Stock::StockDistributorHome
   */
  class DISTRIBUTOR_EXEC_Export StockDistributorHome_exec_i
  : public virtual StockDistributorHome_Exec,
  public virtual TAO_Local_RefCounted_Object
  {
    public:
    StockDistributorHome_exec_i (void);
    virtual ~StockDistributorHome_exec_i (void);

    virtual ::Components::EnterpriseComponent_ptr
    create ()
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));
  };

  extern "C" DISTRIBUTOR_EXEC_Export ::Components::HomeExecutorBase_ptr
  createStockDistributorHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_DISTRIBUTOR_EXEC_H */

