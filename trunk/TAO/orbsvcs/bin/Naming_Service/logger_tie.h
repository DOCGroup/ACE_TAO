#ifndef logger_tieh
#define logger_tieh


#include "logger.hh"
#include <iostream.h>
#include <string.h>


class Logger_Factory_tie 
{
public:
  
  // this function creates and returns a logger with the given name 
  virtual Logger_ptr make_logger (const char * name, 
				  CORBA::Environment &IT_env=CORBA::default_environment) 
    throw (CORBA::SystemException);
};


class Logger_tie 
{

public:

  // constructor
  Logger_tie (const char * name);

  // destructor
  virtual ~Logger_tie (void);

  virtual void log (const char *message, 
		    CORBA::Environment &IT_env=CORBA::default_environment);

private:
  // logger identification
  char *name_;
};



DEF_TIE_Logger (Logger_tie)
DEF_TIE_Logger_Factory (Logger_Factory_tie)

#endif
