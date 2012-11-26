#ifndef guard_string_sequence_element_hpp
#define guard_string_sequence_element_hpp
/**
 * @file
 *
 * @brief Implement the type returned by operator[] in string
 * sequences.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "tao/Basic_Types.h"

#include <algorithm>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template<typename traits>
class string_sequence_element
{
public:
  typedef typename traits::char_type character_type;
  typedef character_type * value_type;
  typedef character_type const * const_value_type;
  typedef typename traits::string_var string_var;
  typedef typename traits::string_out string_out;
  typedef typename traits::string_mgr string_mgr;

private:
  inline string_sequence_element<traits> & pseudo_copy_swap(string_var & rhs)
  {
    if (release())
      {
        traits::release(*element_);
      }
    *element_ = rhs._retn();
    return *this;
  }

public:
  string_sequence_element(
      value_type & e, CORBA::Boolean release)
    : element_(&e)
    , release_(release)
  {
  }

  string_sequence_element(
      string_sequence_element const & rhs)
    : element_(rhs.element_)
    , release_(rhs.release_)
  {
  }

  ~string_sequence_element()
  {
  }

  string_sequence_element & operator=(const_value_type rhs)
  {
    string_var tmp(rhs);
    return pseudo_copy_swap(tmp);
  }

  string_sequence_element & operator=(value_type rhs)
  {
    string_var tmp(rhs);
    return pseudo_copy_swap(tmp);
  }

  string_sequence_element & operator=(string_sequence_element const & rhs)
  {
    string_var tmp(static_cast<const_value_type>(rhs));
    return pseudo_copy_swap(tmp);
  }

  string_sequence_element & operator=(string_var const & rhs)
  {
    string_var tmp(rhs);
    return pseudo_copy_swap(tmp);
  }

  string_sequence_element & operator=(string_mgr const & rhs)
  {
    string_var tmp(rhs.in());
    return pseudo_copy_swap(tmp);
  }

  inline operator const_value_type() const
  {
    return *this->element_;
  }

  inline const character_type *in (void) const {
    return *this->element_;
  }

  inline character_type *&inout (void) const {
    return *this->element_;
  }

  inline string_out out (void) const {

    if (release())
      {
        traits::release(*element_);
      }

    return *this->element_;
  }

  inline character_type *_retn (void) {
    character_type * copy = *this->element_;
    *this->element_ = traits::default_initializer();
    return copy;
  }

  void swap(string_sequence_element & rhs)
  {
    std::swap(element_, rhs.element_);
    std::swap(release_, rhs.release_);
  }

  CORBA::Boolean release() const
  {
    return this->release_;
  }

private:
  // This function is not implemented
  string_sequence_element();

private:
  value_type * element_;
  CORBA::Boolean release_;
};

} // namespace details
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_string_sequence_element_hpp
