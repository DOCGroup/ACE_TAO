// $Id$

#ifndef TOKEN_I_H
#define TOKEN_I_H

#include "Test_T.h"

// Go get ace/Token.h so that we know what an ACE_Token is.
#include "ace/Token.h"

/* Create a very simple derivative of our Test template.  All we have
   to do is provide our mutex choice and a name.
 */
class Token : public Test_T<ACE_Token>
{
public:
  Token (void): Test_T<ACE_Token> ("Token") {}
};

#endif /* TOKEN_I_H */
