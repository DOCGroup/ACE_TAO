/* _*_ C++ _*_ */
// $Id$

#ifndef _MPEG_AUDIO_CONTROL_STATE_H
#define _MPEG_AUDIO_CONTROL_STATE_H

#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "mpeg_shared/Audio_ControlS.h"
#include "Globals.h"

class Audio_Control_i;

class Audio_Control_State
{
public:
  Audio_Control_State (void);
  // constructor

  enum Audio_States
  {
    AUDIO_WAITING =0,
      AUDIO_PLAY =1
  };

  Audio_States get_state (void);
  // returns the current state.

  void set_state (Audio_States state);
  // sets the state.

  virtual CORBA::Boolean play (const Audio_Control::PLAYPara & para,
                               CORBA::Long_out ats) ;

  virtual CORBA::Boolean speed (const Audio_Control::SPEEDPara & para) ;

  virtual CORBA::Boolean stop (CORBA::Long cmdsn) ;

  virtual void close (void) ;
protected:
  Audio_States state_;
  // The state

  Audio_Global *audio_global_;
  // Pointer to the global.
  Audio_Control_i *audio_control_i_;
};


class Audio_Control_Waiting_State : public virtual Audio_Control_State
{
public:
  Audio_Control_Waiting_State (void);
  // Default constructor.

  virtual CORBA::Boolean play (const Audio_Control::PLAYPara &para,
                               CORBA::Long_out ats);

  virtual void close (void);
};

class Audio_Control_Play_State: public virtual Audio_Control_State
{
public:
  Audio_Control_Play_State (void);
  // Default constructor

  virtual CORBA::Boolean speed (const Audio_Control::SPEEDPara & para);

  virtual CORBA::Boolean stop (CORBA::Long cmdsn);

  virtual void close (void);
};

typedef ACE_Singleton <Audio_Control_Waiting_State, ACE_SYNCH_MUTEX>
        AUDIO_CONTROL_WAITING_STATE;
typedef ACE_Singleton <Audio_Control_Play_State, ACE_SYNCH_MUTEX>
        AUDIO_CONTROL_PLAY_STATE;

#endif /*_MPEG_AUDIO_CONTROL_STATE_H */
