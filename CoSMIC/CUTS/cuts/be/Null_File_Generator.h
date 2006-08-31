// -*- C++ -*-

//=============================================================================
/**
 * @file        Null_File_Generator.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BE_NULL_FILE_GENERATOR_H_
#define _CUTS_BE_NULL_FILE_GENERATOR_H_

#include "BE_File_Generator.h"

//=============================================================================
/**
 * @class CUTS_BE_Null_File_Generator
 *
 * Null backend file generator.
 */
//=============================================================================

class CUTS_BE_Export CUTS_BE_Null_File_Generator :
  public CUTS_BE_File_Generator
{
public:
  /// Default constructor.
  CUTS_BE_Null_File_Generator (void);

  /// Destructor.
  virtual ~CUTS_BE_Null_File_Generator (void);
};

#endif  // !defined _CUTS_BE_NULL_FILE_GENERATOR_H_
