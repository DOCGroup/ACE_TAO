// -*- C++ -*-

//=============================================================================
/**
 * @file      BE_File_Generator.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BE_FILE_GENERATOR_H_
#define _CUTS_BE_FILE_GENERATOR_H_

#include "BE_export.h"
#include "PICML/PICML.h"

// Forward decl.
class CUTS_BE_Preprocess_Data;

//=============================================================================
/**
 * @class CUTS_BE_File_Generator
 *
 * Base class for file generators. It defines all the methods that
 * must be overloaded to be used by the CUTS_BE_File_Generator_T class.
 */
//=============================================================================

class CUTS_BE_Export CUTS_BE_File_Generator
{
public:
  /// Destructor.
  virtual ~CUTS_BE_File_Generator (void) = 0;

  /**
   * Initialize the file generator with the preprocessing data for the
   * current file.
   *
   * @param[in]       ppd       Pointer to the preprocessing data.
   */
  void init (const CUTS_BE_Preprocess_Data * ppd);

  /**
   * Open a file for writing.
   *
   * @param[in]     path       Pathname of the file, excluding extension.
   */
  virtual void open (const std::string & pathname);

  /// Close the output file.
  virtual void close (void);

  /**
   * Write the preamble for the file.
   *
   * @param[in]     container       Top-most container of implementation.
   */
  virtual void write_preamble (
    const PICML::ComponentImplementationContainer & container);

  /**
   * Write the postamble for the file.
   *
   * @param[in]     container       Top-most container of implementation.
   */
  virtual void write_postamble (
    const PICML::ComponentImplementationContainer & container);

  virtual void write_include_file (const std::string & str);

  /**
   * Write a single line comment.
   *
   * @param[in]     comment       Comment string.
   */
  virtual void write_single_line_comment (const std::string & comment);

  virtual void write_monolithic_begin (
    const PICML::MonolithicImplementation &);

  virtual void write_monolithic_end (void);

  virtual void write_component_begin (
    const PICML::Component & component);

  virtual void write_component_end (
    const PICML::Component & component);

  virtual void write_component_factory_begin (
    const PICML::ComponentFactory &,
    const PICML::Component &);

  virtual void write_component_factory_end (void);

  virtual void write_method_begin (
    const PICML::InEventPort & sink);

  virtual void write_method_begin (
    const PICML::ProvidedRequestPort & facet);

  virtual void write_method_begin (
    const PICML::PeriodicAction & periodic);

  virtual void write_method_begin (
    const PICML::ReadonlyAttribute & attr);

  virtual void write_method_begin (
    const PICML::Attribute & attr);

  virtual void write_method_end (void);

  virtual void write_action_begin (
    const PICML::Worker & parent,
    const PICML::Action & action);

  virtual void write_action_begin (
    const PICML::OutputAction &);

  virtual void write_action_end (void);

  virtual void write_action_property (
    const PICML::Property &);

  virtual void write_precondition (
    const std::string & precondition);

  virtual void write_postcondition (
    const std::string & postcondition);

  virtual void write_variables_begin (void);

  virtual void write_variable (const PICML::Variable & variable);

  virtual void write_variable (const PICML::WorkerType & worker_type);

  virtual void write_variable (const PICML::ReadonlyAttribute & attr);

  virtual void write_variable (const PICML::Attribute & attr);

  virtual void write_variables_end (void);

protected:
  /**
   * Default constructor.
   *
   * @param[in]     out       Target output stream.
   */
  CUTS_BE_File_Generator (void);

  /**
   * Helper method to convert a string to uppercase.
   *
   * @param[in]       str         Source string.
   * @return          \a str in uppercase.
   */
  static void to_upper (std::string & str);

  /**
   * Helper method to convert a string to lowercase.
   *
   * @param[in]       str         Source string.
   * @return          \a str in lowercase.
   */
  static void to_lower (std::string & str);

  /**
   * Get a reference to the preprocessing data.
   */
  const CUTS_BE_Preprocess_Data & preprocess_data (void) const;

private:
  /// Reference to the preprocessing data.
  const CUTS_BE_Preprocess_Data * ppd_;
};

#if defined (__CUTS_INLINE__)
#include "BE_File_Generator.inl"
#endif

#endif  // !define _CUTS_BE_FILE_GENERATOR_H_
