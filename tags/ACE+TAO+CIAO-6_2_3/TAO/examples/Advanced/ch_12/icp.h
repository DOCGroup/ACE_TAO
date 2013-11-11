
//=============================================================================
/**
 *  @file    icp.h
 *
 *  $Id$
 *
 *  @author Source code used in TAO has been modified and adapted from thecode provided in the book
 *  @author "Advanced CORBA Programming with C++"by Michi Henning and Steve Vinoski. Copyright1999. Addison-Wesley
 *  @author Reading
 *  @author MA.  Used with permission ofAddison-Wesley.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================


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
