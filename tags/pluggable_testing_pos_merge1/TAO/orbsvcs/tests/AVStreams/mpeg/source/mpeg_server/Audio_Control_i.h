/* _*_ C++ _*_ */
//$Id$

#ifndef _AV_AUDIO_CONTROL_H
#define _AV_AUDIO_CONTROL_H

#include "mpeg_shared/Audio_ControlS.h"
#include "Globals.h"
#include "Audio_Server.h"

class Audio_Control_State;

class Audio_Control_i
  :public virtual POA_Audio_Control
{
public:
  Audio_Control_i (void);
  // Default Constructor

  ~Audio_Control_i (void);
  // Default destructor

  int create_handlers (void);
  // creates the data and sig handlers.

  virtual CORBA::Boolean init_audio (
      const Audio_Control::INITaudioPara & para,
      Audio_Control::INITaudioReply_out reply_para,
      CORBA::Environment &_tao_environment
     ) ;

  virtual CORBA::Boolean play (
      const Audio_Control::PLAYPara & para,
      CORBA::Long_out ats,
      CORBA::Environment &_tao_environment
     ) ;

  virtual CORBA::Boolean speed (
      const Audio_Control::SPEEDPara & para,
      CORBA::Environment &_tao_environment
     ) ;

  virtual CORBA::Boolean stop (
      CORBA::Long cmdsn,
      CORBA::Environment &_tao_environment
     ) ;

  virtual CORBA::Boolean set_peer (
                                   char *&peer,
      CORBA::Environment &_tao_environment
     ) ;

  virtual void close (
      CORBA::Environment &_tao_environment
      ) ;

  void change_state (Audio_Control_State *state);
  // Changes the state of the Audio Control object.

  Audio_Control_State *get_state (void);
  // gets the state.
private:
  int register_handlers (void);

  ACE_Reactor *reactor_;

  Audio_Control_State *state_;

  Audio_Data_Handler *data_handler_;

  Audio_Sig_Handler *sig_handler_;

  ACE_SOCK_CODgram dgram_;

  Audio_Global *audio_global_;
};

typedef ACE_Singleton <Audio_Control_i,ACE_Null_Mutex> AUDIO_CONTROL_I;

#endif /*_AV_AUDIO_CONTROL_H */
