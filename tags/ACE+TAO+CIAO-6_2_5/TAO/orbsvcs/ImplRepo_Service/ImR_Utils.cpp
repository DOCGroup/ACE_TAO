//$Id$

#include "utils.h"
#include "ImplRepoC.h"

#include "ace/SString.h"

const char *
ImR_Utils::activationModeToString (ImplementationRepository::ActivationMode mode)
{
  switch (mode)
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

ImplementationRepository::ActivationMode
ImR_Utils::stringToActivationMode (const ACE_CString& s)
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

const char *
ImR_Utils::envListToString(const ImplementationRepository::EnvironmentList& lst)
{
  static ACE_CString ret;
  ret = "";
  for (CORBA::ULong n = 0; n < lst.length(); ++n)
    {
      ret += "name=\"";
      ret += lst[n].name.in();
      ret += "\" value=\"";
      ret += lst[n].value.in();
      ret += "\"\n";
    }
  return ret.c_str();
}

void
ImR_Utils::stringToEnvList(const ACE_CString& s,
                           ImplementationRepository::EnvironmentList& ret)
{
  const ACE_CString NAMETAG = "name=\"";
  const ACE_CString VALTAG = "value=\"";
  const ACE_CString ENDTAG = "\"";

  size_t pos = 0;
  size_t start = 0;

  CORBA::ULong idx =  0;
  for (pos = s.find ('\n'); pos != ACE_CString::npos; pos = s.find ('\n',pos + 1))
    {
      idx++;
    }
  ret.length (idx);
  if (idx == 0)
    {
      return;
    }

  ACE_CString source = s;
  for (idx = 0; idx < ret.length () ;idx++)
    {
      pos = source.find ("\n");
      ACE_CString entry = source.substr (0,pos);
      source = source.substr (pos + 1);
      ret[idx].name = "";
      ret[idx].value = "";

      if ((start = entry.find (NAMETAG)) == ACE_CString::npos)
        continue;
      start += NAMETAG.length();
      if ((pos = entry.find (ENDTAG, start + 1)) == ACE_CString::npos)
        continue;
      ret[idx].name = entry.substr (start, pos - start).c_str();


      if ((start = entry.find (VALTAG, pos)) == ACE_CString::npos)
        continue;
      start += VALTAG.length ();
      if ((pos = entry.find (ENDTAG, start + 1)) == ACE_CString::npos)
        continue;
      ret[idx].value = entry.substr (start, pos - start).c_str();
    }
}

const char *
ImR_Utils::peerListToString (const CORBA::StringSeq& lst)
{
  static ACE_CString ret;
  ret = "";
  for (CORBA::ULong n = 0; n < lst.length(); ++n)
    {
      ret += "name=\"";
      ret += lst[n].in();
      ret += "\"\n";
    }
  return ret.c_str();
}

void
ImR_Utils::stringToPeerList (const ACE_CString& s, CORBA::StringSeq& ret)
{
  const ACE_CString NAMETAG = "name=\"";
  const ACE_CString ENDTAG = "\"";

  size_t pos = 0;
  size_t start = 0;

  CORBA::ULong idx =  0;
  for (pos = s.find ('\n'); pos != ACE_CString::npos; pos = s.find ('\n',pos + 1))
    {
      idx++;
    }
  ret.length (idx);
  if (idx == 0)
    {
      return;
    }

  ACE_CString source = s;
  for (idx = 0; idx < ret.length () ;idx++)
    {
      pos = source.find ("\n");
      ACE_CString entry = source.substr (0,pos);
      source = source.substr (pos + 1);
      ret[idx] = "";

      if ((start = entry.find (NAMETAG)) == ACE_CString::npos)
        continue;
      start += NAMETAG.length();
      if ((pos = entry.find (ENDTAG, start + 1)) == ACE_CString::npos)
        continue;
      ret[idx] = entry.substr (start, pos - start).c_str();
    }

}
