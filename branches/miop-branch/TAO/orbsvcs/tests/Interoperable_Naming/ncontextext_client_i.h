// -*- C++ -*-
// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/ORBSVCS/tests/SimpleNaming
//
// = FILENAME
//    ncontextext_client_i.h
//
// = DESCRIPTION
//    This class implements a simple CORBA client which 
//    converts a Name to a string and viceversa, forms a IIOPNAME
//    url address and can resolve a stringified name.
//
// = AUTHORS
//    Priyanka Gontla <pgontla@ece.uci.edu>
//
//============================================================


#if !defined (NCONTEXTEXT_CLIENT_I_H)
#define NCONTEXTEXT_CLIENT_I_H

#include "Web_ServerC.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

class NContextExt_Client_i
{
  // = TITLE
  //     NContextExt Client Implementation
  //
  // = DESCRIPTION
  //

 public:
  // = COnstructor and destructor.
  NContextExt_Client_i (void);
  ~NContextExt_Client_i (void);

  int run (TAO_ENV_SINGLE_ARG_DECL);
  // Execute the client example code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with the server.

 private:
  
  int parse_args (void);
  // Parses the arguments passed on the command line.

  void print_values (CosNaming::Name name,
                     CORBA::String_var str_name,
                     CosNaming::Name nm,
                     CORBA::String_var obj_name,
                     CORBA::String_var url_string);
  // Prints the values of the original name, strigified name and
  // destringified name. Also, prints the value of the address of the
  // naming context, a name that identifies a binding in that naming
  // context and the corresponding iioploc URL 
  // representation.

  char * get_name (void);
  // Generates a random name

    int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  CosNaming::NamingContextExt_var naming_context_;
  // Naming context

  CORBA::Boolean view_;
  // Flag to check if '-v' option is set.

};

#endif /* NCONTEXTEXT_CLIENT_I_H */
