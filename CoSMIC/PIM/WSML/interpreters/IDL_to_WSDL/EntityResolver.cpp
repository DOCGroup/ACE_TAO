// $Id$

// ============================================================================
//
//
// = LIBRARY
//    IDL_TO_PICML_BE_DLL
//
// = FILENAME
//    EntityResolver.cpp
//
// = DESCRIPTION
//    Source file for class deriving from DOMEntityResolver
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#include "EntityResolver.h"
#include "XercesString.h"

#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

using xercesc::Wrapper4InputSource;
using xercesc::LocalFileInputSource;

EntityResolver::EntityResolver (const char *path)
  : base_path_ (path)
{}

/// This function is called by the Xerces infrastructure to
/// actually resolve the location of a schema.
DOMInputSource *
EntityResolver::resolveEntity (const XMLCh *const publicId,
                               const XMLCh *const systemId,
                               const XMLCh *const baseURI)
{
  ACE_UNUSED_ARG (baseURI);
  ACE_UNUSED_ARG (publicId);

  XStr path (this->base_path_.c_str ());
  path.append (systemId);

  // Ownership of these objects is taken care of.
  return new Wrapper4InputSource (new LocalFileInputSource (path));
}