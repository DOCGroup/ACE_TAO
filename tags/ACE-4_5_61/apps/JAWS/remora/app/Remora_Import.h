// $Id

// ===================================================================
//
// = FILENAME
//    Remora_Import.h
//
// = AUTHOR
//    Seth Widoff
//
// ===================================================================

#ifndef REMORA_IMPORT_H
#define REMORA_IMPORT_H

#include <bstring.h>
#include "remora_c.hh"

#define NO_NAME  "Unnamed Statistic"
#define MAX_VALUE 100
#define CONTROL_AGENT_NAME "Remora Controls Agent"

class Remora_Import
{
public:

  Remora_Import(const char * label = NO_NAME, int value = 0,
                int min_value = 0, int max_value = MAX_VALUE);

  Remora_Import(const Remora_Import& import);

  ~Remora_Import();

  CORBA::Long get(void);

  operator CORBA::Long();

  friend int operator==( Remora_Import& me,  int new_value);
  friend int operator==( Remora_Import& me,  Remora_Import& export);

  friend int operator!=( Remora_Import& me,  int new_value);
  friend int operator!=( Remora_Import& me,  Remora_Import& export);

  friend int operator<=( Remora_Import& me,  int new_value);
  friend int operator<=( Remora_Import& me,  Remora_Import& export);

  friend int operator>=( Remora_Import& me,  int new_value);
  friend int operator>=( Remora_Import& me,  Remora_Import& export);

  friend int operator<( Remora_Import& me,  int new_value);
  friend int operator<( Remora_Import& me,  Remora_Import& export);

  friend int operator>( Remora_Import& me,  int new_value);
  friend int operator>( Remora_Import& me,  Remora_Import& export);

private:

  void init();

  CORBA::Long grab_value();

  remora::Control control_;

  remora::Remora_Controls_Agent_ptr remora_agent_;

};

#include "Remora_Import.i"
#endif
