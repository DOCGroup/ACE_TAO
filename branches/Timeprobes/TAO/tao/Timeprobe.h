// $Id$

#define ACE_COMPILE_TIMEPROBES
#define ACE_ENABLE_TIMEPROBES
//#define ACE_MT_TIMEPROBES
//#define ACE_TSS_TIMEPROBES

#include "ace/Timeprobe.h"

static const char *TAO_Timeprobe_Description[] = 
{ 

  "TAO Timeprobe Description Table",

  "  -> GIOP::send_request - start",
  "  -> GIOP::send_request - fail",
  "  -> GIOP::send_request - end",

  "  -> GIOP::recv_request - start",
  "  -> GIOP::recv_request - fail",
  "  -> GIOP::recv_request - eof",
  "  -> GIOP::recv_request - end",

  "  -> Server_Connection_Handler::send_response - start",
  "  -> Server_Connection_Handler::send_response - end",
  "  -> Server_Connection_Handler::handle_input start",
  "  -> Server_Connection_Handler::handle_input end",

  "  -> Client_Connection_Handler::send_request - start",
  "  -> Client_Connection_Handler::send_request - end",

  "  -> CORBA_ORB::run start handling events",
  "  -> CORBA_ORB::run end events handled",

};

enum 
{
  TAO_TIMEPROBE_DESCRIPTION_TABLE,

  TAO_GIOP_SEND_REQUEST_START,
  TAO_GIOP_SEND_REQUEST_FAIL,
  TAO_GIOP_SEND_REQUEST_END,

  TAO_GIOP_RECV_REQUEST_START,
  TAO_GIOP_RECV_REQUEST_FAIL,
  TAO_GIOP_RECV_REQUEST_EOF,
  TAO_GIOP_RECV_REQUEST_END,

  TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_START,
  TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_END,
  TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_START,
  TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_END,

  TAO_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_START,
  TAO_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_END,

  TAO_CORBA_ORB_RUN_START,
  TAO_CORBA_ORB_RUN_END,



  TAO_LAST_TIMEPROBE_ENTRY

};
