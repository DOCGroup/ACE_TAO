// $Id$

//=============================================================================
/**
 * CUTS_Event_T
 */
//=============================================================================

template <typename EVENTTYPE>
CUTS_INLINE
::CORBA::Long CUTS_Event_T <EVENTTYPE>::event_id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
ACE_THROW_SPEC ((::CORBA::SystemException))
{
  return CUTS_Event_Info <EVENTTYPE>::event_id_;
}

//=============================================================================
/**
 * CUTS_Event_init_T
 */
//=============================================================================

//
// _downcast
//
template <typename EVENTTYPE>
CUTS_INLINE 
CUTS_Event_init_T <EVENTTYPE> * 
CUTS_Event_init_T <EVENTTYPE>::_downcast (CORBA::ValueFactoryBase * v)
{
	return dynamic_cast < CUTS_Event_init_T <EVENTTYPE> * > (v);
}

//
// tao_repository_id
//
template <typename EVENTTYPE>
CUTS_INLINE
const char * CUTS_Event_init_T <EVENTTYPE>::tao_repository_id (void)
{
  return Event_Type::_tao_obv_static_repository_id ();
}

