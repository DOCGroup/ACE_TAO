// $Id$

#ifndef ACE_ADAPTER_HIGH_RES_TIMER_H
#define ACE_ADAPTER_HIGH_RES_TIMER_H
#include "ace/pre.h"

#include "../../High_Res_Timer.h"

class ACE_High_Res_Timer_W : public ACE_High_Res_Timer
{
public:
  static int get_env_global_scale_factor (const wchar_t *env 
                                          = L"ACE_SCALE_FACTOR")
  { return ACE_High_Res_Timer::get_env_global_scale_factor (ACE_TEXT_WCHAR_TO_CHAR (env)); }

  ACE_High_Res_Timer_W (void)
  {}

  ~ACE_High_Res_Timer_W (void)
  {}

#if !defined (ACE_HAS_WINCE)
  void print_total (const wchar_t *message,
                    const int iterations = 1,
                    ACE_HANDLE handle = ACE_STDOUT) const
  { ACE_High_Res_Timer::print_total (ACE_TEXT_WCHAR_TO_CHAR (message), iterations, handle); }

  void print_ave (const wchar_t *message,
                  const int iterations = 1,
                  ACE_HANDLE handle = ACE_STDOUT) const
  { ACE_High_Res_Timer::print_ave (ACE_TEXT_WCHAR_TO_CHAR (message), iterations, handle); }
#endif /* !ACE_HAS_WINCE */

};

#include "ace/post.h"
#endif /* ACE_ADAPTER_HIGH_RES_TIMER_H */
