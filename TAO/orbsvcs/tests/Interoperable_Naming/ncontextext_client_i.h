// -*- C++ -*-

//=============================================================================
/**
 *  @file    ncontextext_client_i.h
 *
 *  This class implements a simple CORBA client which
 *  converts a Name to a string and viceversa, forms a IIOPNAME
 *  url address and can resolve a stringified name.
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
 */
class NContextExt_Client_i
{
public:
  NContextExt_Client_i () = default;
  NContextExt_Client_i (NContextExt_Client_i const &) = delete;
  NContextExt_Client_i (NContextExt_Client_i &&) = delete;
  NContextExt_Client_i &operator= (NContextExt_Client_i const &) = delete;
  NContextExt_Client_i &operator= (NContextExt_Client_i &&) = delete;

  /// Execute the client example code.
  int run ();

  /// Initialize the client communication endpoint with the server.
  int init (int argc, ACE_TCHAR *argv[]);

private:
  /// Parses the arguments passed on the command line.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /**
   * Prints the values of the original name, strigified name and
   * destringified name. Also, prints the value of the address of the
   * naming context, a name that identifies a binding in that naming
   * context and the corresponding iioploc URL
   * representation.
   */
  void print_values (CosNaming::Name const &name,
                     CORBA::String_var const &str_name,
                     CosNaming::Name const &nm,
                     CORBA::String_var const &obj_name,
                     CORBA::String_var const &url_string);

  /// Generates a random name
  CORBA::String_var get_name ();

  /// Naming context
  CosNaming::NamingContextExt_var naming_context_ {};

  /// Whether to print the converted names.
  bool print_values_ {};
};

#endif /* NCONTEXTEXT_CLIENT_I_H */
