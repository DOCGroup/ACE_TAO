// $Id$

//#define ACE_COMPILE_TIMEPROBES
//#define ACE_ENABLE_TIMEPROBES
//#define ACE_MT_TIMEPROBES
//#define ACE_TSS_TIMEPROBES

#include "ace/Timeprobe.h"

static const char *TAO_Timeprobe_Description[] = 
{ 

  "TAO Timeprobe Description Table",

  "GIOP::send_request - start",
  "GIOP::send_request - end",

  "GIOP::recv_request - start",
  "GIOP::recv_request - end",

  "GIOP::read_buffer - start",
  "GIOP::read_buffer - end",

  "GIOP::RequestHeader_init - start",
  "GIOP::RequestHeader_init - end",

  "GIOP::LocateRequestHeader_init - start",
  "GIOP::LocateRequestHeader_init - end",

  "Server_Connection_Handler::send_response - start",
  "Server_Connection_Handler::send_response - end",

  "Server_Connection_Handler::handle_input - start",
  "Server_Connection_Handler::handle_input - end",

  "Client_Connection_Handler::send_request - start",
  "Client_Connection_Handler::send_request - end",

  "CORBA_ORB::run - start",
  "CORBA_ORB::run - end",

  "POA::locate_poa_i - start",
  "POA::locate_poa_i - end",

  "POA::locate_poa_and_servant_i - start",
  "POA::locate_poa_and_servant_i - end",

  "POA::find_servant - start",
  "POA::find_servant - end",

  "POA::dispatch_servant - start",
  "POA::dispatch_servant - end",

  "POA::parse_key - start",
  "POA::parse_key - end",

  "Servant::_dispatch - start",
  "Servant::_dispatch - end",


};

enum 
{
  TAO_TIMEPROBE_DESCRIPTION_TABLE,

  TAO_GIOP_SEND_REQUEST_START,
  TAO_GIOP_SEND_REQUEST_END,

  TAO_GIOP_RECV_REQUEST_START,
  TAO_GIOP_RECV_REQUEST_END,

  TAO_GIOP_READ_BUFFER_START,
  TAO_GIOP_READ_BUFFER_END,

  TAO_GIOP_REQUEST_HEADER_INIT_START,
  TAO_GIOP_REQUEST_HEADER_INIT_END,

  TAO_GIOP_LOCATE_REQUEST_HEADER_INIT_START,
  TAO_GIOP_LOCATE_REQUEST_HEADER_INIT_END,

  TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_START,
  TAO_SERVER_CONNECTION_HANDLER_SEND_RESPONSE_END,

  TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_START,
  TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_END,

  TAO_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_START,
  TAO_CLIENT_CONNECTION_HANDLER_SEND_REQUEST_END,

  TAO_CORBA_ORB_RUN_START,
  TAO_CORBA_ORB_RUN_END,

  TAO_POA_LOCATE_POA_I_START,
  TAO_POA_LOCATE_POA_I_END,

  TAO_POA_LOCATE_POA_AND_SERVANT_I_START,
  TAO_POA_LOCATE_POA_AND_SERVANT_I_END,

  TAO_POA_FIND_SERVANT_START,
  TAO_POA_FIND_SERVANT_END,

  TAO_POA_DISPATCH_SERVANT_START,
  TAO_POA_DISPATCH_SERVANT_END,

  TAO_POA_PARSE_KEY_START,
  TAO_POA_PARSE_KEY_END,

  TAO_SERVANT_DISPATCH_START,
  TAO_SERVANT_DISPATCH_END,

  TAO_LAST_TIMEPROBE_ENTRY

};
