#ifndef guard_fwd_mock_reference_hpp
#define guard_fwd_mock_reference_hpp
/**
 * @file
 *
 * @brief Mock an forward declared object reference so we can test the sequences
 *        in isolation.
 *
 * $Id$
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
#include "ace/config-all.h"

#include "testing_counters.hpp"

#include "tao/Objref_VarOut_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class fwd_mock_reference;
typedef fwd_mock_reference * fwd_mock_reference_ptr;
typedef TAO_Objref_Var_T<fwd_mock_reference> fwd_mock_reference_var;
typedef TAO_Objref_Out_T<fwd_mock_reference> fwd_mock_reference_out;

namespace TAO
{
template<>
struct Objref_Traits< fwd_mock_reference>
{
  static fwd_mock_reference_ptr duplicate (fwd_mock_reference_ptr);
  static void release (fwd_mock_reference_ptr);
  static fwd_mock_reference_ptr nil (void);
  static CORBA::Boolean marshal (const fwd_mock_reference_ptr p, TAO_OutputCDR & cdr);
};
}

TAO_END_VERSIONED_NAMESPACE_DECL
#endif // guard_fwd_mock_reference_hpp
