// $Id$

#include "AssemblyFactory.h"
#include "ACEXML/common/StreamFactory.h"

using namespace Deployment;

AssemblyFactory::AssemblyFactory()
  : cookies_()
{}

AssemblyFactory::~AssemblyFactory()
{}


Cookie
AssemblyFactory::create (const Location& assembly_loc)
  ACE_THROW_SPEC ((InvalidLocation, CreateFailure))
{
  ACEXML_StreamFactory factory;
  ACEXML_CharStream* stream = factory.create_stream (assembly_loc.c_str());
  if (stream == 0)
    ACE_THROW (InvalidLocation());
  char buf[10];
  int size = ACE_OS::sprintf (buf, "%@", stream);
  buf[size] = 0;
  ACEXML_String cookie (buf);
  Assembly* assembly = 0;
  ACE_NEW (assembly, Assembly (AssemblyState::INACTIVE, stream));
  if (assembly == 0)
    ACE_THROW ((CreateFailure()));
  if (this->cookies_.bind (ret, assembly) != 0)
    ACE_THROW ((CreateFailure()));
  return cookie;
}

Assembly*
AssemblyFactory::lookup (const Cookie& c)
{
  Assembly* asm = 0;
  if (this->cookies_->find (c, asm) != 0)
    ACE_THROW (InvalidAssembly());
  return asm;
}

void
AssemblyFactory::destroy (const Cookie& c)
{
  Assembly* asm = 0;
  if (this->unbind (c, asm) != 0)
    ACE_THROW (InvalidAssembly());
  delete asm;
}
