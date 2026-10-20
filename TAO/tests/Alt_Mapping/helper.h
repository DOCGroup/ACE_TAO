
//=============================================================================
/**
 *  @file    helper.h
 *
 *  Defines a helper class that can generate values for the parameters used
 *  for the Param_Test example
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef HELPER_H
#define HELPER_H

#include "alt_mappingC.h"
#include "ace/Singleton.h"

class Generator
{
public:
  Generator ();

  ~Generator ();

  CORBA::Short gen_short ();
  CORBA::Long gen_long ();
  char* gen_string ();
  char* gen_string (int maxlength);
  CORBA::WChar* gen_wstring ();
  CORBA::WChar* gen_wstring (int maxlength);
  Alt_Mapping::Fixed_Struct const gen_fixed_struct ();

 private:
  Alt_Mapping::Fixed_Struct fixed_struct_;
};

using GENERATOR = ACE_Singleton<Generator, TAO_SYNCH_RECURSIVE_MUTEX>;

#endif /* HELPER_H */
