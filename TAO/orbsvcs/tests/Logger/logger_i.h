/* -*- C++ -*- */
#if !defined (LOGGER_I_H)
#define LOGGER_I_H

#include "orbsvcs/LoggerS.h"

class Logger_Factory_i : public virtual POA_Logger_Factory
{
public:
  Logger_Factory_i (void);
  // constructor.

  virtual Logger_ptr make_logger (const char *name,
                                  CORBA::Environment &_env);
  // This function creates and returns a logger with the given name.
};

class Logger_i : public virtual POA_Logger
{
public:
  Logger_i (const char* name);
  // constructor

  virtual ~Logger_i (void);
  // destructor

  virtual void log (const char *message,
                    CORBA::Environment &_env);

private:
  char *name_;
  // logger identification
};

#endif /* LOGGER_I_H */
