// $Id$


#ifndef COMPASS_CCMHOME_H
#define COMPASS_CCMHOME_H

#include "ace/pre.h"
#include "ACEXML/compass/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Set.h"
#include "ACEXML/compass/CompassTypes.h"

namespace Deployment
{
  class Compass_Export CCMHome
  {
  public:
    Component* create_component()
      ACE_THROW_SPEC ((CreateFailure));

    void remove_component (Component* comp)
      ACE_THROW_SPEC ((RemoveFailure));

  private:
    Component* component_;
  };

  typedef ACE_Unbounded_Set<CCMHome*> CCMHomes;

}

#if defined (__ACE_INLINE__)
#include "CCMHome.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* COMPASS_CCMHOME_H */
