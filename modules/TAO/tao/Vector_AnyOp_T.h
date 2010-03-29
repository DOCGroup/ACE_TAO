#ifndef guard_vector_anyop
#define guard_vector_anyop
/**
 * @file
 *
 * @brief CORBA::Any insertion/extraction for std::vector
 *
 * $Id$
 *
 * @author Jeff Parsons
 */
 
#include <vector>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template<typename T>
  void
  insert_value_vector (
    CORBA::Any &,
    std::vector<T> &)
  {
  }
    
  template<typename T>
  bool
  extract_value_vector (
    const CORBA::Any &,
    std::vector<T> &)
  {
    return true;
  }  

  template<typename T>
  void
  insert_objref_vector (
    CORBA::Any &,
    std::vector<T::_ptr_type> &)
  {
  }
    
  template<typename T>
  bool
  extract_objref_vector (
    const CORBA::Any &,
    std::vector<T::_ptr_type> &)
  {
    return true;
  }  

  template<typename T_forany>
  void
  insert_array_vector (
    CORBA::Any &,
    std::vector<T_forany::_slice_type *> &)
  {
  }
    
  template<typename T_forany>
  bool
  extract_array_vector (
    const CORBA::Any &,
    std::vector<T_forany::_slice_type *> &)
  {
    return true;
  }  
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // guard_vector_anyop
