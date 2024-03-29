/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PMC_All.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _PMC_ALL_H
#define _PMC_ALL_H

#include "PM_Client.h"

/**
 * @class PMC_All
 *
 * @brief Provides the client's lookup table abstraction for `all' users...
 */
class PMC_All : public PM_Client
{
protected:
  virtual Protocol_Record *insert_protocol_info (Protocol_Record &protocol_record);
  virtual int encode (char *packet, int &total_bytes);
  virtual int decode (char *packet, int &total_bytes);

public:
  PMC_All ();
  virtual void process ();
};

#endif /* _PMC_ALL_H */
