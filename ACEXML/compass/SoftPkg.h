// $Id$

#ifndef ACEXML_SOFTPKG_H
#define ACEXML_SOFTPKG_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"
#include "ace/Unbounded_Set.h"

class ACEXML_SoftPkg
{
public:
  typedef ACEXML_String Localfile;
  typedef ACEXML_String Fileinarchive;
  typedef ACEXML_String EntryPoint;
  typedef ACEXML_String Processor;
  typedef ACEXML_String Compiler;
  typedef ACEXML_String ProgLang;
  typedef PropertyFile  Fileinarchive;

  struct Dependency
  {
    ACEXML_String type_;
    localfile     localfile_;
  };
  struct OperSys
  {
    ACEXML_String name_;
    ACEXML_String version_;
  };
  struct Code
  {
    ACEXML_String type_;
    Fileinarchive file_;
    EntryPoint    func_;
  };
  struct Implementation
  {
    ACEXML_String   id_;
    OperSys         os_;
    Processor       cpu_;
    Compiler        compiler_;
    ProgLang        lang_;
    PropertyFile    propfile_;
    Code            code_;
  };
  typedef ACE_Unbouded_Set<Dependency*> Dependencies;
  typedef ACE_Unbouded_Set<Implementation*> Implementations;

  ACEXML_SoftPkg();
  ~ACEXML_SoftPkg();
  int add_impl (const ACEXML_String& id);
  int add_dep (const ACEXML_String& type);
  Implementation* get_current_impl (void);
  const ACE_Unbouded_Set<Implementation*>& get_all_impl (void) const;
private:
  ACEXML_String name_;
  ACEXML_String version_;
  ACEXML_String pkgtype_;
  ACEXML_String title_;
  ACEXML_String author_;
  ACEXML_String company_;
  ACEXML_String webpage_;
  ACEXML_String description_;
  Dependency* current_dep_;
  Implementation* current_impl_;
  Implementations deps_;
  Dependencies impls_;
};

#include "ace/post.h"

#endif /* ACEXML_SOFTPKG_H */
