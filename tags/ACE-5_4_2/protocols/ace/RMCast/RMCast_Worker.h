/* -*- C++ -*- */
// $Id$
//

#ifndef ACE_RMCAST_WORKER_H
#define ACE_RMCAST_WORKER_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class KEY, class ITEM>
class ACE_RMCast_Worker
{
public:
  virtual ~ACE_RMCast_Worker (void);

  virtual int work (KEY const & key,
                    ITEM const & item) = 0;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Worker.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "RMCast_Worker.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("RMCast_Worker.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_RMCAST_WORKER_H */
