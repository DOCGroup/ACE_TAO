// $Id$

#include "AssemblyFactory.h"
#include "ACEXML/common/StreamFactory.h"

using namespace Deployment;

Cookie
AssemblyFactory::create (const Location& assembly_loc)
{
  ACEXML_StreamFactory factory;
  ACEXML_CharStream* stream = factory.create_stream (assembly_loc.c_str());
  if (stream == 0)
    ACE_THROW (InvalidLocation);
  char cookie[10];
  ACE_OS::sprintf (cookie, "%x", stream);

  ACE_NEW_RETURN (
