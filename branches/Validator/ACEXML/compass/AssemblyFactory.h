// $Id$


#ifndef COMPASS_ASSEMBLY_FACTORY_H
#define COMPASS_ASSEMBLY_FACTORY_H

#include "ace/pre.h"
#include "ACEXML/common/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Unbounded_Set.h"
#include "ACEXML/compass/CompassTypes.h"

namespace Deployment
{
  class AssemblyFactory
  {
  public:
    friend class ACE_Singleton<AssemblyFactory, ACE_SYNCH_MUTEX>;
    Cookie create(const Location& assembly_loc)
      ACE_THROW_SPEC ((InvalidLocation, CreateFailure));

    Assembly* lookup(const Cookie& c)
      ACE_THROW_SPEC ((InvalidAssembly));

    void destroy(const Cookie& c)
      ACE_THROW_SPEC ((InvalidAssembly, RemoveFailure));
  protected:
    AssemblyFactory (const AssemblyFactory&);
    AssemblyFactory& operator= (const AssemblyFactory&);
  private:
    ACE_Unbounded_Set<ACEXML_String> cookies_;
  };
  typedef ACE_Singleton<AssemblyFactory, ACE_SYNCH_MUTEX> ASSEMBLY_FACTORY;
};


#endif /* COMPASS_ASSEMBLY_FACTORY_H */
