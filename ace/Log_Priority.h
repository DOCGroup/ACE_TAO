/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Log_Priority.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_LOG_PRIORITY_H)
#define ACE_LOG_PRIORITY_H

enum ACE_Log_Priority
  // = TITLE
  //     This data type indicates the relative priorities of the
  //     logging messages, from lowest to highest priority.
  //
  // = DESCRIPTION
  //     These values are defined using powers of two so that it's
  //     possible to form a mask to turn the on or off dynamically.
{
  // = Note, this first argument *must* start at 1! 

  LM_SHUTDOWN = 01,
  // Shutdown the logger (decimal 1).

  LM_TRACE = 02,    
  // Messages indicating function-calling sequence (decimal 2).

  LM_DEBUG = 04,    
  // Messages that contain information normally of use only when
  // debugging a program (decimal 4).

  LM_INFO = 010,	    
  // Informational messages (decimal 8).

  LM_NOTICE = 020,   
  // Conditions that are not error conditions, but that may require
  // special handling (decimal 16).

  LM_WARNING = 040,  
  // Warning messages (decimal 32).

  LM_STARTUP = 0100,  
  // Initialize the logger (decimal 64).

  LM_ERROR = 0200,    
  // Error messages (decimal 128).

  LM_CRITICAL = 0400,	    
  // Critical conditions, such as hard device errors (decimal 256).

  LM_ALERT = 01000,    
  // A condition that should be corrected immediately, such as a
  // corrupted system database (decimal 512).

  LM_EMERGENCY = 02000,
  // A panic condition.  This is normally broadcast to all users
  // (decimal 1024).

  LM_MAX = LM_EMERGENCY,
  // The maximum logging priority.
  
  // Do not use!!, this enum value ensures that the underlying
  // integral type for this enum is at least 32 bits.
  ENSURE_32_BITS = 0xFFFFFFFF
};

#endif /* ACE_LOG_PRIORITY_H */

