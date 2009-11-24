// -*- C++ -*-
// $Id$

#include "NonChangeable_Connector_exec.h"

namespace CIAO_NonChangeable_NonChangeable_Connector_Impl
{
  NonChangeable_Connector_exec_i::NonChangeable_Connector_exec_i (void)
    : DDS_Event_Connector_T<NonChangeableTest_DDS_Traits, NonChangeableTest_Connector_Traits> ()
  {
  }

  NonChangeable_Connector_exec_i::~NonChangeable_Connector_exec_i (void)
  {
  }

  void
  NonChangeable_Connector_exec_i::ccm_activate ()
  {
    bool nonchangeable_caught = false;
    try
      {
        this->topic_name ("test");
      }
    catch (CCM_DDS::NonChangeable)
      {
        CIAO_DEBUG ((LM_DEBUG,
          ACE_TEXT ("NonChangeable_Connector_exec_i::ccm_activate : Expected exception caught\n")));
        nonchangeable_caught = true;
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, 
          ACE_TEXT ("ERROR: Unkown exception caught\n")));
      }
    if (!nonchangeable_caught)
      {
        CIAO_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: No NonChangeable exception caught\n")));
      }
  }
  
  extern "C" NONCHANGEABLE_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_NonChangeable_NonChangeable_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      NonChangeable_Connector_exec_i ());

    return retval;
  }
}

