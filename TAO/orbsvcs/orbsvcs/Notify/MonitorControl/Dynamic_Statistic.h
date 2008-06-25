// $Id$

#ifndef DYNAMIC_STATISTIC_H
#define DYNAMIC_STATISTIC_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Base.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename TYPE>
class TAO_Dynamic_Statistic : public Monitor_Base
{
public:
  /// Construct the dynamic statistic.  It is assumed that
  /// the life of the TYPE object passed in is directly linked
  /// with the life of this object.
  TAO_Dynamic_Statistic (TYPE* interf,
                         const char* name,
                         Monitor_Base::Information_Type type)
    : Monitor_Base (name, type),
      interf_ (interf)
  {
  }

  /// Take the interface, update the data, calculate the statistic and
  /// store it for later calculations.  The implementation of this
  /// method is expected to call the receive base class method. This
  /// method isn't pure virtual in Monitor_Base, but we reintroduce
  /// it here to force all subclasses to implement it.
  virtual void update (void) = 0;

protected:
  TYPE* interf_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif /* DYNAMIC_STATISTIC_H */
