// $Id$

#ifndef TODAY_H
#define TODAY_H

#include "ace/OS.h"
#include "Magazine.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Today : public Magazine
{
  // = TITLE 
  //   This is an derived class of Magazine.
  // 
  // = DESCRIPTION
  //   Polymoriphism is exploited and an object pointer
  //   of Magazine is bound to the Today object at runtime.
public:

  // @@ Kirthika, please put this comment underneath the title()
  // method.  Also, please make the comment more descriptive.

  // The virtual abstract class method.
  void title (void);
};

#endif /* TODAY_H */
