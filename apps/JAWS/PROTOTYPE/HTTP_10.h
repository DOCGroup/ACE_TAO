/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_HTTP_10_H)
#define JAWS_HTTP_10_H

#include "JAWS/Data_Block.h"

class JAWS_HTTP_10_Read_Task : public JAWS_Pipeline_Handler
{
public:
  JAWS_HTTP_10_Read_Task (void);
  virtual ~JAWS_HTTP_10_Read_Task (void);

  virtual int handle_put (JAWS_Data_Block *data, ACE_Time_Value *tv);

private:
};

class JAWS_HTTP_10_Write_Task : public JAWS_Pipeline_Handler
{
public:
  JAWS_HTTP_10_Write_Task (void);
  virtual ~JAWS_HTTP_10_Write_Task (void);

  virtual int handle_put (JAWS_Data_Block *data, ACE_Time_Value *tv);

private:
};

#endif /* !defined (JAWS_HTTP_10_H) */
