// $Id$

namespace CIAO
{
  ACE_INLINE PortableServer::POA_ptr
  Container::the_POA (void) const
  {
    return this->component_poa_.in ();
  }

  ACE_INLINE PortableServer::POA_ptr
  Container::the_facet_cons_POA (void) const
  {
    return this->facet_cons_poa_.in ();
  }

  ACE_INLINE CORBA::ORB_ptr
  Container::the_ORB (void) const
  {
    return this->orb_.in ();
  }

  ACE_INLINE void
  Container::set_receptacle_policy_map (::CIAO::REC_POL_MAP &rec_pol_map)
  {
    for (::CIAO::REC_POL_MAP_ITERATOR it = rec_pol_map.begin ();
         it != rec_pol_map.end ();
         ++it)
      {
        this->rec_pol_map_.bind ((*it).ext_id_, (*it).int_id_);
      }
  }
}
