#ifndef guard_mock_reference_hpp
#define guard_mock_reference_hpp
/**
 * @file
 *
 * @brief Mock an object reference so we can test the sequences in
 *        isolation.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "ace/config-all.h"

#include "testing_counters.hpp"

// Forward declare the class a CORBA::release function for it.  That
// avoids having to introduce CORBA::Object into the tests.
// Ideally the T_var and T_out types should accept mock objects
// too, but that is too much to bite in the current iteration.
class mock_reference;
namespace CORBA
{
void release(mock_reference*);
}

#include "tao/Pseudo_VarOut_T.h"


/**
 * @class
 *
 * @brief Implement a concrete class with the right interface for an
 *        object reference.
 */
class mock_reference
{
public:
  virtual ~mock_reference();

  typedef TAO_Pseudo_Var_T<mock_reference> _var_type;
  typedef TAO_Pseudo_Out_T<mock_reference,_var_type> _out_type;

  static mock_reference * allocate(int id);
  static mock_reference * _nil();

  static call_counter duplicate_calls;
  static mock_reference * _duplicate(mock_reference * rhs);
  static call_counter release_calls;
  static void _tao_release(mock_reference * rhs);

  inline bool operator==(mock_reference const & rhs) const
  {
    return id_ == rhs.id_;
  }

  inline bool operator!=(mock_reference const & rhs) const
  {
    return !(*this == rhs);
  }

  inline int id() const
  {
    return id_;
  }

private:
  inline mock_reference(int id)
    : id_(id)
  {}

private:
  int id_;
};

typedef mock_reference * mock_reference_ptr;
typedef mock_reference::_var_type mock_reference_var;
typedef mock_reference::_out_type mock_reference_out;

#endif // guard_mock_reference_hpp
