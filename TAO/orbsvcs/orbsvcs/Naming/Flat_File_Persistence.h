// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//
//
// = AUTHOR
//
//
// ============================================================================

#ifndef TAO_FLAT_FILE_PERSISTENCE_H
#define TAO_FLAT_FILE_PERSISTENCE_H

#include "Storable.h"
#include "tao/corba.h"

//------------------------------------------------------------------------
// The Flat File concrete classes
//------------------------------------------------------------------------
class TAO_NS_FlatFileWriter : public TAO_Writer_Base
{
public:

  TAO_NS_FlatFileWriter();
  virtual ~TAO_NS_FlatFileWriter();

  virtual int open(const char * filename);
  virtual int close();

  virtual TAO_Writer_Base& operator << (const TAO_NS_Persistence_Header &header);
  virtual TAO_Writer_Base& operator << (const TAO_NS_Persistence_Record &record);

private:
  FILE* fout_;
  ACE_CString filename_;
};

class TAO_NS_FlatFileReader : public TAO_Reader_Base
{
public:

  TAO_NS_FlatFileReader ();
  virtual ~TAO_NS_FlatFileReader();

  virtual int open(const char * filename);
  virtual int close();

  virtual TAO_Reader_Base& operator >> (TAO_NS_Persistence_Header &header);
  virtual TAO_Reader_Base& operator >> (TAO_NS_Persistence_Record &record);

private:
  FILE* fin_;
};


class TAO_NS_FlatFileFactory : public TAO_Naming_Service_Persistence_Factory
{
public:
  // Factory Methods

  virtual TAO_Reader_Base *create_reader();
  virtual TAO_Writer_Base *create_writer();
};


#endif
