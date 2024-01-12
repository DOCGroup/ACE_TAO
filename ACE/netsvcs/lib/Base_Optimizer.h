/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Base_Optimizer.h
 *
 *  @author Per Andersson.
 */
//=============================================================================

#ifndef BASE_OPTIMIZER_H
#define BASE_OPTIMIZER_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Base_Optimizer
 *
 * Thanks to Nathan Myers and Fergus Henderson for this little
 * beauty.
 */
template<class Base, class Member>
class Base_Optimizer : public Base
{
public:
  Base_Optimizer ();
  Base_Optimizer (const Base &base);
  Base_Optimizer (const Base &base,
                  const Member &member);

  Member m_;
};

#include "Base_Optimizer.cpp"

#endif /* BASE_OPTIMIZER_H */
