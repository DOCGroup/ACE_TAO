// $Id$


#ifndef COMPASS_CCMHOME_H
#define COMPASS_CCMHOME_H

#include "ace/pre.h"
#include "ACEXML/common/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/compass/CompassTypes.h"
namespace Deployment
{
  class CCMHome
  {
  public:
    Component* get_component_def();

    void remove_component (const Component* comp)
      ACE_THROW_SPEC ((RemoveFailure));
  private:
    Component* comp_;
  };

  typedef vector<CCMHome*> CCMHomes;

};

#endif /* COMPASS_CCMHOME_H */
