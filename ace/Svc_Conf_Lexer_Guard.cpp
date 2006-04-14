// -*- C++ -*-

#include "ace/Svc_Conf_Lexer_Guard.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 1)

#include "ace/Svc_Conf.h"

ACE_RCSID (ace,
           Svc_Conf_Lexer_Guard,
           "$Id$")

ACE_Svc_Conf_Lexer_Guard::ACE_Svc_Conf_Lexer_Guard (ACE_Svc_Conf_Param *param)
  : buffer_ (0)
{
  // External synchronization is required.

  // Note that allocation/deallocation is done once during the
  // processing of a service configurator directive.  Memory
  // managements is done at a higher level, not in this class.  This
  // is necessary to prevent an allocation/deallocation from occurring
  // when parsing/scanning each token.

  if (param->type == ACE_Svc_Conf_Param::SVC_CONF_FILE)
    ::ace_yy_push_buffer (param->source.file, param->buffer);
  else
    ::ace_yy_push_buffer (param->source.directive,
                          param->buffer);

  this->buffer_ = param->buffer;
}

ACE_Svc_Conf_Lexer_Guard::~ACE_Svc_Conf_Lexer_Guard (void)
{
  // External synchronization is required.

  ::ace_yy_pop_buffer (this->buffer_);
}
#endif /* ACE_USES_CLASSIC_SVC_CONF == 1 */
