
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
#include <string>

class Generator
{
public:
  Generator () = default;
  ~Generator () = default;
  Generator (Generator const &) = delete;
  Generator (Generator &&) = delete;
  Generator &operator= (Generator const &) = delete;
  Generator &operator= (Generator &&) = delete;

  CORBA::Long gen_long () const;
  std::string gen_string () const;
  std::string gen_string (CORBA::ULong maxlength) const;
  Alt_Mapping::Fixed_Struct gen_fixed_struct () const;
};

using GENERATOR = ACE_Singleton<Generator, TAO_SYNCH_RECURSIVE_MUTEX>;

#endif /* HELPER_H */
