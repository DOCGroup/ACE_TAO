/**
 * @file EC_Destroyer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "EC_Destroyer.h"

#if !defined(__ACE_INLINE__)
#include "EC_Destroyer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO_PERF_RTEC, EC_Destroyer, "$Id$")

EC_Destroyer::EC_Destroyer (RtecEventChannelAdmin::EventChannel_ptr ec)
  :  ec_ (RtecEventChannelAdmin::EventChannel::_duplicate (ec))
{
}

EC_Destroyer::~EC_Destroyer (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY {
    this->ec_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  } ACE_CATCH (CORBA::Exception, ex) {
    // @@ TODO Log this event, check the Servant_var.cpp comments for
    // details.
  } ACE_ENDTRY;
}
