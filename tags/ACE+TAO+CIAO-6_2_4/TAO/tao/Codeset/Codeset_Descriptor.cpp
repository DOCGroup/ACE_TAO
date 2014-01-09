
//=============================================================================
/**
 *  @file    Codeset_Descriptor.cpp
 *
 *  $Id$
 *
 *  The base for all the translator factories. Translator factories are
 *  responsible for supplying the proper translator on demand.
 *
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#include "tao/Codeset/Codeset_Descriptor.h"
#include "tao/Codeset/Codeset_Translator_Factory.h"

#include "ace/Codeset_Registry.h"
#include "ace/Log_Msg.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Codeset_Descriptor::TAO_Codeset_Descriptor ()
  :ncs_ (0),
   max_bytes_ (1),
   num_translators_ (0),
   trans_base_(0)
{
}

TAO_Codeset_Descriptor::~TAO_Codeset_Descriptor ()
{
  Translator_Node *temp = trans_base_;
  while (temp)
    {
      temp = trans_base_->next_;
      // don't need to delete the associated translator factory, it is
      // owned by the service registry
      ACE_OS::free (trans_base_->name_);
      delete trans_base_;
      trans_base_ = temp;
    }
}

void
TAO_Codeset_Descriptor::ncs (const ACE_TCHAR *name)
{
  ACE_CDR::ULong n = 0;
  if (ACE_Codeset_Registry::locale_to_registry
      (ACE_TEXT_ALWAYS_CHAR(name), n) == 0)
    {
      char **endPtr = 0;
      n = static_cast<ACE_CDR::ULong> (
            ACE_OS::strtoul(ACE_TEXT_ALWAYS_CHAR(name), endPtr, 0));
    }
  this->ncs(n);
}

void
TAO_Codeset_Descriptor::ncs (ACE_CDR::ULong n)
{
  this->ncs_ = n;
  // Validate the CodesetId
  this->max_bytes_ = ACE_Codeset_Registry::get_max_bytes(n);
  if (this->max_bytes_ == 0)
    {
      if (TAO_debug_level > 0)
        TAOLIB_ERROR((LM_ERROR,
                   ACE_TEXT("(%P|%t) TAO_Codeset_Descriptor::ncs, ")
                   ACE_TEXT("unknown codeset id 0x%x\n"),
                   n));
      this->ncs_ = 0;
    }
}

ACE_CDR::ULong
TAO_Codeset_Descriptor::ncs (void) const
{
  return this->ncs_;
}

int
TAO_Codeset_Descriptor::num_translators (void) const
{
  return this->num_translators_;
}

int
TAO_Codeset_Descriptor::max_bytes (void) const
{
  return this->max_bytes_;
}

void
TAO_Codeset_Descriptor::add_translator (const ACE_TCHAR *name)
{
  Translator_Node *temp = trans_base_;
  if (this->trans_base_ == 0)
    {
      ACE_NEW (this->trans_base_, Translator_Node);
      temp = trans_base_;
    }
  else
    {
      while (temp->next_ != 0)
        temp = temp->next_;
      ACE_NEW (temp->next_, Translator_Node);
      temp = temp->next_;
    }
  if (temp)
    {
      this->num_translators_ ++;
      temp->name_ = ACE_OS::strdup (name);
      temp->translator_factory_ = 0;
      temp->next_ = 0;
    }
}

TAO_Codeset_Descriptor::Translator_Node *
TAO_Codeset_Descriptor::translators (void)
{
  return this->trans_base_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
