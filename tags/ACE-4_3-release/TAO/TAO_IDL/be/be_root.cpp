#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// Default Constructor
be_root::be_root()
{
}

// Constructor
be_root::be_root(UTL_ScopedName *n, UTL_StrList *p)
  : AST_Root(n, p),
    AST_Decl(AST_Decl::NT_module, n, p),
    UTL_Scope(AST_Decl::NT_module)
{
}

// Generates the IDL to C++ mapping
// Generates the client-side header information for the root
long be_root::gen_idl2cplusplus_mapping()
{
  fstream f;  // fstream instance
  TAO_BE_Params *params = TAO_BE_PARAMS::instance();

  // Open the client header file for writing
  f.open(be_get_client_hdr_fname(), ios::out);
  if (f.fail())
    {
      return -1;
    }
  params->client_header(f.rdbuf());

  // generate client header
  if (this->gen_client_header() < 0)
    {
      return -1;
    }
  f.close();

  // Open the server header file for writing
  f.open(be_get_server_hdr_fname(), ios::out);
  if (f.fail())
    {
      return -1;
    }
  params->server_header(f.rdbuf());

  // generate the server header
  if (this->gen_server_header() < 0)
    {
      return -1;
    }
  f.close();
  
  // Open the client stubs file for writing
  f.open(be_get_client_stub_fname(), ios::out);
  params->client_stubs(f.rdbuf());

  // generate client side stubs
  if (this->gen_client_stubs() < 0)
    {
      return -1;
    }
  f.close();

  // Open the server skeleton file for writing
  f.open(be_get_server_skeleton_fname(), ios::out);
  params->server_header(f.rdbuf());

  // generate server side skeletons
  if (this->gen_server_skeletons() < 0)
    {
      return -1;
    }
  f.close();

  return 0;
}

// generate client header
long be_root::gen_client_header()
{
  TAO_BE_Params *params = TAO_BE_PARAMS::instance(); // holds the buffer
						     // pointing to the output
						     // stream  
  ostream o(params->client_header()); // get a handle to the output stream
  
  // check for failure
  if (o.fail())
    {
      return -1;
    }

  // generate the #if !defined, #define statement

  return 0;
}

// Generates the client-side stubs for the root
long be_root::gen_client_stubs()
{
  TAO_BE_Params *params = TAO_BE_PARAMS::instance(); // holds the buffer
						     // pointing to the output
						     // stream  
  ostream o(params->client_stubs()); // get a handle to the output stream
  
  // check for failure
  if (o.fail())
    {
      return -1;
    }

  return 0;
}

// Generates the server-side header information for the root
long be_root::gen_server_header()
{
  TAO_BE_Params *params = TAO_BE_PARAMS::instance(); // holds the buffer
						     // pointing to the output
						     // stream  
  ostream o(params->server_header()); // get a handle to the output stream
  
  // check for failure
  if (o.fail())
    {
      return -1;
    }

  return 0;
}

// Generates the server-side skeletons for the root
long be_root::gen_server_skeletons()
{
  TAO_BE_Params *params = TAO_BE_PARAMS::instance(); // holds the buffer
						     // pointing to the output
						     // stream  
  ostream o(params->server_skeletons()); // get a handle to the output stream
  
  // check for failure
  if (o.fail())
    {
      return -1;
    }

  return 0;
}


/*
 * Narrowing methods
 */
IMPL_NARROW_METHODS1(be_root, AST_Root)
IMPL_NARROW_FROM_DECL(be_root)
IMPL_NARROW_FROM_SCOPE(be_root)
