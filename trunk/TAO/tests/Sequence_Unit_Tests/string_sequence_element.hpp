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
  typedef typename traits::string_mgr string_mgr;

private:
  inline string_sequence_element<traits> & pseudo_copy_swap(
      string_var & rhs)
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
    return *element_;
  }

  void swap(string_sequence_element & rhs)
  {
    std::swap(element_, rhs.element_);
    std::swap(release_, rhs.release_);
  }

  CORBA::Boolean release() const
  {
    return release_;
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

#endif // guard_string_sequence_element_hpp
