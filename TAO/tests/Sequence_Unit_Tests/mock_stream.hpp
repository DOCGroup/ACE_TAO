#ifndef guard_mock_stream_hpp
#define guard_mock_stream_hpp
/**
 * @file
 *
 * @brief Mock an stream
 *
 * $Id$
 *
 * @author Johnny Willemsen
 */
#include "ace/config-all.h"

#include "testing_counters.hpp"
#include "tao/Basic_Types.h"

/**
 * @class mock_stream
 *
 * @brief Implement a concrete class with the right interface for an
 *        object reference.
 */
class mock_stream
{
public:
  virtual ~mock_stream();

  static call_counter serialize_calls;
  static call_counter deserialize_calls;
};

CORBA::Boolean operator<< (mock_stream &, const CORBA::ULong);
CORBA::Boolean operator>> (mock_stream &, CORBA::ULong &);

#endif // guard_mock_stream_hpp
