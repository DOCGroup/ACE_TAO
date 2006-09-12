#include "mil_darpa_arms_mlrm_BandwidthBroker_common_VTime.h"

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

long tv2ms(struct timeval* tv) {
  return tv->tv_sec*1000 + tv->tv_usec/1000;
}

JNIEXPORT jlong JNICALL Java_mil_darpa_arms_mlrm_BandwidthBroker_common_VTime__1getProcessCPUTime
  (JNIEnv *env, jclass cls) {
  
  struct rusage ru;
  int rc;

  rc = getrusage(RUSAGE_SELF, &ru);
  if (rc < 0) return 0;

  return tv2ms(&ru.ru_utime) + tv2ms(&ru.ru_stime);
}
