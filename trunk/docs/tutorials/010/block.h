// $Id$

#ifndef BLOCK_H
#define BLOCK_H

#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* This simple ACE_Message_Block derivative will inform us of it's
   construction and destruction.  We'll use this to assure ourselves
   that we don't have any memory leaks.  In a real application, of
   course, this isn't necessary.  */
class Block : public ACE_Message_Block
{
public:
  Block (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Block ctor 0x%x\n",
                (void *) this));
  }

  Block (size_t size)
    : ACE_Message_Block (size)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Block ctor 0x%x\n",
                (void *) this));
  }

  virtual ~Block (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Block dtor 0x%x\n",
                (void *) this));
  }
};

#endif /* BLOCK_H */
