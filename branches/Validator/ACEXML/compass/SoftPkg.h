// $Id$

#ifndef ACEXML_SOFTPKG_H
#define ACEXML_SOFTPKG_H

#include "ace/pre.h"
#include "ACEXML/compass/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"
#include "ace/Unbounded_Set.h"

class Compass_Export ACEXML_SoftPkg
{
public:
  typedef ACEXML_String Localfile;
  typedef ACEXML_String Fileinarchive;
  typedef ACEXML_String EntryPoint;
  typedef ACEXML_String Processor;
  typedef ACEXML_String Compiler;
  typedef ACEXML_String ProgLang;

  struct PropertyFile
  {
    Fileinarchive file_;
    void dump(void) const;
  };

  struct Dependency
  {
    ACEXML_String type_;
    Localfile     localfile_;
    void dump(void) const;
  };
  struct OperSys
  {
    ACEXML_String name_;
    ACEXML_String version_;
    void dump(void) const;
  };
  struct Code
  {
    ACEXML_String type_;
    Fileinarchive file_;
    EntryPoint    func_;
    void dump(void) const;
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
    void dump(void) const;
  };
  typedef ACE_Unbounded_Set<Dependency*> Dependencies;
  typedef ACE_Unbounded_Set_Iterator<Dependency*> DEP_ITERATOR;
  typedef ACE_Unbounded_Set<Implementation*> Implementations;
  typedef ACE_Unbounded_Set_Iterator<Implementation*> IMPL_ITERATOR;

  ACEXML_SoftPkg();
  ~ACEXML_SoftPkg();
  int start_element (const ACEXML_Char* element, const ACEXML_String& id);
  void end_element (const ACEXML_Char* element);
  const Implementations& get_impls (void) const;
  const Dependencies& get_deps (void) const;
  int set (const ACEXML_Char* element,
           const ACEXML_String& attname,
           const ACEXML_String& value);
  void dump(void) const;
private:
  ACEXML_String name_;
  ACEXML_String version_;
  ACEXML_String pkgtype_;
  ACEXML_String title_;
  ACEXML_String description_;
  PropertyFile  propfile_;
  Code* current_code_;
  Dependency* current_dep_;
  Implementation* current_impl_;
  Implementations impls_;
  Dependencies deps_;
};

#include "ace/post.h"

#endif /* ACEXML_SOFTPKG_H */
