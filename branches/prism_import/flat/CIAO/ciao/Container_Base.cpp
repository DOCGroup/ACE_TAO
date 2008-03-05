// $Id$

#include "Container_Base.h"
#include "CIAO_common.h"

#if !defined (__ACE_INLINE__)
# include "Container_Base.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{
  ////////////////////////////////////////////////////////////////

  Container::Container (CORBA::ORB_ptr o)
    : orb_ (CORBA::ORB::_duplicate (o)),
      container_impl_ (0)
  {
  }

  Container::Container (CORBA::ORB_ptr o, Container_Impl *container_impl)
    : orb_ (CORBA::ORB::_duplicate (o)),
      container_impl_ (container_impl)
  {
  }

  Container::~Container (void)
  {
  }

  CORBA::PolicyList
  Container::get_receptacle_policy (const char* name)
  {
    CORBA::PolicyList policy_list;
    
    if (this->rec_pol_map_.find (name, policy_list) != 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "Unable to find policies "
                    "for the receptacle %s\n",
                    name));
        policy_list.length (0);
      }
      
    return policy_list;
  }
}
