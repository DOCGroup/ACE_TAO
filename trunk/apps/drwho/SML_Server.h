/* -*- C++ -*- */
// $Id$

#ifndef _SML_SERVER_H
#define _SML_SERVER_H

#include "SM_Server.h"

class SML_Server : public SM_Server
{
public:
              SML_Server (void);
  virtual    ~SML_Server (void);
};

#ifdef __OPTIMIZE__
inline
SML_Server::SML_Server (void)
{}

inline
SML_Server::~SML_Server (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _SML_SERVER_H */
