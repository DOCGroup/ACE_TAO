# $Id$
#
# Please see $TAO_ROOT/docs/Options.html for details on these options.
#

# This is a single threaded "server."
static Advanced_Resource_Factory "-ORBReactorType select_st -ORBInputCDRAllocator null  -ORBConnectionCacheLock null"

# Make sure we use RW client handler so if something goes wrong, the program will bloc,
# i.e., won't work.
static Client_Strategy_Factory "-ORBClientConnectionHandler RW"

# Not absolutely necessary for this test, but we add it for fun.
static Server_Strategy_Factory "-ORBConcurrency reactive"

static TAO_CSD_TP_Strategy_Factory "-CSDtp child:2:OFF"
