/* -*- C++ -*- */

#if !defined (LOGGER_I_H)
#define LOGGER_I_H


#include "loggerS.h"
#include <iostream.h>
#include <string.h>


class Logger_Factory_i : public virtual POA_Logger_Factory
{
public:
  Logger_Factory_i (const char *key);
  // constructor

  virtual Logger_ptr make_logger (const char * name, 
				  CORBA::Environment &IT_env);
  // this function creates and returns a logger with the given name 
};


class Logger_i : public virtual POA_Logger
{
public:

  // constructor
  Logger_i (const char * name);

  // destructor
  virtual ~Logger_i (void);

  virtual void log (const char *message, 
		    CORBA::Environment &IT_env);

private:
  // logger identification
  char *name_;
};

#endif /* LOGGER_I_H */
