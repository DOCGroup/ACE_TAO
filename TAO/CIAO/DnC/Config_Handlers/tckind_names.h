//==================================================================
/**
 *  @file  tckind_names.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

/*
 *  Used to get the corresponding text for ::CORBA::TCKind enum
 */

class TCKind_Names {
protected:
  TCKind_Names();
  TCKind_Names(const TCKind_Names&);
  TCKind_Names& operator=(const TCKind_Names&);

public:
  static const char* get_name(::CORBA::TCKind kind)
  {
    return tc_kind_names[size_t(kind)];
  }
  
private:
  static const char* tc_kind_names[36];
};
