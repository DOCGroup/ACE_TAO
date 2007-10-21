// -*- C++ -*-
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_AMH_DSI_Response_Handler_ptr
TAO_AMH_DSI_Response_Handler::_duplicate (TAO_AMH_DSI_Response_Handler_ptr x)
{
  if (x != 0)
    {
      x->_add_ref ();
    }

  return x;
}

TAO_END_VERSIONED_NAMESPACE_DECL
