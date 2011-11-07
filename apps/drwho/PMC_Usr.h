/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PMC_Usr.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _PMC_USR_H
#define _PMC_USR_H

#include "PM_Client.h"

/**
 * @class PMC_Usr
 *
 * @brief Provides the client's lookup table abstraction for `Usr' users...
 */
class PMC_Usr : public PM_Client
{
public:
  PMC_Usr (char *usr_name);
  virtual void process (void);

protected:
  virtual int encode (char *packet, int &total_bytes);
  virtual int decode (char *packet, int &total_bytes);

private:
  char *usr_name;
};

#endif /* _PMC_USR_H */
