/**
 * @file RIR_Narrow.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef RIR_Narrow__h_
#define RIR_Narrow__h_

#include <tao/ORB.h>
#include <tao/PortableInterceptorC.h>

namespace TAO
{
namespace Utils
{

/**
 * @class RIR_Narrow
 *
 * @brief Helper class to obtain an initial reference and narrow it
 *        to the proper object reference.
 */
template<class T> class RIR_Narrow
{
public:
  typedef typename T::_ptr_type _ptr_type;
  typedef typename T::_var_type _var_type;

  /// Use resolve_initial_references to find an object and then
  /// narrow it.
  static _ptr_type narrow (CORBA::ORB_ptr orb,
                           char const * id,
                           CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ());

  /// Use resolve_initial_references to find an object and then
  /// narrow it.
  static _ptr_type narrow (PortableInterceptor::ORBInitInfo_ptr orb,
                           char const * id);

private:
  static _ptr_type narrow_object (CORBA::Object_ptr object,
                                  CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ());
};

} // namespace Utils
} // namespace TAO

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
# include "RIR_Narrow.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif // RIR_Narrow__h_
