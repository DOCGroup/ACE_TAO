// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Pluggable_Messaging.h
//
// = DESCRIPTION
//     Interface for the TAO pluggable messaging framework.
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef _TAO_PLUGGABLE_MESSAGE_H_
#define _TAO_PLUGGABLE_MESSAGE_H_

class TAO_Export TAO_Pluggable_Message_Factory
{
  // = TITLE
  //   Generic definitions for the new Pluggable Messaging class.
  //
  // = DESCRIPTION
  //   
  
  TAO_Pluggable_Message_Factory (void);
  // Ctor

  virtual ~TAO_Pluggable_Message_Factory (void);
  // Dtor

};


#if defined (__ACE_INLINE__)
# include "tao/Pluggable_Messaging.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_PLUGGABLE_MESSAGE_H_*/
