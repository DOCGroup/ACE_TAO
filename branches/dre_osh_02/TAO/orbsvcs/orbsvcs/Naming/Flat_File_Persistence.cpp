// $Id$

//-----------------------------------------------------------------------------
// Flat File class implementations
//-----------------------------------------------------------------------------
#include "Flat_File_Persistence.h"

TAO_NS_FlatFileWriter::TAO_NS_FlatFileWriter ()
  : fout_ (0)
{
}

int
TAO_NS_FlatFileWriter::open(const char * filename)
{

  this->fout_ = ACE_OS::fopen(ACE_TEXT_CHAR_TO_TCHAR(filename), ACE_LIB_TEXT("w"));
  if (this->fout_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing %s",
                       filename),
                      1);

  delete_bindings_ = 0;
  filename_        = filename;

  return 0;
}

TAO_NS_FlatFileWriter::~TAO_NS_FlatFileWriter ()
{
}

int
TAO_NS_FlatFileWriter::close()
{
  ACE_OS::fclose(this->fout_);

  if (delete_bindings_)
#if defined (ACE_HAS_WINCE)
    ::DeleteFile(ACE_TEXT_CHAR_TO_TCHAR(filename_.c_str()));
#else
    remove (filename_.c_str());
#endif  // ACE_HAS_WINCE
  return 0;
}

TAO_Writer_Base &
TAO_NS_FlatFileWriter::operator <<(
                                const TAO_NS_Persistence_Header &header)
{
  unsigned int size = header.size();
  ACE_CString context_name = header.context_name();

  if (this->fout_ != 0)
    {
      ACE_OS::fprintf(this->fout_, "%u\n%s\n%d\n",
                      ACE_static_cast(unsigned int, context_name.length()),
                      context_name.c_str(),
                      size);
      ACE_OS::fflush(this->fout_);
    }

  return *this;
}

TAO_Writer_Base &
TAO_NS_FlatFileWriter::operator <<(
                                const TAO_NS_Persistence_Record &record)
{
  if (this->fout_ != 0)
    {
      TAO_NS_Persistence_Record::Record_Type type = record.type();

      ACE_OS::fprintf(this->fout_, "%d\n", type);

      ACE_CString id = record.id();

      ACE_OS::fprintf(this->fout_, "%u\n%s\n",
                      ACE_static_cast (unsigned int, id.length()),
                      id.c_str ());

      ACE_CString kind = record.kind();
      ACE_OS::fprintf(this->fout_, "%u\n%s\n",
                      ACE_static_cast(unsigned int, kind.length()),
                      kind.c_str());

      ACE_CString ior = record.ior();

      ACE_OS::fprintf(this->fout_, "%u\n%s\n",
                      ACE_static_cast(unsigned int, ior.length()),
                      ior.c_str());

      ACE_CString context_binding = record.context_binding();

      ACE_OS::fprintf(this->fout_, "%u\n%s\n",
                      ACE_static_cast(unsigned int, context_binding.length()),
                      context_binding.c_str());

      ACE_OS::fflush(this->fout_);
    }

  return *this;
}



//******************** Flat File Reader ****************************


TAO_NS_FlatFileReader::TAO_NS_FlatFileReader ()
 : fin_ (0)
{
}

int
TAO_NS_FlatFileReader::open(const char * filename)
{
  fin_ = ACE_OS::fopen(ACE_TEXT_CHAR_TO_TCHAR(filename), ACE_LIB_TEXT("r"));

  ACE_DECLARE_NEW_CORBA_ENV;
  if (fin_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ""),
                      1);

  return 0;
}

TAO_NS_FlatFileReader::~TAO_NS_FlatFileReader ()
{

}

int
TAO_NS_FlatFileReader::close()
{
  ACE_OS::fclose(fin_);
  return 0;
}


TAO_Reader_Base &
TAO_NS_FlatFileReader::operator >>(
                      TAO_NS_Persistence_Header &header)
{
  unsigned int size;
  int bufSize = 0;

  fscanf(fin_, "%d\n", &bufSize);
  ACE_TCHAR* context_name = new ACE_TCHAR[bufSize+1];
  //char* context_name = 0;
  //ACE_NEW_RETURN(context_name, char[bufSize+1], 0);
  ACE_OS::fgets(context_name, bufSize+1, fin_);
  fscanf(fin_, "%u\n", &size);

  ACE_CString new_cn = ACE_CString(ACE_TEXT_ALWAYS_CHAR(context_name));
  header.context_name(new_cn);
  delete context_name;

  header.size(size);

  return *this;

}

TAO_Reader_Base &
TAO_NS_FlatFileReader::operator >>(
                       TAO_NS_Persistence_Record &record)
{
  int type_integer = 0;
  fscanf(fin_, "%d\n", &type_integer);
  TAO_NS_Persistence_Record::Record_Type type =
    ACE_static_cast (TAO_NS_Persistence_Record::Record_Type, type_integer);

  record.type(type);

  int bufSize = 0;

  //id
  fscanf(fin_, "%d\n", &bufSize);
  ACE_TCHAR *id = new ACE_TCHAR[bufSize+1];
  //char *id;
  //ACE_NEW_RETURN (id, char[bufSize+1], 1);

  ACE_OS::fgets(id, bufSize+1, fin_);
  ACE_CString newId(ACE_TEXT_ALWAYS_CHAR(id));
  record.id(newId);

  delete id;

  //kind
  fscanf(fin_, "%d\n", &bufSize);
  ACE_TCHAR *kind = new ACE_TCHAR[bufSize+1];
  //char *kind;
  //ACE_NEW (kind, char[bufSize+1]);

  ACE_OS::fgets(kind, bufSize+1, fin_);
  kind[bufSize] = '\0';

  ACE_CString newKind(ACE_TEXT_ALWAYS_CHAR(kind));
  record.kind(newKind);

  delete kind;

   //ior
  fscanf(fin_, "%d\n", &bufSize);
  ACE_TCHAR *ior = new ACE_TCHAR[bufSize+1];
  //char *ior;
  //ACE_NEW(ior, char[bufSize+1]);

  ACE_OS::fgets(ior, bufSize+1, fin_);
  ACE_CString newIor(ACE_TEXT_ALWAYS_CHAR(ior));
  record.ior(newIor);

  delete ior;

  //context_binding
  fscanf(fin_, "%d\n", &bufSize);
  ACE_TCHAR *context_binding = new ACE_TCHAR[bufSize+1];
  //char *context_binding;
  //ACE_NEW (context_binding, char[bufSize+1]);

  ACE_OS::fgets(context_binding, bufSize+1, fin_);

  ACE_CString new_cb(ACE_TEXT_ALWAYS_CHAR(context_binding));

  record.context_binding(new_cb);

  delete context_binding;

  return *this;

}

TAO_Reader_Base *TAO_NS_FlatFileFactory::create_reader()
{
  TAO_Reader_Base *reader = 0;

  ACE_NEW_RETURN (reader,
                  TAO_NS_FlatFileReader(),
                  0);

  return reader;
}

TAO_Writer_Base *TAO_NS_FlatFileFactory::create_writer()
{
  TAO_Writer_Base *writer = 0;

  ACE_NEW_RETURN (writer,
                  TAO_NS_FlatFileWriter(),
                  0);
  return writer;
}
