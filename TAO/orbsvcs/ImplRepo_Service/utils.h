#ifndef TAO_IMR_UTILS_H
#define TAO_IMR_UTILS_H

#include "ImplRepoC.h"

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ImR_Utils
{
public:
  static char const *activationModeToString (ImplementationRepository::ActivationMode mode);
  static ImplementationRepository::ActivationMode stringToActivationMode (ACE_CString const& s);

  static char const *envListToString (const ImplementationRepository::EnvironmentList& lst);
  static void stringToEnvList (ACE_CString const& s,
                               ImplementationRepository::EnvironmentList& ret);

  static char const *peerListToString (CORBA::StringSeq const& lst);

  static void stringToPeerList (ACE_CString const& s,
                                CORBA::StringSeq& ret);
};

#endif
