#include "tao/XtResource/XtResource_Factory.h"
#include "tao/debug.h"
#include "ace/XtReactor/XtReactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  XtResource_Factory::XtResource_Factory (XtAppContext p_context):
    reactor_impl_(nullptr),
    context_(p_context)
  {
  }

  ACE_Reactor_Impl *
  XtResource_Factory::reactor_impl ()
  {
    // synchronized by external locks
    if (!this->context_)
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT("TAO (%P|%t) - XtAppContext is undefined.")
                  ACE_TEXT(" Cannot create ACE_XtReactor\n")));
      return nullptr; // Private and undefined;
    }

    if (!this->reactor_impl_)
    {
      ACE_NEW_RETURN (this->reactor_impl_,
                      ACE_XtReactor (this->context_),
                      nullptr);

      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - ACE_XtReactor created\n"));
    }

    return this->reactor_impl_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
