// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Advanced/ch_12
//
// = FILENAME
//    icp.h
//
// = AUTHORS
//   Source code used in TAO has been modified and adapted from the
//   code provided in the book, "Advanced CORBA Programming with C++"
//   by Michi Henning and Steve Vinoski. Copyright
//   1999. Addison-Wesley, Reading, MA.  Used with permission of
//   Addison-Wesley.
//
//   Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
//
// ============================================================================

#ifndef _ICP_H
#define _ICP_H

extern "C"
{
  int ICP_online (unsigned long id);   // Add device
  int ICP_offline (unsigned long id);  // Remove device

  // Get attribute
  int ICP_get (unsigned long id,
               const char * attr,
               void *value,
               size_t len);
  // Set attribute
  int ICP_set (unsigned long id,
               const char *attr,
               const void *value);
}

#endif /* _ICP_H */
