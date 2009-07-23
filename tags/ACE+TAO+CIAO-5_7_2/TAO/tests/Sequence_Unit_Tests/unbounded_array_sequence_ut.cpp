/**
 * @file
 *
 * @brief Unit test for unbounded sequences of arrays.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "mock_array.hpp"
#include "testing_allocation_traits.hpp"
#include "testing_range_checking.hpp"

#include "tao/Unbounded_Array_Sequence_T.h"

#define FAIL_RETURN_IF(CONDITION) \
          if (CONDITION) \
      { \
              ACE_DEBUG ((LM_ERROR, ACE_TEXT ("\tFailed at %N:%l\n"))); \
        return 1; \
      }

typedef unbounded_array_sequence<my_array, my_array_slice, my_array_tag> tested_sequence;
typedef tested_sequence::value_type value_type;
typedef tested_sequence::const_value_type const_value_type;

typedef tested_sequence::element_traits tested_element_traits;
typedef tested_sequence::allocation_traits tested_allocation_traits;
typedef TAO::details::range_checking<value_type,true> range;

int test_default_constructor()
  {
    {
      tested_sequence x;

      FAIL_RETURN_IF(CORBA::ULong(0), x.maximum());
      FAIL_RETURN_IF(CORBA::ULong(0), x.length());
      FAIL_RETURN_IF(true, x.release());
    }
  }

int ACE_TMAIN(int,ACE_TCHAR*[])
{
  int status = 0;

  status += test_default_constructor ();

  return status;
}
