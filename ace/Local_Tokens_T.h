/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Local_Tokens_T
//
// = AUTHOR
//    Tim Harrison (harrison@cs.wustl.edu)
//
// = DESCRIPTION
//   This file contains definitions for the following classes:
//
// 	public:
//	  ACE_Token_Name
//   
// ============================================================================

#if !defined (ACE_LOCAL_TOKENS_T_H)
#define ACE_LOCAL_TOKENS_T_H

#include "ace/ACE.h"

template <class TYPE>
class ACE_Token_Name
  // = TITLE
  //   Allows Token_Manger to identify tokens.
  //
  // = DESCRIPTION
  //   A 2-tuple of token name and token type.  For now, we're going
  //   to use int's instead of ACE_Token_Manager::TOKEN_TYPE.  This is
  //   because of the different ways the compilers are instantiating
  //   the templates in the token manager.
{
public:
  ACE_Token_Name (void);
  // for map manager

  ACE_Token_Name (const char *token_name,
		  TYPE type);
  // construction

  ACE_Token_Name (const ACE_Token_Name<TYPE> &rhs);
  // copy construction

  ~ACE_Token_Name (void);
  // death

  void operator= (const ACE_Token_Name<TYPE> &rhs);
  // copy

  int operator== (const ACE_Token_Name<TYPE> &rhs) const;
  // comparison

  const char *name (void) const;
  // token name

  void name (const char *new_name);
  // token name

  TYPE type (void) const;
  // token type

  void type (TYPE type);
  // token type

  void dump (void) const;
  // Dump the state of the class.

private:
  char token_name_[ACE_MAXTOKENNAMELEN];
  // Name of the token.
  
  TYPE type_;
  // Type of the token.
};

#if defined (__ACE_INLINE__)
#include "ace/Local_Tokens_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Local_Tokens_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Local_Tokens_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_LOCAL_TOKENS_T_H */
