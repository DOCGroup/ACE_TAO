#ifndef guard_object_reference_sequence_element_hpp
#define guard_object_reference_sequence_element_hpp
/**
 * @file
 *
 * @brief Implement the type returned by operator[] in object_reference
 * sequences.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Basic_Types.h"

namespace TAO
{
namespace details
{

template<typename traits>
class object_reference_sequence_element
{
public:
  typedef typename traits::object_type object_reference_type;
  typedef object_reference_type * value_type;
  typedef object_reference_type const * const_value_type;
  typedef typename traits::object_type_var object_reference_var;

private:
  inline object_reference_sequence_element<traits> & pseudo_copy_swap(
      object_reference_var & rhs)
  {
    if (release())
    {
      traits::release(*element_);
    }
    *element_ = rhs._retn();
    return *this;
  }

public:
  object_reference_sequence_element(
      value_type & e, CORBA::Boolean release)
    : element_(&e)
    , release_(release)
  {
  }

  object_reference_sequence_element(
      object_reference_sequence_element const & rhs)
    : element_(rhs.element_)
    , release_(rhs.release_)
  {
  }

  ~object_reference_sequence_element()
  {
  }

  object_reference_sequence_element & operator=(
      object_reference_var const & rhs)
  {
    object_reference_var tmp(rhs);
    return pseudo_copy_swap(tmp);
  }

  object_reference_sequence_element & operator=(
      object_reference_sequence_element const & rhs)
  {
    object_reference_var tmp(traits::duplicate(*rhs.element_));
    return pseudo_copy_swap(tmp);
  }

  object_reference_sequence_element & operator=(value_type rhs)
  {
    object_reference_var tmp(rhs);
    return pseudo_copy_swap(tmp);
  }

  inline operator value_type()
  {
    return *element_;
  }

  inline operator const_value_type() const
  {
    return *element_;
  }

  inline value_type operator->()
  {
    return *element_;
  }

  inline value_type operator->() const
  {
    return *element_;
  }

  void swap(object_reference_sequence_element & rhs)
  {
    std::swap(element_, rhs.element_);
    std::swap(release_, rhs.release_);
  }

  inline CORBA::Boolean release() const
  {
    return release_;
  }

private:
  // This function is not implemented
  object_reference_sequence_element();

private:
  value_type * element_;
  CORBA::Boolean release_;
};

} // namespace details
} // namespace CORBA

#endif // guard_object_reference_sequence_element_hpp
