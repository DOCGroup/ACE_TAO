#ifndef guard_object_const_reference_sequence_element_hpp
#define guard_object_const_reference_sequence_element_hpp
/**
 * @file
 *
 * @brief Implement the type returned by const operator[] in object_reference
 * sequences.
 *
 * $Id$
 *
 * @author Johnny Willemsen
 */
#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template<typename obj_ref_traits>
class object_reference_const_sequence_element
{
public:
  typedef typename obj_ref_traits::object_type object_reference_type;
  typedef object_reference_type * value_type;
  typedef object_reference_type const * const_value_type;
  typedef typename obj_ref_traits::object_type_var object_reference_var;

public:
  object_reference_const_sequence_element(
      value_type const & e, CORBA::Boolean release)
    : element_(&e)
    , release_(release)
  {
  }

  object_reference_const_sequence_element(
      object_reference_const_sequence_element const & rhs)
    : element_(rhs.element_)
    , release_(rhs.release_)
  {
  }

  ~object_reference_const_sequence_element()
  {
  }

  inline operator object_reference_type * (void) const
  {
    return *this->element_;
  }

  inline object_reference_type * in (void) const
  {
    return *this->element_;
  }

  inline value_type operator->() const
  {
    return *this->element_;
  }

  inline CORBA::Boolean release() const
  {
    return this->release_;
  }

private:
  // This function is not implemented
  object_reference_const_sequence_element();
  object_reference_const_sequence_element & operator=(
      object_reference_const_sequence_element const & rhs);

private:
  value_type const * const element_;
  CORBA::Boolean const release_;
};

} // namespace details
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_object_const_reference_sequence_element_hpp
