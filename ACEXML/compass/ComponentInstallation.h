// $Id$


#ifndef COMPASS_COMPONENT_INSTALLATION_H
#define COMPASS_COMPONENT_INSTALLATION_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/compass/CompassTypes.h"
#include "ace/Unbounded_Set.h"

namespace Deployment
{
  typedef ACE_Hash_Map_Entry<ACEXML_String,
                             ACEXML_String> Package;

  typedef ACE_Hash_Map_Manager_Ex<ACEXML_String,
                                  ACEXML_String,
                                  ACE_Hash<ACEXML_String>,
                                  ACE_Equal_To<ACEXML_String>,
                                  ACE_SYNCH_MUTEX> Package_Manager;

  typedef ACE_Hash_Map_Iterator_Ex<ACEXML_String,
                                   ACEXML_String,
                                   ACE_Hash<ACEXML_String>,
                                   ACE_Equal_To<ACEXML_String>,
                                   ACE_SYNCH_MUTEX> Package_Manager_Iter;

  typedef ACE_Hash_Map_Reverse_Iterator_Ex<ACEXML_String,
                                           ACEXML_String,
                                           ACE_Hash<ACEXML_String>,
                                           ACE_Equal_To<ACEXML_String>,
                                           ACE_SYNCH_MUTEX> Package_Manager_Reverse_Iter;

  class Compass_Export ComponentInstallation
  {
  public:
    friend class ACE_Singleton<ComponentInstallation, ACE_SYNCH_MUTEX>;

    void install(const UUID& implUUID, const Location& component_loc)
      ACE_THROW_SPEC ((InvalidLocation,InstallationFailure));

    void replace(const UUID& implUUID, const Location& component_loc)
      ACE_THROW_SPEC ((InvalidLocation, InstallationFailure));

    void remove(const UUID& implUUID)
      ACE_THROW_SPEC ((UnknownImplId, RemoveFailure));

    const Location& get_implementation (const UUID& implUUID)
      ACE_THROW_SPEC ((UnknownImplId, InstallationFailure));

  protected:
    ComponentInstallation();
    ~ComponentInstallation();
    ComponentInstallation (const ComponentInstallation&);
    ComponentInstallation& operator= (const ComponentInstallation&);

  private:
    Package_Manager packages_;
  };
  typedef ACE_Singleton<ComponentInstallation, ACE_SYNCH_MUTEX> COMP_INSTALL;


}

#if defined (__ACE_INLINE__)
#include "ComponentInstallation.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* COMPASS_COMPONENT_INSTALLATION_H */
