//
// $Id$
//
#include "ace/streams.h"
#include "ace/ACE.h"
#include "orbsvcs/LoggerC.h"
#include "logger_i.h"


Logger_ptr 
Logger_Factory_i::make_logger (const char* name,
			       CORBA::Environment &_env) 
{
  Logger_i* l = new Logger_i (name);
  return l->_this (_env);
}

Logger_Factory_i::Logger_Factory_i (void)
{
}

Logger_i::Logger_i (const char* name)
  :  name_ (ACE_OS::strdup (name))
{
  cout << this->name_ << ": in the constructor" << endl;
}

Logger_i::~Logger_i (void)
{
  cout << name_ << ": in the destructor" << endl;
  ACE_OS::free (this->name_);
}

void 
Logger_i::log (const char* message, CORBA::Environment &_env)
{
  cout << this->name_ << ": " << message << endl;
}
