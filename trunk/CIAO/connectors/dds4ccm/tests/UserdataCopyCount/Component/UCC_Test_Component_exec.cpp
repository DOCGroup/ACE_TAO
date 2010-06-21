// -*- C++ -*-
// $Id$

#include "UCC_Test_Component_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

#define NR_OF_KEYS 10

namespace CIAO_UCC_Test_UCCTestComponent_Impl
{
  size_t UCCVariableSizedStructTest_i::count_ = 0;
  size_t UCCVariableSizedStructTest_i::max_count_ = 0;
  size_t UCCFixedSizedStructTest_i::count_ = 0;
  size_t UCCFixedSizedStructTest_i::max_count_ = 0;

  //============================================================
  // WriteVariableHandler
  //============================================================
  WriteVariableHandler::WriteVariableHandler (Component_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  WriteVariableHandler::handle_timeout (const ACE_Time_Value &, const void *)
  {
    this->callback_.write_variable ();
    return 0;
  }

  //============================================================
  // WriteFixedHandler
  //============================================================
  WriteFixedHandler::WriteFixedHandler (Component_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  WriteFixedHandler::handle_timeout (const ACE_Time_Value &, const void *)
  {
    this->callback_.write_fixed ();
    return 0;
  }



  //============================================================
  // Component_exec_i
  //============================================================
  Component_exec_i::Component_exec_i (void)
  {
    this->var_rh_ = new WriteVariableHandler (*this);
    this->fixed_rh_ = new WriteFixedHandler (*this);
  }

  Component_exec_i::~Component_exec_i (void)
  {
    delete this->var_rh_;
    delete this->fixed_rh_;
  }

  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::UCC_Test::CCM_UCCTestComponent_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Component_exec_i::write_fixed (void)
  {
    ::UCCFixedSizeStruct_Test::UCCFixedSizedStructTestConnector::Writer_var fixed_writer
      = this->context_->get_connection_fixed_size_write_data ();

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->fixed_mutex_, CORBA::INTERNAL ());
    {
      for (CORBA::UShort i = 1; i < NR_OF_KEYS + 1; ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "Write fixed %i\n", i));
          fixed_writer->write_one (*(this->fixed_samples_[i]), ::DDS::HANDLE_NIL);
        }
    }
  }

  void
  Component_exec_i::write_variable (void)
  {
    ::UCCVariableSizedStruct_Test::UCCVariableSizedStructTestConnector::Writer_var var_writer
      = this->context_->get_connection_var_size_write_data ();

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->var_mutex_, CORBA::INTERNAL ());
    {
      for (CORBA::UShort i = 1; i < NR_OF_KEYS + 1; ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "Write variable %C\n", this->var_samples_[i]->symbol.in ()));
          var_writer->write_one (*(this->var_samples_[i]), ::DDS::HANDLE_NIL);
        }
    }
  }

  void
  Component_exec_i::create_fixed_sized_samples (void)
  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->fixed_mutex_, CORBA::INTERNAL ());
    ACE_DEBUG ((LM_DEBUG, "Component_exec_i::create_fixed_sized_samples - "
                          "Create fixed sized samples.\n"));
    for (::CORBA::UShort i = 1; i < NR_OF_KEYS + 1; ++i)
      {
        UCCFixedSizedStructTest_i* new_key = new UCCFixedSizedStructTest_i;
        new_key->x = i;
        new_key->y = new_key->x;
        new_key->z = 2*i;

        this->fixed_samples_[i] = new_key;
      }
  }

  void
  Component_exec_i::create_variable_sized_samples (void)
  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->var_mutex_, CORBA::INTERNAL ());
    ACE_DEBUG ((LM_DEBUG, "Component_exec_i::create_variable_sized_samples - "
                      "Create variable sized samples\n"));
    for (::CORBA::UShort i = 1; i < NR_OF_KEYS + 1; ++i)
      {
        UCCVariableSizedStructTest_i* new_key = new UCCVariableSizedStructTest_i;
        char key[7];
        ACE_OS::sprintf (key, "KEY_%d", i);
        new_key->symbol = CORBA::string_dup (key);
        new_key->x = i;
        new_key->y = new_key->x;
        this->var_samples_[i] = new_key;
      }
  }

  void
  Component_exec_i::configuration_complete (void)
  {
  }

  void
  Component_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Component_exec_i::ccm_activate - "
                          "Create samples.\n"));
    this->create_fixed_sized_samples ();
    this->create_variable_sized_samples ();

    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->var_rh_,
                0,
                ACE_Time_Value (2, 0),
                ACE_Time_Value (1, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Component_exec_i::ccm_activate : ")
                              ACE_TEXT ("Error scheduling timer for variable sized struct.\n")));
      }

    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->fixed_rh_,
                0,
                ACE_Time_Value (2, 0),
                ACE_Time_Value (1, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Component_exec_i::ccm_activate : ")
                              ACE_TEXT ("Error scheduling timer for fixed sized struct.\n")));
      }
  }

  void
  Component_exec_i::ccm_passivate (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->var_rh_);
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->fixed_rh_);
  }

  void
  Component_exec_i::ccm_remove (void)
  {
    for (::CORBA::UShort i = 1; i < NR_OF_KEYS + 1; ++i)
      {
        UCCVariableSizedStructTest_i* stype =this->var_samples_[i];
        delete stype;
      }

    for (::CORBA::UShort i = 1; i < NR_OF_KEYS + 1; ++i)
      {
        UCCFixedSizedStructTest_i* stype =this->fixed_samples_[i];
        delete stype;
      }

    this->var_samples_.clear ();
    this->fixed_samples_.clear ();
    
    // Check count_
    if (UCCFixedSizedStructTest_i::count_ != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Reference count for fixed sized "
                              "samples is not 0. expected <0> - found <%d>\n",
                              UCCFixedSizedStructTest_i::count_));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Reference count for fixed sized "
                              "samples is 0\n"));
      }
    if (UCCVariableSizedStructTest_i::count_ != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Reference count for variable sized "
                              "samples is not 0. expected <0> - found <%d>\n",
                              UCCVariableSizedStructTest_i::count_));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Reference count for variable sized "
                              "samples is 0\n"));
      }
      
    // Check max_count_, for each sample we allocated an instance so we
    // expect a count of 10
    if (UCCFixedSizedStructTest_i::max_count_ != 10)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Maximum refence count for fixed sized "
                              "samples is not ok, expected <10> - found <%d>\n",
                              UCCFixedSizedStructTest_i::max_count_));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Maximum reference count for fixed sized "
                              "samples is 10\n"));
      }
    if (UCCVariableSizedStructTest_i::max_count_ != 10)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Maximum reference count for variable sized "
                              "samples is not ok, expected <10> - found <%d>\n",
                              UCCVariableSizedStructTest_i::max_count_));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Maximum reference count for variable sized "
                              "samples is 10\n"));
      }
  }

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_UCCTest_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i);

    return retval;
  }
}

