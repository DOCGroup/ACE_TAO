// Local_Tokens_T.cpp
// $Id$

#if 0
#if !defined (ACE_LOCAL_TOKENS_T_C)
#define ACE_LOCAL_TOKENS_T_C

#define ACE_BUILD_DLL
#include "ace/Local_Tokens_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Local_Tokens_T.i"
#endif /* __ACE_INLINE__ */

template <class TYPE>
ACE_Token_Name<TYPE>::ACE_Token_Name (void)
{
  ACE_TRACE ("ACE_Token_Name::ACE_Token_Name");
}

template <class TYPE>
ACE_Token_Name<TYPE>::ACE_Token_Name (const char *token_name, 
				      TYPE type)
{ 
  ACE_TRACE ("ACE_Token_Name::ACE_Token_Name");
  this->type (type);
  this->name (token_name);
}

template <class TYPE>
ACE_Token_Name<TYPE>::ACE_Token_Name (const ACE_Token_Name<TYPE> &rhs)
{
  ACE_TRACE ("ACE_Token_Name::ACE_Token_Name");
  this->type (rhs.type ());
  this->name (rhs.name ());
}

template <class TYPE>
ACE_Token_Name<TYPE>::~ACE_Token_Name ()
{
  ACE_TRACE ("ACE_Token_Name::~ACE_Token_Name");
}

template <class TYPE> void
ACE_Token_Name<TYPE>::dump (void) const
{
  ACE_TRACE ("ACE_Token_Name::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "ACE_Token_Name::dump:\n"
			" token_name_ = %s\n"
			" type_ = %d\n",
			token_name_, (int) type_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

#endif /* ACE_LOCAL_TOKENS_T_C */
#endif /* 0 */
