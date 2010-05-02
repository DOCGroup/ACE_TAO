// -*- C++ -*-
// $Id$
#ifndef CIAO_NONCHANGEABLE_CONNECTOR_EXEC_H_
#define CIAO_NONCHANGEABLE_CONNECTOR_EXEC_H_

#include "NonChangeable_Connector_conn.h"

namespace CIAO_NonChangeable_NonChangeable_Connector_Impl
{
  class NONCHANGEABLE_CONNECTOR_CONN_Export NonChangeable_Connector_exec_impl
    : public virtual NonChangeable_Connector_exec_i
  {
  public:
    NonChangeable_Connector_exec_impl (void);
    virtual ~NonChangeable_Connector_exec_impl (void);

    virtual void ccm_activate (void);

  private:
    bool test_topic_name (void);
    bool test_key_fields (void);
    bool test_domain_id (void);
    bool test_qos_profile (void);
  };

  extern "C" NONCHANGEABLE_CONNECTOR_CONN_Export ::Components::EnterpriseComponent_ptr
  create_NonChangeable_NonChangeable_Connector_i (void);
}

 #endif /* CIAO_NONCHANGEABLE_CONNECTOR_EXEC_H_ */
