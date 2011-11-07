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

ORB_Holder::ORB_Holder (int &argc, ACE_TCHAR *argv[],
                        const char *orb_id)
  :  orb_ (CORBA::ORB_init (argc, argv, orb_id))
{
}

ORB_Holder::~ORB_Holder (void)
{
  try{
    this->orb_->destroy ();
  } catch (const CORBA::Exception&) {
    // @@ TODO Log this event, check the Servant_var.cpp comments for
    // details.
  }
}
