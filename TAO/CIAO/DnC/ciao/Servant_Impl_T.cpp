// $Id$

#ifndef CIAO_SERVANT_IMPL_T_C
#define CIAO_SERVANT_IMPL_T_C

#include "Servant_Impl_T.h"

namespace CIAO
{
  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR, 
            typename CONTEXT>
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::Servant_Impl (
      EXEC * exe, 
      Session_Container * c
    )
    : Servant_Impl_Base (c),
      executor_ (EXEC::_duplicate (exe))
  {
  }
  
  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR, 
            typename CONTEXT>
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::~Servant_Impl (void)
  {
  }
  
  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR, 
            typename CONTEXT>
  ::Components::FacetDescriptions *
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::get_all_facets (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::FacetDescriptions *tmp = 0;
    ACE_NEW_RETURN (tmp,
                    ::Components::FacetDescriptions,
                    0);
                    
    ::Components::FacetDescriptions_var retval = tmp;

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
  
  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR, 
            typename CONTEXT>
  ::Components::ConsumerDescriptions *
  Servant_Impl<BASE_SKEL, EXEC, EXEC_VAR, CONTEXT>::get_all_consumers (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::ConsumerDescriptions *tmp = 0;
    ACE_NEW_RETURN (tmp,
                    ::Components::ConsumerDescriptions,
                    0);
                    
    ::Components::ConsumerDescriptions_var retval = tmp;

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
