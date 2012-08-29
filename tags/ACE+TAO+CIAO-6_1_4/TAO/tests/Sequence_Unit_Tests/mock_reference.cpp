/**
 * @file
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "mock_reference.hpp"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

call_counter mock_reference::duplicate_calls;
call_counter mock_reference::release_calls;
call_counter mock_reference::serialize_calls;
call_counter mock_reference::deserialize_calls;
call_counter mock_reference::marshal_calls;

mock_reference::
~mock_reference()
{
}

mock_reference * mock_reference::
allocate(int id)
{
  return new mock_reference(id);
}

mock_reference * mock_reference::
_nil()
{
  return 0;
}

mock_reference * mock_reference::
_duplicate(mock_reference * rhs)
{
  duplicate_calls();
  if (rhs == 0)
  {
    return 0;
  }
  return new mock_reference(*rhs);
}

void mock_reference::
_tao_release(mock_reference * rhs)
{
  release_calls();
  delete rhs;
}

void CORBA::release(mock_reference * r)
{
  mock_reference::_tao_release(r);
}

mock_reference *
TAO::Objref_Traits<mock_reference>::duplicate (
    mock_reference_ptr p
  )
{
  return mock_reference::_duplicate (p);
}

void
TAO::Objref_Traits<mock_reference>::release (
    mock_reference_ptr p
  )
{
  CORBA::release (p);
}

mock_reference_ptr
TAO::Objref_Traits<mock_reference>::nil (void)
{
  return mock_reference::_nil ();
}

CORBA::Boolean
TAO::Objref_Traits<mock_reference>::marshal (
    const mock_reference_ptr,
    TAO_OutputCDR &
  )
{
  mock_reference::marshal_calls ();
  return true;
}

CORBA::Boolean operator<< (TAO_OutputCDR &, const mock_reference *)
{
  mock_reference::serialize_calls ();
  return true;
}
CORBA::Boolean operator>> (TAO_InputCDR &, mock_reference *&)
{
  mock_reference::deserialize_calls ();
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
