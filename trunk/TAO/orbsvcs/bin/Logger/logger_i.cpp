#include <iostream.h>
#include "loggerC.h"
#include "logger_i.h"


Logger_ptr 
Logger_Factory_i::make_logger (const char* name, 
				 CORBA::Environment &IT_env) 
{
  Logger_i* l = new Logger_i (name);
  return (l->_duplicate (l));
}

Logger_Factory_i::Logger_Factory_i (const char *key)
  :POA_Logger_Factory (key)
{
}

Logger_i::Logger_i (const char* name)
  :POA_Logger (name)
{
  name_ = new char [strlen (name) + 1];
  strcpy (name_, name);

  cout << name_ << ": in the constructor" << endl;
}

Logger_i::~Logger_i (void)
{
  cout << name_ << ": in the destructor" << endl;
  delete name_;
}

void 
Logger_i::log (const char* message, CORBA::Environment &IT_env)
{
  cout << name_ << ": " << message << endl;
}
