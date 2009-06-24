// $Id$

#include "Container_Base.h"
#include "ciao/CIAO_common.h"

#if !defined (__ACE_INLINE__)
# include "Container_Base.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{
  ////////////////////////////////////////////////////////////////

  Container_i::Container_i (CORBA::ORB_ptr o)
    : orb_ (CORBA::ORB::_duplicate (o)),
      container_impl_ (0)
  {
  }

  Container_i::Container_i (CORBA::ORB_ptr o,
                            Deployment::CIAO_Container_i *container_impl)
    : orb_ (CORBA::ORB::_duplicate (o)),
      container_impl_ (container_impl)
  {
  }

  Container_i::~Container_i (void)
  {
  }

  CORBA::PolicyList *
  Container_i::get_receptacle_policy (const char* name)
  {
    CORBA::PolicyList_var policy_list;

    ACE_NEW_THROW_EX (policy_list,
                      CORBA::PolicyList (),
                      CORBA::NO_MEMORY ());

    if (this->rec_pol_map_.find (name, policy_list) != 0)
      {
        CIAO_ERROR ((LM_DEBUG, CLINFO
                    "Container_i::get_receptacle_policy - No policies found "
                    "for the receptacle %C\n",
                    name));
      }

    return policy_list._retn ();
  }
}
