// $Id$

#ifndef TAO_TRADER_TEST_UTILS_H
#define TAO_TRADER_TEST_UTILS_H

#include "TTestS.h"
#include "orbsvcs/SequencesC.h"
#include "orbsvcs/CosTradingC.h"

class TT_Info
{
public:

  static void dump_properties (const CosTrading::PropertySeq& prop_seq);
  // Dump the contents of this property sequence.

  static void serialize_offer (const CosTrading::Offer& offer);
  
  enum INTERFACES
  {
    REMOTE_IO,
    PLOTTER,
    PRINTER,
    FILESYSTEM,
    PS_PRINTER
  };
  
  class Remote_Output : public POA_TAO_Trader_Test::Remote_Output
  {
  public:
    Remote_Output (void) {}
  };
  
  class Printer : public POA_TAO_Trader_Test::Printer
  {
  public:
    Printer (void) {}
  };
  
  class Plotter : public POA_TAO_Trader_Test::Plotter
  {
  public:
    Plotter (void) {}
  };
  
  class File_System : public POA_TAO_Trader_Test::File_System
  {
  public:  
    File_System (void) {}
  };
  
  class PostScript_Printer : public POA_TAO_Trader_Test::PostScript_Printer
  {
  public:
    PostScript_Printer (void) {}
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
    TRADER_NAME
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

#endif /* TAO_TRADER_TEST_UTILS_H */
