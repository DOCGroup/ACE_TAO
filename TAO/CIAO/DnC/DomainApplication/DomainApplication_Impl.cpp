// $Id$

#include "DomainApplication_Impl.h"

//===============================================================
//============   DomainApplication_Impl  =================
//===============================================================

CIAO::DomainApplication_Impl::DomainApplication_Impl ()
{
  // @@ 
}

CIAO::DomainApplication_Impl::~DomainApplication_Impl ()
{
  // @@ remove all NodeApplication's?
}




virtual void 
CIAO::DomainApplication_Impl::
finishLaunch (const Deployment::Connections & providedReference,
              CORBA::Boolean start
              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError,
                   Deployment::InvalidConnection))
{
  /**
   *  1. Call finishLaunch operation on each individual NodeApplication.
   *  2. ...
   */
  ACE_TRY
    {

    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (0);

  return;
}


virtual void 
CIAO::DomainApplication_Impl::
start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, Deployment::StartError))
{

}

