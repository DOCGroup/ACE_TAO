// $Id$


#ifndef COMPASS_ASSEMBLY_H
#define COMPASS_ASSEMBLY_H

#include "ace/pre.h"
#include "ACEXML/common/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/CharStream.h"
#include "ACEXML/compass/CompassTypes.h"

namespace Deployment
{
  class Assembly
  {
  public:
    Assembly(void);
    Assembly (const AssemblyState& state, ACEXML_CharStream* stream);
    ~Assembly();

    AssemblyState get_state();

    void build()
      ACE_THROW_SPEC ((CreateFailure));

    void tear_down()
      ACE_THROW_SPEC ((RemoveFailure));

  private:
    AssemblyState state_;
    ACEXML_CharStream* stream_;
  };
};


#endif /* COMPASS_ASSEMBLY_H */
