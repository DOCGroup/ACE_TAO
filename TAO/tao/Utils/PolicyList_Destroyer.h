/**
 * @file PolicyList_Destroyer.hpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef PolicyList_Destroyer__hpp_
#define PolicyList_Destroyer__hpp_

#include <tao/PolicyC.h>

namespace TAO
{
namespace Utils
{

/**
 * @class PolicyList_Destroyer
 *
 * @brief Automatically destroy all the policies set in a PolicyList
 *
 */
class PolicyList_Destroyer
  : public CORBA::PolicyList
{
public:
  PolicyList_Destroyer(CORBA::ULong length_hint);
  ~PolicyList_Destroyer();
};

} // namespace Utils
} // namespace TAO

#if defined (__ACE_INLINE__)
# include "PolicyList_Destroyer.inl"
#endif /* __ACE_INLINE__ */

#endif // PolicyList_Destroyer__hpp_
