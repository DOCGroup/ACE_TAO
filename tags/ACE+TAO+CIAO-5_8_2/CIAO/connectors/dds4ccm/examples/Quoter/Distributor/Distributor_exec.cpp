// -*- C++ -*-
//
// $Id$

#include "Distributor_exec.h"
#include "ace/Reactor.h"
#include "ace/Guard_T.h"
#include "ace/CORBA_macros.h"
#include "ace/OS_NS_time.h"
#include "tao/ORB_Core.h"

namespace CIAO_Quoter_Distributor_Impl
{
  //============================================================
  // Component Executor Implementation Class: Distributor_exec_i
  //============================================================

  pulse_Generator::pulse_Generator (Distributor_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  pulse_Generator::~pulse_Generator ()
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    // Notify the subscribers
    try
      {
        this->pulse_callback_.tick ();
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Distributor pulse_Generator::handle_timeout: "
                              "Caught exception\n"));
      }

    return 0;
  }

  Distributor_exec_i::Distributor_exec_i (void)
    : rate_ (1)
  {
    ACE_OS::srand (static_cast <u_int> (ACE_OS::time ()));
    this->ticker_ = new pulse_Generator (*this);
  }

  Distributor_exec_i::~Distributor_exec_i (void)
  {
  }

  // Supported operations and attributes.

  void
  Distributor_exec_i::tick (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Ticking\n"));

    for (Stock_Table::iterator i = this->stocks_.begin ();
         i != this->stocks_.end ();
         ++i)
      {
        if (ACE_OS::rand () % 2)
          {
            int const delta = (ACE_OS::rand () % 10) - 2;

            i->second->current += delta;

            if (i->second->current > i->second->high)
              i->second->high = i->second->current;

            if (i->second->current < i->second->low)
              i->second->low = i->second->current;

            ::Quoter::Writer_var writer =
              this->context_->get_connection_info_in_data ();

            if (! ::CORBA::is_nil (writer.in ())) {
              ACE_DEBUG ((LM_DEBUG, "WRITE AND CREATE stock_info for <%C> %u:%u:%u\n",
                            i->first.c_str (),
                            i->second->low,
                            i->second->current,
                            i->second->high));
              try
                {
                  writer->write_one (i->second, ::DDS::HANDLE_NIL);
                }
              catch (const CCM_DDS::InternalError& )
                {
                  ACE_ERROR ((LM_ERROR, "Internal Error while writing Stock_info for <%C>.\n",
                                i->first.c_str ()));
                }
            }
            else
              ACE_ERROR ((LM_ERROR, "Writer reference is nil!\n"));
          }
      }
  }

  void
  Distributor_exec_i::add_stock (const char * stock)
  {
    ACE_DEBUG ((LM_DEBUG, "Distributor_exec_i::add_stock - Adding stock: %C\n", stock));

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->mutex_, CORBA::INTERNAL ());

    Quoter::Stock_Info *new_stock = new Quoter::Stock_Info;

    new_stock->low = 50;
    new_stock->high = 50;
    new_stock->current = 50;
    new_stock->symbol = stock;

    this->stocks_[stock] = new_stock;
  }

  void
  Distributor_exec_i::del_stock (const char * stock)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Distributor_exec_i::del_stock - Removing stock: %C\n", stock));

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->mutex_, CORBA::INTERNAL ());

    Stock_Table::iterator pos = this->stocks_.find (stock);

    if (pos != this->stocks_.end ())
      {
        this->stocks_.erase (pos);
      }
    else
      {
        ACE_ERROR ((LM_ERROR,
                   "Distributor_exec_i::del_stock - Stock not present: %C\n",
                    stock));
      }
  }

  void
  Distributor_exec_i::start (void)
  {
    // calculate the interval time
    long const usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (0, usec),
                ACE_Time_Value (0, usec)) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Distributor_exec_i::start : error scheduling timer\n"));
    }
  }

  void
  Distributor_exec_i::stop (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    ACE_DEBUG ((LM_DEBUG, "Distributor_exec_i::stop\n"));
    delete this->ticker_;
  }

  // Component attributes.
  ::CORBA::ULong
  Distributor_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Distributor_exec_i::rate (
    ::CORBA::ULong rate )
  {
    this->rate_ = rate;
  }

  // Port operations.

  // Operations from Components::SessionComponent.
  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Distributor_exec_i::get_info_out_connector_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, "*************** DIST out connector status************************\n"));
    return ::CCM_DDS::CCM_ConnectorStatusListener::_nil ();
  }

  void
  Distributor_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    ::Quoter::CCM_Distributor_Context_var lctx =
      ::Quoter::CCM_Distributor_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (lctx.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }

    this->context_ = lctx;
  }

  void
  Distributor_exec_i::configuration_complete (void)
  {
  }

  void
  Distributor_exec_i::ccm_activate (void)
  {
    this->start ();
    this->add_stock ("MSFT");
    this->add_stock ("IBM");
    this->add_stock ("HP");
    this->add_stock ("DELL");
    this->add_stock ("ACER");
    this->add_stock ("ASUS");
  }

  void
  Distributor_exec_i::ccm_passivate (void)
  {
    this->stop ();
  }

  void
  Distributor_exec_i::ccm_remove (void)
  {
  }

  extern "C" DISTRIBUTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Quoter_Distributor_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Distributor_exec_i);

    return retval;
  }
}

