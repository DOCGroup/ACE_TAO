/**
 * @file
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "mock_reference.hpp"

call_counter mock_reference::duplicate_calls;
call_counter mock_reference::release_calls;

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
