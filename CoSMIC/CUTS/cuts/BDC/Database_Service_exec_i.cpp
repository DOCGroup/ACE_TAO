// $Id$

#include "Database_Service_exec_i.h"
#include "BDC_Task_Ex.h"
#include "cuts/svcs/dbase/DBase_Service.h"

namespace CUTS
{
  //
  // Database_Service_exec_i
  //
  Database_Service_exec_i::Database_Service_exec_i (
    CUTS_Database_Service &dbase_svc)
  : dbase_svc_ (dbase_svc)
  {

  }

  //
  // ~Database_Service_exec_i
  //
  Database_Service_exec_i::~Database_Service_exec_i (void)
  {

  }

  //
  // create_new_test
  //
  CORBA::Long Database_Service_exec_i::create_new_test (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    int test = 0;

    if (this->dbase_svc_.create_new_test ())
    {
      test = this->dbase_svc_.current_test ();
    }

    return test;
  }

  //
  // stop_current_test
  //
  void Database_Service_exec_i::stop_current_test (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->dbase_svc_.stop_current_test ();
  }

  //
  // current_test
  //
  CORBA::Long Database_Service_exec_i::current_test (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->dbase_svc_.current_test ();
  }
}
