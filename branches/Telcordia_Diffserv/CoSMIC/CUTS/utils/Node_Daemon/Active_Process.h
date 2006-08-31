/* -*- C++ -*- */

//=============================================================================
/**
 * @file      Active_Process.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_ACTIVE_PROCESS_H_
#define _CUTS_ACTIVE_PROCESS_H_

#include "cuts/config.h"
#include "ace/Process.h"

class CUTS_Active_Process : public ACE_Managed_Process
{
public:
  CUTS_Active_Process (pid_t pid);

  virtual ~CUTS_Active_Process (void);

  pid_t spawn (ACE_Process_Options & options);
};

#if defined (__CUTS_INLINE__)
#include "Active_Process.inl"
#endif

#endif  /* !defined _CUTS_ACTIVE_PROCESS_H_ */
