//$Id$
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
  static const char * activationModeToString (ImplementationRepository::ActivationMode mode);
  static ImplementationRepository::ActivationMode stringToActivationMode (const ACE_CString& s);

  static const char *envListToString (const ImplementationRepository::EnvironmentList& lst);
  static void stringToEnvList (const ACE_CString& s,
                               ImplementationRepository::EnvironmentList& ret);

  static const char *peerListToString (const CORBA::StringSeq& lst);

  static void stringToPeerList (const ACE_CString& s,
                                CORBA::StringSeq& ret);
};

#endif
