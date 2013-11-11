//$Id$
#ifndef TAO_IMR_UTILS_H
#define TAO_IMR_UTILS_H

#include "ImplRepoC.h"

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ImR_Utils {
public:
  static ACE_CString activationModeToString(ImplementationRepository::ActivationMode mode)
  {
    switch (mode )
    {
    case ImplementationRepository::NORMAL:
      return "NORMAL";
    case ImplementationRepository::MANUAL:
      return "MANUAL";
    case ImplementationRepository::PER_CLIENT:
      return "PER_CLIENT";
    case ImplementationRepository::AUTO_START:
      return "AUTO_START";
    default:
      ACE_ASSERT(mode == ImplementationRepository::NORMAL);
      return "";
    }
  }
  static ImplementationRepository::ActivationMode parseActivationMode(const ACE_CString& s)
  {
    if (s == "NORMAL")
      return ImplementationRepository::NORMAL;
    if (s == "MANUAL")
      return ImplementationRepository::MANUAL;
    if (s == "PER_CLIENT")
      return ImplementationRepository::PER_CLIENT;
    if (s == "AUTO_START")
      return ImplementationRepository::AUTO_START;

    return ImplementationRepository::NORMAL;
  }
  static ACE_CString envListToString(const ImplementationRepository::EnvironmentList& lst)
  {
    ACE_CString ret;
    for (CORBA::ULong n = 0; n < lst.length(); ++n)
    {
      ret += "name=\"";
      ret += lst[n].name.in();
      ret += "\" value=\"";
      ret += lst[n].value.in();
      ret += "\"\n";
    }
    return ret;
  }
  static ImplementationRepository::EnvironmentList parseEnvList(const ACE_CString& s)
  {
    ImplementationRepository::EnvironmentList ret(10);

    const ACE_CString NAMETAG = "name=\"";
    const ACE_CString VALTAG = "value=\"";
    const ACE_CString ENDTAG = "\"";

    ACE_CString::size_type i = 0;

    for (CORBA::ULong idx = 0; ; ++idx)
    {
      // find name
      ACE_CString::size_type j = s.find(NAMETAG, i);
      if (j == ACE_CString::npos) break;
      j += NAMETAG.length();
      ACE_CString::size_type k = s.find(ENDTAG, j + 1);
      if (k == ACE_CString::npos) break;
      ACE_CString name = s.substr(j, k - j);

      i = k + 1;

      // find value
      j = s.find(VALTAG, i);
      if (j == ACE_CString::npos) break;
      j += VALTAG.length();
      k = s.find(ENDTAG, j + 1);
      if (k == ACE_CString::npos) break;
      ACE_CString value = s.substr(j, k - j);

      i = k + 1;

      ret.length(idx + 1);
      ret[idx].name = name.c_str();
      ret[idx].value = value.c_str();
    }
    return ret;
  }
};

#endif
