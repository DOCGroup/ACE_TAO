// -*- C++ -*-
//
// $Id$

#ifndef CIAO_DISTRIBUTOR_EXEC_H_
#define CIAO_DISTRIBUTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "DistributorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Distributor_exec_export.h"
#include "tao/LocalObject.h"
#include "ace/Thread_Mutex.h"
#include "ace/Reactor.h"

#include <map>

namespace CIAO_Quoter_Distributor_Impl
{
  class Distributor_exec_i;

  /**
  * @class pulse generator
  *
  * @brief An active object used by StockDistributor to perform periodical
  * pulse generation
  *
  */
  class pulse_Generator : public ACE_Event_Handler
  {
  public:
    pulse_Generator (Distributor_exec_i &callback);

    virtual ~pulse_Generator ();

    /// Handle the timeout.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    /// Maintains a handle that actually process the event
    Distributor_exec_i &pulse_callback_;
  };

  class DISTRIBUTOR_EXEC_Export Distributor_exec_i
    : public virtual Distributor_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Distributor_exec_i (void);
    virtual ~Distributor_exec_i (void);

    void tick (void);

    virtual void add_stock (const char * stock);

    virtual void del_stock (const char * stock);

    virtual void start (void);

    virtual void stop (void);

    virtual ::CORBA::ULong rate (void);

    virtual void rate (::CORBA::ULong rate);

    // Port operations.
    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_info_out_connector_status (void);

    virtual void
    set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    pulse_Generator * ticker_;
    ::Quoter::CCM_Distributor_Context_var context_;
    TAO_SYNCH_MUTEX mutex_;
    typedef std::map<ACE_CString, Quoter::Stock_Info_var> Stock_Table;
    Stock_Table stocks_;
    CORBA::ULong rate_;
  };

  extern "C" DISTRIBUTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Quoter_Distributor_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

