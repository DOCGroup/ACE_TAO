/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Multiplexor.h
//
// Define the Driver and ACE_Multiplexor container classes.
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================


#if !defined (ACE_MULTIPLEXOR_H)
#define ACE_MULTIPLEXOR_H

#include "ace/Module.h"
#include "ace/Map_Manager.h"

#if defined (ACE_HAS_THREADS)

#if 0
class Driver
  // = TITLE
  //
  //
  // = DESCRIPTION
  //
{
public:
  Driver (void);
  ~Driver (void);
  
  virtual int link_from_below (ACE_Module *mod);
  virtual ACE_Module *alloc_module (Driver *) = 0;
  virtual int unlink_from_below (ACE_Module *);
};

class ACE_Export ACE_Multiplexor
  // = TITLE
  //
  //
  // = DESCRIPTION
  //
{
public:
  // = Constructors and destructors 
  ACE_Multiplexor (void);
  ~ACE_Multiplexor (void);

  virtual int link_from_above (Driver &ld);
  virtual int link_from_above (ACE_Multiplexor &lm);
  virtual int link_from_below (ACE_Module *mod);
  virtual ACE_Module *alloc_lower_module (ACE_Multiplexor *) = 0;
  virtual ACE_Module *alloc_upper_module (ACE_Multiplexor *) = 0;

  virtual int unlink_from_above (Driver &ld);
  virtual int unlink_from_above (ACE_Multiplexor &lm);
  virtual int unlink_from_below (ACE_Module *mod);
};

#if defined (__ACE_INLINE__)
#include "ace/Multiplexor.i"
#endif /* __ACE_INLINE__ */

#endif /* 0 */

#endif /* ACE_HAS_THREADS */
#endif /* ACE_MULTIPLEXOR_H */
