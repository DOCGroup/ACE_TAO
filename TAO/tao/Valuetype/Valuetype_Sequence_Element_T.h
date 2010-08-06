#ifndef guard_valuetype_sequence_element_hpp
#define guard_valuetype_sequence_element_hpp
/**
 * @file
 *
 * @brief Implement the type returned by operator[] in valuetype
 * sequences.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template<typename obj_ref_traits>
class valuetype_sequence_element
{
public:
  typedef typename obj_ref_traits::object_type valuetype_type;
  typedef valuetype_type * value_type;
  typedef valuetype_type const * const_value_type;
  typedef typename obj_ref_traits::object_type_var valuetype_var;

private:
  inline valuetype_sequence_element<obj_ref_traits> & pseudo_copy_swap(
      valuetype_var & rhs)
  {
    if (release())
    {
      obj_ref_traits::release(*element_);
    }
    *element_ = rhs._retn();
    return *this;
  }

public:
  valuetype_sequence_element(
      value_type & e, CORBA::Boolean release)
    : element_(&e)
    , release_(release)
  {
  }

  valuetype_sequence_element(
      valuetype_sequence_element const & rhs)
    : element_(rhs.element_)
    , release_(rhs.release_)
  {
  }

  ~valuetype_sequence_element()
  {
  }

  valuetype_sequence_element & operator=(
      valuetype_var const & rhs)
  {
    valuetype_var tmp(rhs);
    return pseudo_copy_swap(tmp);
  }

  valuetype_sequence_element & operator=(
      valuetype_sequence_element const & rhs)
  {
    valuetype_var tmp(obj_ref_traits::duplicate(*rhs.element_));
    return pseudo_copy_swap(tmp);
  }

  valuetype_sequence_element & operator=(value_type rhs)
  {
    valuetype_var tmp(rhs);
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

  void swap(valuetype_sequence_element & rhs)
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
  valuetype_sequence_element();

private:
  value_type * element_;
  CORBA::Boolean release_;
};

} // namespace details
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_valuetype_sequence_element_hpp
