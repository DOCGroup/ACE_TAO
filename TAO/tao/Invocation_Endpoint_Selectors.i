// -*- C++ -*-
// $Id$

// ****************************************************************

ACE_INLINE
TAO_Endpoint_Selector_Factory::TAO_Endpoint_Selector_Factory (void)
{
}

ACE_INLINE
TAO_Endpoint_Selector_Factory::~TAO_Endpoint_Selector_Factory (void)
{
}

// ****************************************************************

#if (TAO_HAS_RT_CORBA == 1)

ACE_INLINE
TAO_Endpoint_Selection_State::TAO_Endpoint_Selection_State (void)
  : priority_model_policy_ (0),
    client_protocol_policy_ (0),
    private_connection_ (0),
    bands_policy_ (0),
    client_protocol_index_ (0),
    valid_endpoint_found_ (0),
    client_priority_ (0),
    min_priority_ (0),
    max_priority_ (0)
{
}

#endif /* TAO_HAS_RT_CORBA == 1 */

// ****************************************************************

ACE_INLINE
TAO_Invocation_Endpoint_Selector::TAO_Invocation_Endpoint_Selector (void)
{
}


// ****************************************************************

ACE_INLINE
TAO_Default_Endpoint_Selector::TAO_Default_Endpoint_Selector (void)
{
}
