
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
