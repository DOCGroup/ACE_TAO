// $Id$

#define CLIENT_COMPRESSION_LEVEL 7
#define SERVER_COMPRESSION_LEVEL 5
#define LEAST_COMPRESSION_LEVEL ((CLIENT_COMPRESSION_LEVEL < SERVER_COMPRESSION_LEVEL) ?\
                                  CLIENT_COMPRESSION_LEVEL : SERVER_COMPRESSION_LEVEL)

static const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
static const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");
static int test = 1;
