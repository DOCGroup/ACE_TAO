// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    helper.h
//
// = DESCRIPTION
//    Defines a helper class that can generate values for the parameters used
//    for the Param_Test example
//
// = AUTHORS
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef HELPER_H
#define HELPER_H

#include "alt_mappingC.h"
#include "ace/Singleton.h"

class Generator
{
public:
  Generator (void);

  ~Generator (void);

  CORBA::Short gen_short (void);
  CORBA::Long gen_long (void);
  char* gen_string (void);
  char* gen_string (int maxlength);
  CORBA::WChar* gen_wstring (void);
  CORBA::WChar* gen_wstring (int maxlength);
  const Alt_Mapping::Fixed_Struct gen_fixed_struct (void);

 private:
  Alt_Mapping::Fixed_Struct fixed_struct_;
};

typedef ACE_Singleton<Generator, TAO_SYNCH_RECURSIVE_MUTEX> GENERATOR;

#endif /* HELPER_H */
