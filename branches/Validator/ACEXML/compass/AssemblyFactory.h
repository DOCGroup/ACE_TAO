// $Id$


#ifndef COMPASS_ASSEMBLY_FACTORY_H
#define COMPASS_ASSEMBLY_FACTORY_H

#include "ace/pre.h"
#include "ACEXML/compass/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager.h"
#include "ACEXML/compass/CompassTypes.h"

namespace Deployment
{

  typedef ACE_Hash_Map_Entry<ACEXML_String,
                             Assembly*> COOKIE;

  typedef ACE_Hash_Map_Manager_Ex<ACEXML_String,
                                  Assembly*
                                  ACE_Hash<ACEXML_String>,
                                  ACE_Equal_To<ACEXML_String>,
                                  ACE_SYNCH_MUTEX> COOKIES_MANAGER;

  typedef ACE_Hash_Map_Iterator_Ex<ACEXML_String,
                                   Assembly*
                                   ACE_Hash<ACEXML_String>,
                                   ACE_Equal_To<ACEXML_String>,
                                   ACE_SYNCH_MUTEX> COOKIES_MANAGER_ITER;

  typedef ACE_Hash_Map_Reverse_Iterator_Ex<ACEXML_String,
                                           Assembly*
                                           ACE_Hash<ACEXML_String>,
                                           ACE_Equal_To<ACEXML_String>,
                                           ACE_SYNCH_MUTEX> COOKIES_MANAGER_REVERSE_ITER;

  class Compass_Export AssemblyFactory
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
    AssemblyFactory();
    ~AssemblyFactory();
    AssemblyFactory (const AssemblyFactory&);
    AssemblyFactory& operator= (const AssemblyFactory&);
  private:
    COOKIES_MANAGER cookies_;
  };
  typedef ACE_Singleton<AssemblyFactory, ACE_SYNCH_MUTEX> ASSEMBLY_FACTORY;
}

#include "ace/post.h"

#endif /* COMPASS_ASSEMBLY_FACTORY_H */
