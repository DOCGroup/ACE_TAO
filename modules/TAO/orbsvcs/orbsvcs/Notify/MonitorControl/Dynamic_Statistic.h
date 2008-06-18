// $Id$

#ifndef DYNAMIC_STATISTIC_H
#define DYNAMIC_STATISTIC_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/MonitorControl/Statistic.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename TYPE>
class TAO_Dynamic_Statistic : public TAO_Statistic
{
public:
  /// Construct the dynamic statistic.  It is assumed that
  /// the life of the TYPE object passed in is directly linked
  /// with the life of this object.
  TAO_Dynamic_Statistic (TYPE* interf,
                         const char* name,
                         TAO_Statistic::Information_Type type)
    : TAO_Statistic (name, type),
      interf_ (interf)
  {
  }

  /// Take the interface, calculate the statistic and
  /// store it for later calculations.  The implementation of this
  /// method is expected to call the receive base class method.
  virtual void calculate (void) = 0;

protected:
  TYPE* interf_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* DYNAMIC_STATISTIC_H */
