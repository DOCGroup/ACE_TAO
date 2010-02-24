// -*- C++ -*-
// $Id$

#include "QueryFilter_Test_Connector_exec.h"

namespace CIAO_QueryFilter_Test_QueryFilter_Test_Connector_Impl
{
  QueryFilter_Test_Connector_exec_i::QueryFilter_Test_Connector_exec_i (void)
    : DDS_Event_Connector_T<QueryFilterTest_DDS_Traits, QueryFilterTest_Connector_Traits> ()
  {
  }

  QueryFilter_Test_Connector_exec_i::~QueryFilter_Test_Connector_exec_i (void)
  {
  }

  extern "C" QUERY_FILTER_TEST_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QueryFilter_Test_QueryFilter_Test_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      QueryFilter_Test_Connector_exec_i ());

    return retval;
  }
}

