// $Id$

#include "ace/ACE.h"
#include "orbsvcs/LoggerC.h"
#include "orbsvcs/Log/Logger_i.h"

Logger_ptr
Logger_Factory_i::make_logger (const char* name,
                               CORBA::Environment &_env)
{
  Logger_i *l = new Logger_i (name);
  return l->_this (_env);
}

Logger_Factory_i::Logger_Factory_i (void)
{
}

Logger_i::Logger_i (const char* name)
  :  name_ (ACE_OS::strdup (name))
{
}

Logger_i::~Logger_i (void)
{
  ACE_OS::free (this->name_);
}

void
Logger_i::log (const char* message, CORBA::Environment &_env)
{
}
