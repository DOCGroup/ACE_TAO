/**
 * @file POA_Destroyer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "POA_Destroyer.h"

#if !defined(__ACE_INLINE__)
#include "POA_Destroyer.inl"
#endif /* __ACE_INLINE__ */

POA_Destroyer::~POA_Destroyer ()
{
  if (CORBA::is_nil (this->poa_.in ()))
    return;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY {
    this->poa_->destroy (1, 1
                         TAO_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  } ACE_CATCHANY {
    // @@ This event should be logged...
  } ACE_ENDTRY;
}
