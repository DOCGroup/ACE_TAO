// $Id$

#ifndef NEWSWEEK_H
#define NEWSWEEK_H

#include "ace/OS.h"
#include "Magazine.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Newsweek : public Magazine
{
  //= TITLE 
  //   This is an derived class of Magazine.
  // 
  //= DESCRIPTION
  //   Polymoriphism is exploited and an object pointer
  //   of Magazine is bound to the Newsweek object at runtime.
public:

  // @@ Kirthika, please put this comment underneath the title()
  // method.

  // This is the abstract class method which describes the magazine.
  void title (void);
};

# endif /* NEWSWEEK_H */
