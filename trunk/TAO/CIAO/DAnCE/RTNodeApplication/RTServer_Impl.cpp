// $Id$

#include "RTServer_Impl.h"
#include "ciao/CIAO_common.h"

#if !defined (__ACE_INLINE__)
# include "RTServer_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::RTServer::RTNodeApplication_Impl::~RTNodeApplication_Impl ()
{
}

CORBA::Long
CIAO::RTServer::RTNodeApplication_Impl::init (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->CIAO::NodeApplication_Impl::init (ACE_ENV_SINGLE_ARG_PARAMETER);

  // We will probably need two ORBs in this process.  One for the
  // deployment framework, and one for the actual components.
  return 0;
}

