// $Id

// ===================================================================
// 
// = FILENAME
//    Remora_Export.h
//
// = AUTHOR
//    Seth Widoff
// 
// ===================================================================


#ifndef REMORA_EXPORT_H
#define REMORA_EXPORT_H

#include <bstring.h>
#include "remora_c.hh"

#define MAX_VALUE 100
#define NO_NAME "Unnamed Statistic"
#define STATS_AGENT_NAME "Remora Statistics Agent"

class Remora_Export
{
public:
  
  Remora_Export(const char* label = NO_NAME, int value = 0,
		int max_value = MAX_VALUE); 

  Remora_Export(const Remora_Export& export);

  ~Remora_Export();
  
  void operator=(const int new_value);
  void operator=(const Remora_Export& export);

  void operator+=(const int new_value);  
  void operator+=(const Remora_Export& export);

  void operator*=(const int new_value);  
  void operator*=(const Remora_Export& export);

  void operator/=(const int new_value);  
  void operator/=(const Remora_Export& export);

  void operator-=(const int new_value);
  void operator-=(const Remora_Export& export);

  operator CORBA::Long() const;
  operator CORBA::Long&();
  
  friend int operator==(const Remora_Export& me, const int new_value);
  friend int operator==(const Remora_Export& me, const Remora_Export& export);

  friend int operator!=(const Remora_Export& me, const int new_value);
  friend int operator!=(const Remora_Export& me, const Remora_Export& export);
  
  friend int operator<=(const Remora_Export& me, const int new_value);
  friend int operator<=(const Remora_Export& me, const Remora_Export& export);
  
  friend int operator>=(const Remora_Export& me, const int new_value);
  friend int operator>=(const Remora_Export& me, const Remora_Export& export);
  
  friend int operator<(const Remora_Export& me, const int new_value);
  friend int operator<(const Remora_Export& me, const Remora_Export& export);

  friend int operator>(const Remora_Export& me, const int new_value);
  friend int operator>(const Remora_Export& me, const Remora_Export& export);

private:

  void init();
  
  void update_value(int new_value);

  remora::Statistic stat_;
    
  remora::Remora_Statistics_Agent_ptr remora_agent_;
  
};

#include "Remora_Export.i"

#endif
