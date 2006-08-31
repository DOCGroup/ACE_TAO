// -*- C++ -*-

//=============================================================================
/**
 * @file      File_Generator_Base.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_CIAO_FILE_GENERATOR_BASE_H_
#define _CUTS_CIAO_FILE_GENERATOR_BASE_H_

#include "UDM_CIAO_Export.h"
#include "cuts/be/BE_File_Generator.h"
#include <fstream>
#include <memory>

namespace Indentation
{
  // Forward decl.
  template <typename T> class Cxx;

  // Forward decl.
  template <template <typename> class BufferType,
            typename C> class Implanter;
}

//=============================================================================
/**
 * @class CUTS_CIAO_File_Generator_Base
 */
//=============================================================================

class CUTS_UDM_CIAO_Export CUTS_CIAO_File_Generator_Base :
  public CUTS_BE_File_Generator
{
public:
  /// Default constructor.
  CUTS_CIAO_File_Generator_Base (void);

  /// Destructor.
  virtual ~CUTS_CIAO_File_Generator_Base (void);

  /// Close the file.
  virtual void close (void);

  virtual void write_single_line_comment (
    const std::string & comment);

  virtual void write_monolithic_begin (
    const PICML::MonolithicImplementation &);

  virtual void write_monolithic_end (void);

  virtual void write_component_end (
    const PICML::Component & component);

  virtual void write_include_file (
    const std::string & str);

  virtual void write_method_begin (
    const PICML::InEventPort & sink);

  virtual void write_method_begin (
    const PICML::ProvidedRequestPort & facet);

protected:
  void write_scope (const PICML::NamedType & named_type,
                    const std::string & seperator,
                    std::string * out = 0);

  virtual void write_set_session_context (const PICML::Component & component);

  virtual void write_ciao_preactivate (const PICML::Component & component);

  virtual void write_ccm_activate (const PICML::Component & component);

  virtual void write_ciao_postactivate (const PICML::Component & component);

  virtual void write_ccm_passivate (const PICML::Component & component);

  virtual void write_ccm_remove (const PICML::Component & component);

  /**
   * Shared implemenation for opening the specified filename. This
   * assumes the filename has the necessary extenstion.
   *
   * @param[in]       filename        Full pathname of the file.
   */
  void open_i (const std::string & filename);

  /// Name of the object that has operations.
  std::string object_name_;

  /// Output stream for the file generator.
  std::ofstream out_;

  /// Indentation implanter.
  typedef Indentation::Implanter <Indentation::Cxx, char> Formatter_Type;

  std::auto_ptr <Formatter_Type> formatter_;
};

#endif  // !defined _CUTS_CIAO_FILE_GENERATOR_BASE_H_
