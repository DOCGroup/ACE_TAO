#ifndef _C_SERVER_H
#define _C_SERVER_H

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/tao_util.h"
#include "orbsvcs/CosNamingC.h"
#include "cubit_i.h"
#include "Naming_Server.h"


class Cubit_Server:public TAO_ORB_Manager
{
public:
  Cubit_Server (void);
  ~Cubit_Server (void);
  
  int init (int argc,char **argv,CORBA::Environment& env);
  int run (CORBA::Environment& env);

private:
  int parse_args (void);
  // Parses the commandline arguments

  int init_naming_service (CORBA::Environment& env);
  // Initialises the name server and registers cubit factory with the
  // name server

  int num_of_objs_;
  // Number of cubit objects we export

  int use_naming_service_;
  //Flag to tell whether to use the TAO Naming Service

  FILE* ior_output_file_;
  // File to output the cubit factory IOR

  TAO_Naming_Server my_name_server_;
  // An instance of the name server used for registering the factory objects.

  Cubit_Factory_i factory_impl_;
  // Implementation object of the cubit factory
       
  Cubit_Factory_var factory;
  // Factory_var to register with NamingService

  CosNaming::NamingContext_var cubit_context_;
  // Naming context for the cubit_factory

  CosNaming::NamingContext_var naming_context_;
  // Naming context for the Naming Service;

  int argc_;
  // Number of commandline arguments

  char **argv_;
  // commandline arguments
};

#endif
