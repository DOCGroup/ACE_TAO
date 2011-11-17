// -*- C++ -*-

//=============================================================================
/**
 *  @file    ncontextext_client_i.h
 *
 *  $Id$
 *
 *  This class implements a simple CORBA client which
 *  converts a Name to a string and viceversa, forms a IIOPNAME
 *  url address and can resolve a stringified name.
 *
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================



#if !defined (NCONTEXTEXT_CLIENT_I_H)
#define NCONTEXTEXT_CLIENT_I_H

#include "Web_ServerC.h"
#include "orbsvcs/CosNamingC.h"

/**
 * @class NContextExt_Client_i
 *
 * @brief NContextExt Client Implementation
 *
 */
class NContextExt_Client_i
{

 public:
  // = COnstructor and destructor.
  NContextExt_Client_i (void);
  ~NContextExt_Client_i (void);

  /// Execute the client example code.
  int run (void);

  /// Initialize the client communication endpoint with the server.
  int init (int argc, ACE_TCHAR *argv[]);

 private:

  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /**
   * Prints the values of the original name, strigified name and
   * destringified name. Also, prints the value of the address of the
   * naming context, a name that identifies a binding in that naming
   * context and the corresponding iioploc URL
   * representation.
   */
  void print_values (CosNaming::Name name,
                     CORBA::String_var str_name,
                     CosNaming::Name nm,
                     CORBA::String_var obj_name,
                     CORBA::String_var url_string);

  /// Generates a random name
  char * get_name (void);

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// Naming context
  CosNaming::NamingContextExt_var naming_context_;

  /// Flag to check if '-v' option is set.
  CORBA::Boolean view_;

};

#endif /* NCONTEXTEXT_CLIENT_I_H */
