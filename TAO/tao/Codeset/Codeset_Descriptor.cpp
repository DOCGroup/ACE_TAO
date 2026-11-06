
//=============================================================================
/**
 *  @file    Codeset_Descriptor.cpp
 *
 *  The base for all the translator factories. Translator factories are
 *  responsible for supplying the proper translator on demand.
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#include "tao/Codeset/Codeset_Descriptor.h"
#include "tao/Codeset/Codeset_Translator_Factory.h"

#include "ace/Codeset_Registry.h"
#include "ace/Log_Msg.h"
#include "tao/debug.h"

#include <utility>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Codeset_Descriptor::~TAO_Codeset_Descriptor ()
  = default;

void
TAO_Codeset_Descriptor::ncs (ACE_TCHAR const *name)
{
  ACE_CDR::ULong n = 0;
  if (ACE_Codeset_Registry::locale_to_registry
      (ACE_TEXT_ALWAYS_CHAR(name), n) == 0)
    {
      char **endPtr = nullptr;
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
TAO_Codeset_Descriptor::ncs () const
{
  return this->ncs_;
}

int
TAO_Codeset_Descriptor::num_translators () const
{
  return static_cast<int> (this->translators_.size ());
}

int
TAO_Codeset_Descriptor::max_bytes () const
{
  return this->max_bytes_;
}

void
TAO_Codeset_Descriptor::add_translator (ACE_TCHAR const *name)
{
  Translator_Node node;
  node.name_ = name;
  this->translators_.push_back (std::move (node));
}

TAO_Codeset_Descriptor::Translator_List &
TAO_Codeset_Descriptor::translators ()
{
  return this->translators_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
