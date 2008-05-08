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

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
// Forward declare the class a CORBA::release function for it.  That
// avoids having to introduce CORBA::Object into the tests.
// Ideally the T_var and T_out types should accept mock objects
// too, but that is too much to bite in the current iteration.
class mock_reference;
namespace CORBA
{
void release(mock_reference*);
}

class mock_stream;

TAO_END_VERSIONED_NAMESPACE_DECL

#include "tao/Objref_VarOut_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef mock_reference *mock_reference_ptr;
typedef TAO_Objref_Var_T<mock_reference> mock_reference_var;
typedef TAO_Objref_Out_T<mock_reference> mock_reference_out;

/**
 * @class mock_reference
 *
 * @brief Implement a concrete class with the right interface for an
 *        object reference.
 */
class mock_reference
{
public:
  virtual ~mock_reference();

  typedef mock_reference_var _var_type;
  typedef mock_reference_out _out_type;

  static mock_reference * allocate(int id);
  static mock_reference * _nil();

  static mock_reference * _duplicate(mock_reference * rhs);
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
  mock_reference ();

  inline mock_reference(int id)
    : id_(id)
  {}

private:
  int id_;
};

CORBA::Boolean operator<< (TAO_OutputCDR &, const mock_reference *);
CORBA::Boolean operator>> (TAO_InputCDR &, mock_reference *&);

namespace TAO
{
  template<>
  struct Objref_Traits< mock_reference>
  {
    static mock_reference_ptr duplicate (mock_reference_ptr);
    static void release (mock_reference_ptr);
    static mock_reference_ptr nil (void);
    static CORBA::Boolean marshal (const mock_reference_ptr p, TAO_OutputCDR & cdr);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL
#endif // guard_mock_reference_hpp
