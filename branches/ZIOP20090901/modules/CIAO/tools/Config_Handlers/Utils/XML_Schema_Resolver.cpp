// $Id$
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XercesDefs.hpp>

#include "XML_Schema_Resolver.h"
#include "XercesString.h"

#include "ace/Env_Value_T.h"

#include <iostream>

namespace CIAO
{
  namespace Config_Handlers
  {
    Basic_Resolver::Basic_Resolver (const ACE_TCHAR *path)
      : path_ (path)
    {
    }

    XMLCh *
    Basic_Resolver::operator() (const XMLCh *const,
                                const XMLCh *const systemId,
                                const XMLCh *const) const
    {
      XStr path (path_);
      path.append (systemId);
      return path.release ();
    }

    void
    Basic_Resolver::set_path (const ACE_TCHAR *path)
    {
      this->path_ = path;
    }

    Environment_Resolver::Environment_Resolver (const ACE_TCHAR *variable,
                                                const ACE_TCHAR *relpath)
    {
      xercesc::XMLPlatformUtils::Initialize();
      this->add_path (variable, relpath);
    }

    Environment_Resolver::~Environment_Resolver ()
    {
      xercesc::XMLPlatformUtils::Terminate();
    }

    using xercesc::XMLPlatformUtils;

    void
    Environment_Resolver::add_path (const ACE_TCHAR *variable,
                                    const ACE_TCHAR *relpath)
    {
      ACE_Env_Value <const ACE_TCHAR *> path_env (variable,
                                                  ACE_TEXT(""));

      XStr xpath (path_env);
      XStr xrelpath (relpath);

      xpath.append (xrelpath);

      paths_.push_back (xpath);
    }

    XMLCh *
    Environment_Resolver::operator() (const XMLCh *const,
                                      const XMLCh *const systemId,
                                      const XMLCh *const) const
    {
      for (std::vector<XStr>::const_iterator i = this->paths_.begin ();
           i != this->paths_.end ();
           ++i)
        {
          XStr path (*i);
          path.append(systemId);

          FileHandle file (XMLPlatformUtils::openFile (path));

          // Check for the file presence.
          if (file != 0)
            {
              XMLPlatformUtils::closeFile (file);
              return path.release ();
            }
        }
      return 0;
    }
  }
}
