/**
 * @file
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "mock_stream.hpp"

call_counter mock_stream::serialize_calls;
call_counter mock_stream::deserialize_calls;

mock_stream::
~mock_stream()
{
}

CORBA::ULong
mock_stream::length () const
{
  return 100;
}

CORBA::Boolean operator<< (mock_stream &, const CORBA::ULong)
{
  return true;
}
CORBA::Boolean operator>> (mock_stream &, CORBA::ULong &)
{
  return true;
}

