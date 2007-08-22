// $Id$
#include "XML_Schema_Resolver.h"
#include "XercesString.h"
#include "ace/Env_Value_T.h"
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

using xercesc::Wrapper4InputSource;
using xercesc::LocalFileInputSource;


namespace CIAO
{
  namespace Config_Handlers
  {
    CIAO_Schema_Resolver::CIAO_Schema_Resolver (void)
    {
      this->base_path_ = this->resolve_from_environment ();
    }

    CIAO_Schema_Resolver::CIAO_Schema_Resolver (const char *path)
    {
      if (path == 0)
        this->base_path_ = this->resolve_from_environment ();
      else
        this->base_path_ = path;
    }

    /// This function is called by the Xerces infrastructure to
    /// actually resolve the location of a schema.
    DOMInputSource *
    CIAO_Schema_Resolver::resolveEntity (const XMLCh *const publicId,
                                         const XMLCh *const systemId,
                                         const XMLCh *const baseURI)
    {
      ACE_UNUSED_ARG (baseURI);
      ACE_UNUSED_ARG (publicId);

      XStr path (this->base_path_.c_str ());
      path.append (systemId);

      // Ownership of these objects is given to other people.
      return new Wrapper4InputSource (new LocalFileInputSource (path));
    }

    std::string
    CIAO_Schema_Resolver::resolve_from_environment (void)
    {
      ACE_Env_Value <const char *> path ("CIAO_ROOT", "");

      std::string retval (path);
      return retval += "/docs/schema/";
    }
  }
}
