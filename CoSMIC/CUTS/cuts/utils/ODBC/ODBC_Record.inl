// $Id$

//
// get_data
//
CUTS_INLINE
void ODBC_Record::get_data (size_t column,
                            char * buffer,
                            size_t bufsize)
                            ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_CHAR,
                    buffer,
                    bufsize,
                    0);
}


//
// get_data
//
CUTS_INLINE
void ODBC_Record::get_data (size_t column,
                            long & value)
                            ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_SLONG,
                    &value,
                    sizeof (long),
                    0);
}

//
// get_data
//
CUTS_INLINE
void ODBC_Record::get_data (size_t column,
                            unsigned long & value)
                            ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_ULONG,
                    &value,
                    sizeof (unsigned long),
                    0);
}

//
// get_data
//
CUTS_INLINE
void ODBC_Record::get_data (size_t column,
                            char & value)
                            ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_CHAR,
                    &value,
                    sizeof (char),
                    0);
}

//
// get_data
//
CUTS_INLINE
void ODBC_Record::get_data (size_t column,
                            unsigned char & value)
                            ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_CHAR,
                    &value,
                    sizeof (unsigned char),
                    0);
}

//
// get_data
//
CUTS_INLINE
void ODBC_Record::get_data (size_t column,
                            short & value)
                            ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_SHORT,
                    &value,
                    sizeof (short),
                    0);
}

//
// get_data
//
CUTS_INLINE
void ODBC_Record::get_data (size_t column,
                            unsigned short & value)
                            ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->get_data_i (static_cast <SQLUSMALLINT> (column),
                    SQL_C_USHORT,
                    &value,
                    sizeof (unsigned short),
                    0);
}
