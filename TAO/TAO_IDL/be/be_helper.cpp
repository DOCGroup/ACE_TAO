#include        "ace/ACE.h"
#include        "ace/OS.h"
#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

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

const char* be_get_client_impl_fname()
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

const char* be_get_server_impl_fname()
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

