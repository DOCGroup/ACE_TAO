#include "util.h"



URL::URL(char *input_buf) {

  char *buffer = new char[BUFSIZ];

  ACE_OS::strcpy(buffer,input_buf);
  if(buffer == NULL)
    return;

  char *temp;
  char *lasts;

  if((temp = ACE_OS::strtok_r(buffer,": ",&lasts)))  {
	protocol_ = (char *) ACE_OS::malloc(strlen(temp));
	ACE_OS::strcpy(protocol_, temp);
  }

  if((temp = ACE_OS::strtok_r(NULL,"/",&lasts)))  {
	hostname_ = (char *) ACE_OS::malloc(strlen(temp));  
	ACE_OS::strcpy(hostname_, temp);
  }
  if((temp = ACE_OS::strtok_r(NULL,"\0",&lasts))) { 
    filename_ = (char *) malloc(strlen(temp));
	ACE_OS::strcpy(filename_, temp);
  }
  else {
    filename_ = (char *) malloc(strlen(INDEX_NAME));
	ACE_OS::strcpy(filename_,INDEX_NAME);
  }
}

char *URL::get_protocol(void) {
  return protocol_;
}

char *URL::get_hostname(void) {
  return hostname_;
}

char *URL::get_filename(void) {
  return filename_;
}
  

 


void cleanup(void) {
  unlink(TEMPORARY_FILE_NAME);
  unlink(INCOMING_FILE_NAME);
}

void sigint(int) {
  cleanup();
}
 



