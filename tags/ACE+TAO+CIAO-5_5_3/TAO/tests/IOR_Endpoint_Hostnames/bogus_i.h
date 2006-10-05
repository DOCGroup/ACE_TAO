//
// $Id$
//

#ifndef bogus_h
#define bogus_h
#include /**/ "ace/pre.h"

#include "bogusS.h"

class bogus
  : public virtual POA_Test::bogus
{
public:
  bogus ();
  virtual ~bogus ();

  virtual void noop (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#include /**/ "ace/post.h"
#endif //bogus_h
