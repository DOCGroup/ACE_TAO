#include "logger_tie.h"


Logger_ptr 
Logger_Factory_tie::make_logger (const char* name, 
				 CORBA::Environment &IT_env) 
  throw (CORBA::SystemException)
{
  Logger_tie* l = new Logger_tie (name);
  Logger_ptr lp = new TIE_Logger (Logger_tie) (l);
  return (lp->_duplicate (lp));
}

Logger_tie::Logger_tie (const char* name)
{
  name_ = new char [strlen (name) + 1];
  strcpy (name_, name);

  cout << name_ << ": in the constructor" << endl;
}

Logger_tie::~Logger_tie (void)
{
  cout << name_ << ": in the destructor" << endl;
  delete name_;
}

void 
Logger_tie::log (const char* message, CORBA::Environment &IT_env)
{
  cout << name_ << ": " << message << endl;
}













