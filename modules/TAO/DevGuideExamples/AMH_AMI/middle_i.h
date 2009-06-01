/* -*- C++ -*- $Id$ */

#ifndef MIDDLE_I_H_
#define MIDDLE_I_H_

#include "middleS.h"
#include "innerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Asynch_Middle_i : public virtual POA_AMH_Middle
{
public:
  //Constructor
  Asynch_Middle_i (PortableServer::POA_ptr poa, Inner_ptr inner);

  virtual void get_the_answer (AMH_MiddleResponseHandler_ptr _tao_rh,
                               const char * question);

private:
  PortableServer::POA_var poa_;
  Inner_var inner_;
};

// This version of the implementation does not use AMH, It is supplied for
// comparison. Either this or the asynch version may be used to serve
// "Middle" objects.
class  Middle_i : public virtual POA_Middle
{
public:
  //Constructor
  Middle_i (Inner_ptr inner);

  virtual char * get_the_answer (const char * question);

private:
  PortableServer::POA_var poa_;
  Inner_var inner_;
};


#endif /* MIDDLE_I_H_  */
