#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/* BE global Data */
TAO_BE_Params::TAO_BE_Params()
  : pd_client_header(0),
    pd_client_stubs(0),
    pd_server_header(0),
    pd_server_skeletons(0)
{
}

// set the client header stream
void TAO_BE_Params::client_header(streambuf *sbuf)
{
  this->pd_client_header = sbuf;
}

// get the client header stream
streambuf* TAO_BE_Params::client_header()
{
  return this->pd_client_header;
}

// set the client stub stream
void TAO_BE_Params::client_stubs(streambuf* f)
{
  this->pd_client_stubs = f;
}

// get the client stubs stream
streambuf* TAO_BE_Params::client_stubs()
{
  return this->pd_client_stubs;
}

// set the server header stream
void TAO_BE_Params::server_header(streambuf* f)
{
  this->pd_server_header = f;
}

// get the server header stream
streambuf* TAO_BE_Params::server_header()
{
  return this->pd_server_header;
}

// set the server skeletons stream
void TAO_BE_Params::server_skeletons(streambuf* f)
{
  this->pd_server_skeletons = f;
}

// get the server skeletons stream
streambuf* TAO_BE_Params::server_skeletons()
{
  return this->pd_server_skeletons;
}

/************ Helper functions **************/
const char* be_get_client_hdr_fname()
{
  String *s;
  char *base;
  static char fname[128];

  ACE_OS::memset(fname, 0, 128);
  s = idl_global->idl_src_file(); 
  // get the IDL source file name
  base = ACE_OS::strstr(s->get_string(), ".idl");
  // get the base part of the filename
  if (base == NULL)
    return 0;

  ACE_OS::strncpy(fname, s->get_string(), base-s->get_string());
  ACE_OS::strcat(fname, "C.h");
  return fname;
}

const char* be_get_client_stub_fname()
{
  String *s;
  char *base;
  static char fname[128];

  ACE_OS::memset(fname, 0, 128);
  s = idl_global->idl_src_file(); 
  // get the IDL source file name
  base = ACE_OS::strstr(s->get_string(), ".idl");
  // get the base part of the filename
  if (base == NULL)
    return 0;

  ACE_OS::strncpy(fname, s->get_string(), base-s->get_string());
  ACE_OS::strcat(fname, "C.cpp");
  return fname;
}

const char* be_get_server_hdr_fname()
{
  String *s;
  char *base;
  static char fname[128];

  ACE_OS::memset(fname, 0, 128);
  s = idl_global->idl_src_file(); 
  // get the IDL source file name
  base = ACE_OS::strstr(s->get_string(), ".idl");
  // get the base part of the filename
  if (base == NULL)
    return 0;

  ACE_OS::strncpy(fname, s->get_string(), base-s->get_string());
  ACE_OS::strcat(fname, "S.h");
  return fname;
}

const char* be_get_server_skeleton_fname()
{
  String *s;
  char *base;
  static char fname[128];

  ACE_OS::memset(fname, 0, 128);
  s = idl_global->idl_src_file(); 
  // get the IDL source file name
  base = ACE_OS::strstr(s->get_string(), ".idl");
  // get the base part of the filename
  if (base == NULL)
    return 0;

  ACE_OS::strncpy(fname, s->get_string(), base-s->get_string());
  ACE_OS::strcat(fname, "S.cpp");
  return fname;
}

