/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PMC_Flo.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _PMC_FLO_H
#define _PMC_FLO_H

#include "PM_Client.h"

/**
 * @class PMC_Flo
 *
 * @brief Provides the client's lookup table abstraction for `flo' users...
 */
class PMC_Flo : public PM_Client
{

public:
  PMC_Flo (void);
  virtual void process (void);

protected:
  virtual Protocol_Record *insert_protocol_info (Protocol_Record &protocol_record);
  virtual int encode (char *packet, int &total_bytes);
  virtual int decode (char *packet, int &total_bytes);
};

#endif /* _PMC_FLO_H */
