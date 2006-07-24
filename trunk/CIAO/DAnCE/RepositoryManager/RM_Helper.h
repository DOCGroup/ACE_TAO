
/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    RM_Helper.h
 *
 *  $Id$
 *
 *  This class aggregates a number of helper functions used by the
 *  CIAO RepositoryManager
 *
 *
 *  @author Stoyan Paunov
 */
//=============================================================================

#ifndef RM_HELPER_H_
#define RM_HELPER_H_


#include "ciao/Packaging_DataC.h"    //for the PackageConfiguration declaration
#include "tao/CDR.h"                 //for TAO CDR classes
#include "ace/Message_Block.h"      //for ACE_Message_Block


class RM_Helper
{
public:

  static void pc_to_cdr (const Deployment::PackageConfiguration& pc, TAO_OutputCDR& cdr);

  static void cdr_to_pc (Deployment::PackageConfiguration& pc, TAO_InputCDR& cdr);

  static bool externalize (const Deployment::PackageConfiguration& pc, const char* path);

  static bool reincarnate (Deployment::PackageConfiguration& pc, const char* path);

  ///function that copies the file from a specified location to another 
  ///specified location on the hard disk.
  ///returns 1 on success
  ///        0 on error

  static bool copy_from_disk_to_disk (const char* from_path, const char* to_path);


  ///function that writes out a file to a specified location on the hand disk
  ///returns 1 on success
  ///        0 on already exists and replace == false
  ///        0 on error

  static bool write_to_disk (const char* full_path,
                 ACE_Message_Block& mb,
                 bool replace = true
                 );

  ///function that writes out a file to a specified location on the hand disk
  ///returns 1 on success
  ///        0 on already exists and replace == false
  ///        0 on error
  ///
  ///NOTE: This function is relevant for PackageConfigurations ONLY

  static bool write_pc_to_disk (const char* full_path,
                  ACE_Message_Block& mb,
                  bool replace = true
                 );


  ///function to read the contents of a file from disk into an ACE_Message_Block
  ///returns a pointer to an ACE_Message_Block and updates the lenght on success
  ///0 on failure

  static ACE_Message_Block* read_pc_from_disk (const char* full_path, size_t &length);

};

#endif
