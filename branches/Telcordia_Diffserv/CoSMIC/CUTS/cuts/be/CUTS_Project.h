// -*- C++ -*-

#ifndef _CUTS_PROJECT_H_
#define _CUTS_PROJECT_H_

#include "BE_export.h"
#include "PICML/PICML.h"

class CUTS_BE_Export CUTS_Project :
  public PICML::Visitor
{
public:
  const PICML::Object & get_testing_service (void) const;

  const PICML::File & get_cuts_file (void) const;

  bool is_valid (void) const;

  const std::string & message (void) const;

  static CUTS_Project * instance (void);

  static void close (void);

private:
  CUTS_Project (void);

  virtual ~CUTS_Project (void);

  void Visit_RootFolder (const PICML::RootFolder &);
  void Visit_InterfaceDefinitions (const PICML::InterfaceDefinitions &);
  void Visit_Package (const PICML::Package &);
  void Visit_File (const PICML::File &);

  PICML::Object testing_service_;

  PICML::File cuts_file_;

  bool valid_;

  std::string message_;

  /// Singleton instance of the project.
  static CUTS_Project * instance_;
};

#endif  /* !defined _CUTS_PROJECT_H_ */
