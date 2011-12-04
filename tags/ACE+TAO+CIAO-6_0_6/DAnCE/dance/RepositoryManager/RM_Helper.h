
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
 *  @author Stoyan Paunov
 */
//=============================================================================

#ifndef RM_HELPER_H_
#define RM_HELPER_H_

#include "dance/Deployment/Deployment_Packaging_DataC.h"
#include "tao/CDR.h"
#include "ace/Message_Block.h"


class RM_Helper
{
public:

  static void pc_to_cdr (const Deployment::PackageConfiguration& pc, TAO_OutputCDR& cdr);

  static void cdr_to_pc (Deployment::PackageConfiguration& pc, TAO_InputCDR& cdr);

  static bool externalize (const Deployment::PackageConfiguration& pc, const char* path);

  static bool reincarnate (Deployment::PackageConfiguration& pc, const char* path);

  /**
   * Function that copies the file from a specified location to another
   * specified location on the hard disk.
   * @retval true on success
   * @retval false on error
   */
  static bool copy_from_disk_to_disk (const char* from_path, const char* to_path);

  /**
   * Function that writes out a file to a specified location on the hard disk
   * @retval true on success
   * @retval false on already exists and replace == false
   * @retval false on error
   */
  static bool write_to_disk (const char* full_path,
                 ACE_Message_Block& mb,
                 bool replace = true);

  /**
   * Function that writes out a file to a specified location on the hard disk
   * @retval true on success
   * @retval false on already exists and replace == false
   * @retval 0 on error
   * @note This function is relevant for PackageConfigurations ONLY
   */
  static bool write_pc_to_disk (const char* full_path,
                                ACE_Message_Block& mb,
                                bool replace = true);

  /**
   * Function to read the contents of a file from disk into an
   * ACE_Message_Block returns a pointer to an ACE_Message_Block and updates
   * the lenght on success
   * @retval 0 on failure
   */
  static ACE_Message_Block* read_pc_from_disk (const char* full_path,
                                               size_t &length);

};

#endif
