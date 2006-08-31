// $Id$

#if !defined (__CUTS_INLINE__)
# include "cuts/CCM_Events_T.inl"
#endif

//=============================================================================
/**
 * CUTS_Event_T
 */
//=============================================================================

//
// CUTS_Event_T
//
template <typename EVENTTYPE>
CUTS_Event_T <EVENTTYPE>::CUTS_Event_T (void)
{

}

//
// ~CUTS_Event_T
//
template <typename EVENTTYPE>
CUTS_Event_T <EVENTTYPE>::~CUTS_Event_T (void)
{

}

//=============================================================================
/**
 * CUTS_Event_init_T
 */
//=============================================================================

//
// CUTS_Event_init_T
//
template <typename EVENTTYPE>
CUTS_Event_init_T <EVENTTYPE>::CUTS_Event_init_T (void)
{

}

//
// ~CUTS_Event_init_T
//
template <typename EVENTTYPE>
CUTS_Event_init_T <EVENTTYPE>::~CUTS_Event_init_T (void)
{

}

//
// create_for_unmarshal
//
template <typename EVENTTYPE>
CORBA::ValueBase *
CUTS_Event_init_T <EVENTTYPE>::create_for_unmarshal (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ValueBase *ret_val = 0;
  ACE_NEW_THROW_EX (ret_val, EVENTTYPE, ::CORBA::NO_MEMORY ());
  return ret_val;
}
