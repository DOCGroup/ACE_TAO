// -*- C++ -*-

//=============================================================================
/**
 * @file      Preprocess_Data.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BE_PREPROCESS_DATA_H_
#define _CUTS_BE_PREPROCESS_DATA_H_

#include "BE_export.h"
#include "String_Set.h"
#include "PICML/PICML.h"

//=============================================================================
/**
 * @class CUTS_BE_Preprocess_Data
 *
 * Preprocessor for components.
 */
//=============================================================================

class CUTS_BE_Export CUTS_BE_Preprocess_Data : public PICML::Visitor
{
public:
  /// Default constructor.
  CUTS_BE_Preprocess_Data (void);

  /// Destructor.
  ~CUTS_BE_Preprocess_Data (void);

  /**
   * Get the collection of headers.
   *
   * @return      Collection of strings.
   */
  CUTS_String_Set & headers (void);
  const CUTS_String_Set & headers (void) const;

  /**
   * Get the collection of include files.
   *
   * @return      Collection of strings.
   */
  CUTS_String_Set & includes (void);
  const CUTS_String_Set & includes (void) const;

  /**
   * Get the collection of import libraries.
   *
   * @return      Collection of strings.
   */
  CUTS_String_Set & libs (void);
  const CUTS_String_Set & libs (void) const;

  /**
   * Get the collection of import library paths.
   *
   * @return      Collection of strings.
   */
  CUTS_String_Set & libpaths (void);
  const CUTS_String_Set & libpaths (void) const;

  /// Clear all the data.
  void clear (void);

protected:
  void Visit_Component (
    const PICML::Component & component);

  void Visit_WorkerType (
    const PICML::WorkerType &);

  void Visit_Worker (
    const PICML::Worker &);

  void Visit_WorkerPackage (
    const PICML::WorkerPackage &);

  void Visit_WorkerFile (
    const PICML::WorkerFile &);

  void Visit_WorkerLibrary (
    const PICML::WorkerLibrary &);

private:
  /// Collection of required headers.
  CUTS_String_Set headers_;

  /// Collection of include paths.
  CUTS_String_Set includes_;

  /// Collection of import libraries.
  CUTS_String_Set libs_;

  /// Collection of import library paths.
  CUTS_String_Set libpaths_;

  // prevent the following operations
  CUTS_BE_Preprocess_Data (const CUTS_BE_Preprocess_Data &);
  const CUTS_BE_Preprocess_Data & operator = (const CUTS_BE_Preprocess_Data &);
};

#if defined (__CUTS_INLINE__)
#include "Preprocess_Data.inl"
#endif

#endif  // !defined _CUTS_BE_PREPROCESS_DATA_H_