#ifndef guard_string_const_sequence_element_hpp
#define guard_string_const_sequence_element_hpp
/**
 * @file
 *
 * @brief Implement the type returned by const operator[] in string
 * sequences.
 *
 * $Id$
 *
 * @author Carlos O'Ryan and Johnny Willemsen
 */

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

template<typename traits>
class string_const_sequence_element
{
public:
  typedef typename traits::char_type character_type;
  typedef character_type * value_type;
  typedef character_type const * const_value_type;
  typedef typename traits::string_var string_var;
  typedef typename traits::string_mgr string_mgr;

public:
  string_const_sequence_element(value_type const & e, CORBA::Boolean release)
    : element_(&e)
    , release_(release)
  {
  }

  string_const_sequence_element(
      string_const_sequence_element const & rhs)
    : element_(rhs.element_)
    , release_(rhs.release_)
  {
  }

  ~string_const_sequence_element()
  {
  }

  inline operator const_value_type() const
  {
    return *this->element_;
  }

  inline const character_type *in (void) const
  {
    return *this->element_;
  }

  CORBA::Boolean release() const
  {
    return this->release_;
  }

private:
  // This function is not implemented
  string_const_sequence_element();
  string_const_sequence_element & operator=(string_const_sequence_element const & rhs);

private:
  const_value_type const * element_;
  CORBA::Boolean const release_;
};

} // namespace details
} // namespace CORBA

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_string_const_sequence_element_hpp
