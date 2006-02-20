#include "ace/Log_Msg.h"
#include "client_i.h"

ACE_RCSID (Callback,
           client_i,
           "$Id$")

client_i::client_i (server_ptr s)
  : server_ (server::_duplicate (s))
{
}

client_i::~client_i (void)
{
}

void
client_i::test_reply (const char *msg
                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG((LM_INFO,
             ACE_TEXT("(%N) message received was ")
             ACE_TEXT("(%s)\n"),
             msg));
}
