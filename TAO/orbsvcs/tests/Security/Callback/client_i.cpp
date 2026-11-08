#include "ace/Log_Msg.h"
#include "client_i.h"

client_i::client_i (server_ptr s)
  : server_ (server::_duplicate (s))
{
}

void
client_i::test_reply (char const *msg)
{
  ACE_DEBUG((LM_INFO,
             ACE_TEXT("(%N) message received was ")
             ACE_TEXT("(%s)\n"),
             msg));
}
