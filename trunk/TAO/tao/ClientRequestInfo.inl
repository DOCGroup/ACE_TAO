// -*- C++ -*-
//
// $Id$

#include "SystemException.h"

ACE_INLINE void
TAO_ClientRequestInfo::check_validity (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->invocation_ == 0)
    ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14,
                                     CORBA::COMPLETED_NO));
}

