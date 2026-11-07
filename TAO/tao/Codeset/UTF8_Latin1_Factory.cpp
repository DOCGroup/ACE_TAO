#include "tao/debug.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"
#include "tao/Codeset/UTF8_Latin1_Factory.h"

#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_UTF8_Latin1_Factory,
                       ACE_TEXT ("UTF8_Latin1_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_UTF8_Latin1_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Codeset, TAO_UTF8_Latin1_Factory)

TAO_UTF8_Latin1_Factory::~TAO_UTF8_Latin1_Factory () = default;

int
TAO_UTF8_Latin1_Factory::init (int argc, ACE_TCHAR *argv[])
{
  TAO_Codeset_Translator_Factory::init (argc, argv);
  return 0;
}

CONV_FRAME::CodeSetId
TAO_UTF8_Latin1_Factory::ncs () const
{
  return this->create_translator () ? this->translator_->ncs () : 0;
}

CONV_FRAME::CodeSetId
TAO_UTF8_Latin1_Factory::tcs () const
{
  return this->create_translator () ? this->translator_->tcs () : 0;
}

// Assign either a reference to the existing translator or a new translator
// for input CDR streams
void
TAO_UTF8_Latin1_Factory::assign (TAO_InputCDR *cdr) const
{
  if (cdr)
    {
      if (this->create_translator ())
        {
          this->assign_i (cdr, this->translator_.get ());
        }
    }
}

// Assign either a reference to the existing translator or a new translator
// for output CDR streams
void
TAO_UTF8_Latin1_Factory::assign (TAO_OutputCDR *cdr) const
{
  if (cdr)
    {
      if (this->create_translator ())
        {
          this->assign_i (cdr, this->translator_.get ());
        }
    }
}

bool
TAO_UTF8_Latin1_Factory::create_translator () const
{
  if (!this->translator_)
    {
      this->translator_.reset (new (std::nothrow) TAO_UTF8_Latin1_Translator);
      if (!this->translator_)
        {
          if (TAO_debug_level)
            TAOLIB_ERROR ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) TAO_UTF8_Latin1_Factory cannot ")
                           ACE_TEXT ("create TAO_UTF8_Latin1_Translator\n")));
        }
    }

  return this->translator_ != nullptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL
