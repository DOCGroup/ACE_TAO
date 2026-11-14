#ifndef TAO_TRADER_TEST_UTILS_H
#define TAO_TRADER_TEST_UTILS_H

#include "TTestS.h"
#include "ace/Arg_Shifter.h"
#include "ace/Read_Buffer.h"
#include "orbsvcs/CosTradingC.h"
#include "orbsvcs/CosTradingReposC.h"
#include "orbsvcs/CosTradingDynamicC.h"
#include "ttest_export.h"

class TAO_TTest_Export TT_Info
{
public:
  static void dump_properties (const CosTrading::PropertySeq& prop_seq,
                               CORBA::Boolean print_dynamic = 1);
  /// Dump the contents of this property sequence.

  enum INTERFACES
  {
    REMOTE_IO,
    PLOTTER,
    PRINTER,
    FILESYSTEM,
    PS_PRINTER
  };

  ///@name Test servants
  ///@{

  class Remote_Output :
    public POA_TAO_Trader_Test::Remote_Output
  {
  public:
    Remote_Output () {}

    /// Confirm that the importer received a valid reference to the exported object.
    void confirm () override
      {}
  };

  class TAO_TTest_Export Printer :
    public POA_TAO_Trader_Test::Printer
  {
  public:
    Printer () {}

    /// Confirm that the importer received a valid reference to the exported object.
    void confirm () override
      {}
  };

  class TAO_TTest_Export Plotter :
    public POA_TAO_Trader_Test::Plotter
  {
  public:
    Plotter () {}

    /// Confirm that the importer received a valid reference to the exported object.
    void confirm () override
      {}
  };

  class TAO_TTest_Export File_System :
    public POA_TAO_Trader_Test::File_System
  {
  public:
    File_System () {}

    /// Confirm that the importer received a valid reference to the exported object.
    void confirm () override
      {}
  };

  class TAO_TTest_Export PostScript_Printer :
    public POA_TAO_Trader_Test::PostScript_Printer
  {
  public:
    PostScript_Printer () {}

    /// Confirm that the importer received a valid reference to the exported object.
    void confirm () override
      {}
  };
  ///@}

#define NUM_TYPES 5
  static const char* INTERFACE_NAMES[];

  ///@name Remote I/O property descriptions
  ///@{

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
  ///@}

  ///@name Plotter property descriptions
  ///@{

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
  ///@}

  ///@name Printer property descriptions
  ///@{

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
  ///@}

  ///@name File system property descriptions
  ///@{

  enum FILESYSTEM_PROPERTIES
  {
    DISK_SIZE,
    SPACE_REMAINING,
    PERMISSION_LEVEL
  };

  static const char* FILESYSTEM_NAME;
  static const char* FILESYSTEM_PROPERTY_NAMES[];
  ///@}

  ///@name PostScript printer property descriptions
  ///@{

  enum PS_PRINTER_PROPERTIES
  {
    VERSION
  };

  static const char* PS_PRINTER_NAME;
  static const char* PS_PRINTER_PROPERTY_NAMES[];
  ///@}

  ///@name Offer information
  ///@{

#define NUM_OFFERS 15
  static const char* LOCATIONS[];
  static const char* USERS[];
  static const char* MODEL_NUMBERS[];
  ///@}

  ///@name Query information
  ///@{

  static const int NUM_QUERIES;
  static const char* QUERIES[][4];
  ///@}
};

/// Parses command-line arguments for the Trading Service tests.
class TAO_TTest_Export TT_Parse_Args
{
  public:
  TT_Parse_Args (int& argc, ACE_TCHAR** argv);

  ~TT_Parse_Args ();

  /// Whether the test should exercise federated trading features.
  bool federated () const;

  /// Whether to limit output to essential test information.
  bool quiet () const;

  /// Explicit IOR supplied by the user, or null if none was supplied.
  char* ior () const;

  private:
  bool federated_, quiet_;
  char* ior_;
};

#endif /* TAO_TRADER_TEST_UTILS_H */
