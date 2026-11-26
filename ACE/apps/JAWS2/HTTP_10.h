#ifndef JAWS_HTTP_10_H
#define JAWS_HTTP_10_H

#include "ace/RB_Tree.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "JAWS/Pipeline_Tasks.h"

// Forward declaration
class JAWS_HTTP_10_Request;

// Reading the initial request

class JAWS_HTTP_10_Read_Task : public JAWS_Pipeline_Handler
{
public:
  JAWS_HTTP_10_Read_Task ();
  ~JAWS_HTTP_10_Read_Task () override;

  int handle_put (JAWS_Data_Block *data, ACE_Time_Value *tv) override;

private:
};

// Parsing the request

class JAWS_HTTP_10_Parse_Task : public JAWS_Pipeline_Handler
{
public:
  JAWS_HTTP_10_Parse_Task ();
  ~JAWS_HTTP_10_Parse_Task () override;

  int handle_put (JAWS_Data_Block *data, ACE_Time_Value *tv) override;
};

// Write the response

class JAWS_HTTP_10_Write_Task : public JAWS_Pipeline_Handler
{
public:
  JAWS_HTTP_10_Write_Task ();
  ~JAWS_HTTP_10_Write_Task () override;

  int handle_put (JAWS_Data_Block *data, ACE_Time_Value *tv) override;

private:
};

// Helpers

/// Static functions to enhance the lives of HTTP programmers everywhere.
class JAWS_HTTP_10_Helper
{
public:
  JAWS_HTTP_10_Helper () = delete;

  /// Decode '%' escape codes in a URI.
  static char *HTTP_decode_string (char *path);
};

#endif /* !defined (JAWS_HTTP_10_H) */
