// $Id$

#ifndef TAO_TRADER_TEST_UTILS_H
#define TAO_TRADER_TEST_UTILS_H

#include "TTestS.h"
#include "ace/Arg_Shifter.h"
#include "ace/Read_Buffer.h"
#include "orbsvcs/CosTradingC.h"
#include "orbsvcs/CosTradingReposC.h"
#include "orbsvcs/CosTradingDynamicC.h"

class TT_Info
{
public:

  static void dump_properties (const CosTrading::PropertySeq& prop_seq,
                               CORBA::Boolean print_dynamic = 1);
  // Dump the contents of this property sequence.

  enum INTERFACES
  {
    REMOTE_IO,
    PLOTTER,
    PRINTER,
    FILESYSTEM,
    PS_PRINTER
  };

  // = Test servants.

  class Remote_Output :
    public POA_TAO_Trader_Test::Remote_Output
  {
  public:

    Remote_Output (void) {}

    virtual void confirm (CORBA::Environment &)
      TAO_THROW_SPEC ((CORBA::SystemException)) {}
    // Method to test that the importer received a valid reference to
    // the exported object.
  };

  class Printer :
    public POA_TAO_Trader_Test::Printer
  {
  public:

    Printer (void) {}

    virtual void confirm (CORBA::Environment &)
      TAO_THROW_SPEC ((CORBA::SystemException)) {}
    // Method to test that the importer received a valid reference to
    // the exported object.
  };

  class Plotter :
    public POA_TAO_Trader_Test::Plotter
  {
  public:
    Plotter (void) {}

    virtual void confirm (CORBA::Environment &)
      TAO_THROW_SPEC ((CORBA::SystemException)) {}
    // Method to test that the importer received a valid reference to
    // the exported object.
  };

  class File_System :
    public POA_TAO_Trader_Test::File_System
  {
  public:
    File_System (void) {}

    virtual void confirm (CORBA::Environment &)
      TAO_THROW_SPEC ((CORBA::SystemException)) {}
    // Method to test that the importer received a valid reference to
    // the exported object.
  };

  class PostScript_Printer :
    public POA_TAO_Trader_Test::PostScript_Printer
  {
  public:
    PostScript_Printer (void) {}

    virtual void confirm (CORBA::Environment &)
      TAO_THROW_SPEC ((CORBA::SystemException)) {}
    // Method to test that the importer received a valid reference to
    // the exported object.
  };

#define NUM_TYPES 5
  static const char* INTERFACE_NAMES[];

  // = Remote IO property descriptions

  enum REMOTE_IO_PROPERTIES
  {
    NAME,
    LOCATION,
    DESCRIPTION,
    HOST_NAME,
    TRADER_NAME,
    MISCELLANEOUS
  };

  static const char* REMOTE_IO_NAME;
  static const char* REMOTE_IO_PROPERTY_NAMES[];

  // = Plotter property descriptions

  enum PLOTTER_PROPERTIES
  {
    PLOTTER_NUM_COLORS,
    PLOTTER_AUTO_LOADING,
    PLOTTER_COST_PER_PAGE,
    PLOTTER_MODEL_NUMBER,
    PLOTTER_USER_QUEUE,
    PLOTTER_FILE_SIZES_PENDING
  };

  static const char* PLOTTER_NAME;
  static const char* PLOTTER_PROPERTY_NAMES[];

  // = Printer property descriptions

  enum PRINTER_PROPERTIES
  {
    PRINTER_COLOR,
    PRINTER_DOUBLE_SIDED,
    PRINTER_COST_PER_PAGE,
    PRINTER_MODEL_NUMBER,
    PRINTER_PAGES_PER_SEC,
    PRINTER_USER_QUEUE,
    PRINTER_FILE_SIZES_PENDING
  };

  static const char* PRINTER_NAME;
  static const char* PRINTER_PROPERTY_NAMES[];

  // = File System Property Descriptions

  enum FILESYSTEM_PROPERTIES
  {
    DISK_SIZE,
    SPACE_REMAINING,
    PERMISSION_LEVEL
  };

  static const char* FILESYSTEM_NAME;
  static const char* FILESYSTEM_PROPERTY_NAMES[];

  // = PostScript Printer property descriptions.

  enum PS_PRINTER_PROPERTIES
  {
    VERSION
  };

  static const char* PS_PRINTER_NAME;
  static const char* PS_PRINTER_PROPERTY_NAMES[];

  // = Offer Info

#define NUM_OFFERS 15
  static const char* LOCATIONS[];
  static const char* USERS[];
  static const char* MODEL_NUMBERS[];

  // = Query info

  static const int NUM_QUERIES;
  static const char* QUERIES[][3];
};

class TT_Parse_Args
  // = TITLE
  // Utility to parse the command-line arguments to the trading service tests. 
{
  public:
  
  TT_Parse_Args (int& argc, char** argv);

  ~TT_Parse_Args ();
  
  int federated () const;
  // True if the test should test the federated features of the trading
  // service.

  int quiet () const;
  // True if the tests should supress all but the most essential output. 

  char* ior () const;
  // Not null if the test user supplied an explicit ior.

  private:

  int federated_, quiet_;
  char* ior_; 
};

#endif /* TAO_TRADER_TEST_UTILS_H */
