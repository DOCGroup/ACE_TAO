/**
 * @file ORB_Holder.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "ORB_Holder.h"

#if !defined(__ACE_INLINE__)
#include "ORB_Holder.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO_PERF_RTEC, ORB_Holder, "$Id$")

ORB_Holder::ORB_Holder (int &argc, char *argv[],
                        const char *orb_id
                        ACE_ENV_ARG_DECL)
  :  orb_ (CORBA::ORB_init (argc, argv, orb_id
                            ACE_ENV_ARG_PARAMETER))
{
}

ORB_Holder::~ORB_Holder (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY {
    this->orb_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  } ACE_CATCH (CORBA::Exception, ex) {
    // @@ TODO Log this event, check the Servant_var.cpp comments for
    // details.
  } ACE_ENDTRY;
}
