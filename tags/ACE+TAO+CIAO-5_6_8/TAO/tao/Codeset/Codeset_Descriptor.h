// -*- C++ -*-

//=============================================================================
/**
 *  @file   Codeset_Descriptor.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier
 */
//=============================================================================

#ifndef TAO_CODESET_DESCRIPTOR_H
#define TAO_CODESET_DESCRIPTOR_H

#include /**/ "ace/pre.h"

#include "tao/Codeset/codeset_export.h"
#include "ace/CDR_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Codeset_Descriptor_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Codeset_Translator_Factory;

class TAO_Codeset_Export TAO_Codeset_Descriptor
  : public TAO_Codeset_Descriptor_Base
{
public:
  TAO_Codeset_Descriptor ();
  virtual ~TAO_Codeset_Descriptor ();

  struct Translator_Node
  {
    ACE_TCHAR *name_;
    TAO_Codeset_Translator_Factory *translator_factory_;
    Translator_Node *next_;
  };

  void ncs (ACE_CDR::ULong ncs);
  void ncs (const ACE_TCHAR *name);
  ACE_CDR::ULong ncs (void) const;
  int max_bytes (void) const;
  int num_translators (void) const;

  void add_translator (const ACE_TCHAR *name);
  Translator_Node *translators (void);

private:
  ACE_CDR::ULong ncs_;
  int max_bytes_;
  int num_translators_;
  Translator_Node *trans_base_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_CODESET_DESCRIPTOR_H */
