#if !defined (TAO_DEMUX_TEST_CODEGEN_H)
#define TAO_DEMUX_TEST_CODEGEN_H

// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux/CodeGen
//
// = FILENAME
//    codegen.h
//
//    Defines the code generator class for the Demux_Test
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================


#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "ace/Containers_T.h"
#include "ace/Singleton.h"
#include "ace/OS.h"

class Demux_Test_CodeGenerator
{
public:
  Demux_Test_CodeGenerator (void);
  // ctor

  ~Demux_Test_CodeGenerator (void);
  // dtor

  int init (int argc, char *argv []);
  // initialize

  int run (void);
  // run the various algorithms

  enum CG_STATE
  {
    CG_NONE,
    CG_POA,
    CG_OBJ,
    CG_OP
  };

private:
  int parse_args (int argc, char *argv []);
  // parse arguments

  int gen_poa_names (void);
  // generate names of POAs

  int gen_object_names (void);
  // generate object names

  int gen_operation_names (void);
  // generate operation names

  int gen_idl_interface (void);
  // generate the IDL interface definition

  int gen_object_create_code (void);
  // generate code that activates the objs and o/ps their IOR

  int gen_servant_hdr_code (void);
  // generate the servant impl header file

  int gen_servant_impl_code (void);
  // generate servant implementation

  int gen_client_incl_code (void);
  // generate the client side include code

  int print_names (void);
  // print generated names

  void usage (void);
  // print the usage

  int gen_names (ACE_Unbounded_Queue<ACE_CString> &, int limit);
  // common fn used by others to generate names in a table of size = limit

  int insert_unique_string (ACE_Unbounded_Queue<ACE_CString> &);
  // insert a unique string at location locn distinct from all strings inserted
  // from locations 0 to locn-1

  long gen_rand (int, int);

  int is_present (ACE_Unbounded_Queue<ACE_CString> &, ACE_CString &);
  // is the given string present in the queue?

  int create_string (int len, ACE_CString &s);
  // create a valid identifier of length len

  char gen_valid_char (int);
  // generate a valid character. The argument indicates what position it will
  // be inserted.

  int print_array (ACE_Unbounded_Queue<ACE_CString> &);
  // print the array

  int num_POAs_;
  // number of POAs

  int num_objs_;
  // number of objects

  int num_ops_;
  // number of operations

  ACE_Unbounded_Queue <ACE_CString> poa_array_;
  // array of POA names

  ACE_Unbounded_Queue <ACE_CString> obj_array_;
  // array of object names

  ACE_Unbounded_Queue <ACE_CString> op_array_;
  // array of operation names

  CG_STATE state_;
  // code generation state

  int oneway_;
  // whether operations must be oneway or not

  int debug_;
  // whether to print debugging info

};


#endif /* TAO_DEMUX_TEST_CODEGEN_H */
