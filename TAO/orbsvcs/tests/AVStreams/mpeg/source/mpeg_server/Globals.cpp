
#include "Globals.h"

int Mpeg_Global::parentpid = -1;
int Mpeg_Global::listenSocketIn = -1;
int Mpeg_Global::listenSocketUn = -1;
struct linger Mpeg_Global::linger = {1,1};
int Mpeg_Global::live_audio = 0;
int Mpeg_Global::live_video = 0;  /* 0 - no,  1 - to open,  2 - opened */
int Mpeg_Global::drift_ppm = 0;  /* clock drift in ppm */
int Mpeg_Global::session_limit = SESSION_NUM;
int Mpeg_Global::session_num = 0;
int Mpeg_Global::rttag = 0;

// initialize the nasty int's, doubles and their friends
Video_Global::Video_Global ()
{
  live_source = 0;
  video_format = 0;
  
  pkts_sent = 0;
  start_time = 0;
  
  conn_tag = 0;

  normalExit = 1;

  serviceSocket = 0;
  videoSocket = -1;

  ACE_OS::memset (videoFile, 
                  0,
                  PATH_SIZE);

  fp = 0;

  needHeader = 0;

  lastRef [0] = lastRef [1] = 0;

  lastRefPtr = 0;
  currentUPF = 0;
  addedUPF = 0;
  addedSignals = 0;
  VStimeAdvance = 0;
  fps = 0;  /* current frames-per-second: playback speed */
  frameRateLimit = 0;

  packet = 0;
  packetBufSize = 0;
  msgsn = 0;
  packetsn = 0;
  msgsize = 0;

  precmd = 0;
  cmd = 0;
  cmdsn = 0;
  nextFrame = 0;
  nextGroup = 0;
  firstPatternSize = 0;
  firstSendPattern = 0;
  sendPatternGops = 0;
  ACE_OS::memset (sendPattern,
                  0,
                  PATTERN_SIZE);

#ifdef STAT
  framesSent = 0;
#endif

  fileSize = 0;
  maxS = 0;
  maxG = 0;  
  maxI = 0;
  maxP = 0;
  maxB = 0;
  minS = 0x7fffffff;
  minG = 0x7fffffff;
  minI = 0x7fffffff;
  minP = 0x7fffffff;
  minB = 0x7fffffff;

  numS = 0;
  numG = 0;
  numF = 0;
  numI = 0;
  numP = 0;
  numB = 0;

  averageFrameSize = 0;
  horizontalSize = 0;
  verticalSize = 0;
  pelAspectRatio = 0;
  pictureRate = 0;
  vbvBufferSize = 0;
  firstGopFrames = 0;
  patternSize = 0;

  ACE_OS::memset (pattern,
                  0,
                  PATTERN_SIZE);

  // struct pointers
  systemHeader = 0;

  gopTable = 0;

  frameTable = 0;

}
