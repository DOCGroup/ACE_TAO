// $Id$

#ifndef CIAO_SERVANT_IMPL_T_C
#define CIAO_SERVANT_IMPL_T_C

#include "Servant_Impl_T.h"

namespace CIAO
{
  template <typename SKEL, typename EXEC, typename CONTEXT>
  Servant_Impl<SKEL, EXEC, CONTEXT>::Servant_Impl (
      typename EXEC::_ptr_type exe, 
      Session_Container * c
    )
    : Servant_Impl_Base (c),
      executor_ (EXEC::_duplicate (exe))
  {
  }
  
  template <typename SKEL, typename EXEC, typename CONTEXT>
  Servant_Impl<SKEL, EXEC, CONTEXT>::~Servant_Impl (void)
  {
  }
  
  template <typename SKEL, typename EXEC, typename CONTEXT>
  ::Components::FacetDescriptions *
  Servant_Impl<SKEL, EXEC, CONTEXT>::get_all_facets (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::FacetDescriptions *fd_ptr = 0;
    ACE_NEW_RETURN (fd_ptr,
                    ::Components::FacetDescriptions,
                    0);
    ::Components::FacetDescriptions_var retval = fd_ptr;
                    
    retval->length (this->facet_table_.current_size ());
    CORBA::ULong i = 0;
    
    for (FacetTable::iterator iter = this->facet_table_.begin ();
         iter != this->facet_table_.end ();
         ++iter, ++i)
      {
        FacetTable::ENTRY & entry = *iter;
        retval[i] = entry.int_id_;
      }
    
    return retval._retn ();
  }
  
  template <typename SKEL, typename EXEC, typename CONTEXT>
  ::Components::ConsumerDescriptions *
  Servant_Impl<SKEL, EXEC, CONTEXT>::get_all_consumers (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::ConsumerDescriptions *cd_ptr = 0;
    ACE_NEW_RETURN (cd_ptr,
                    ::Components::ConsumerDescriptions,
                    0);
    ::Components::ConsumerDescriptions_var retval = cd_ptr;
                    
    retval->length (this->consumer_table_.current_size ());
    CORBA::ULong i = 0;
    
    for (ConsumerTable::iterator iter = this->consumer_table_.begin ();
         iter != this->consumer_table_.end ();
         ++iter, ++i)
      {
        ConsumerTable::ENTRY & entry = *iter;
        retval[i] = entry.int_id_;
      }
    
    return retval._retn ();
  }
}

#endif /* CIAO_SERVANT_IMPL_T_C */
