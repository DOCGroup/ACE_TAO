#include "XML_Schema_Resolver.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XercesDefs.hpp>

#include "XercesString.h"

#include "ace/Env_Value_T.h"
#include "ace/SString.h"

#include <new>
#include <utility>

namespace XML
{
  Basic_Resolver::Basic_Resolver (ACE_TCHAR const *path)
    : path_ (path)
  {
  }

  XMLCh *Basic_Resolver::operator() (XMLCh const *const, XMLCh const *const systemId) const
  {
    XStr path (path_);
    if (!path.append (systemId))
      return nullptr;

    return path.release ();
  }

  Environment_Resolver::Environment_Resolver (ACE_TCHAR const *variable, ACE_TCHAR const *relpath)
  {
    this->add_path (variable, relpath);
  }

  void Environment_Resolver::add_path (ACE_TCHAR const *variable, ACE_TCHAR const *relpath)
  {
    ACE_Env_Value<ACE_TString> path_env (variable, ACE_TEXT (""));

    XStr xpath (static_cast<ACE_TString> (path_env).c_str ());
    XStr const xrelpath (relpath);

    if (!xpath.append (xrelpath))
      throw std::bad_alloc ();

    paths_.push_back (std::move (xpath));
  }

  XMLCh *Environment_Resolver::operator() (XMLCh const *const, XMLCh const *const systemId) const
  {
    for (XStr const &path_prefix : this->paths_)
    {
      XStr path (path_prefix);
      if (!path.append (systemId))
        continue;

      xercesc::FileHandle const file (xercesc::XMLPlatformUtils::openFile (path));

      if (file != nullptr)
      {
        xercesc::XMLPlatformUtils::closeFile (file);
        return path.release ();
      }
    }
    return nullptr;
  }
}
