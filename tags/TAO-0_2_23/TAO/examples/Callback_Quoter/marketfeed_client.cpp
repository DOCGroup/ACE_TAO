// $Id$

#include "MarketFeed_i.h"

// This function runs the Callback Quoter MarketFeed daemon.

int
main (int argc, char **argv)
{
  MarketFeed_i client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tMarketFeed client\n\n"));

  if (client.init (argc, argv) == -1)
    return -1;
  else
    return client.run ();
}
