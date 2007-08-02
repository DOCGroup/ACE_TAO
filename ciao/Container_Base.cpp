// $Id$

#include "Container_Base.h"
#include "CIAO_common.h"

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

  void
  Container::set_receptacle_policy_map (::CIAO::REC_POL_MAP &rec_pol_map)
  {
    for (::CIAO::REC_POL_MAP_ITERATOR it = rec_pol_map.begin ();
         it != rec_pol_map.end (); ++it)
      {
        this->rec_pol_map_.bind ((*it).ext_id_, (*it).int_id_);
      }
  }

  CORBA::PolicyList
  Container::get_receptacle_policy (const char* name)
  {
    CORBA::PolicyList policy_list;
    if (this->rec_pol_map_.find (name, policy_list) != 0)
      {
        ACE_DEBUG ((LM_DEBUG, "Unable to find policies "
                    "for the receptacle %s\n", name));
        policy_list.length (0);
      }
    return policy_list;
  }

  PortableServer::POA_ptr
  Container::the_POA (void) const
  {
    return this->component_poa_.in ();
  }

  PortableServer::POA_ptr
  Container::the_facet_cons_POA (void) const
  {
    return this->facet_cons_poa_.in ();
  }

  CORBA::ORB_ptr
  Container::the_ORB (void) const
  {
    return this->orb_.in ();
  }
}
