#!/bin/bash
$CIAO_ROOT/DAnCE/TM_Daemon/TM_Daemon -ORBInitRef NameService=corbaloc:iiop:roon.dre.vanderbilt.edu:40000/NameService -k SPACE.cdp -m NodeManagerMap_1.dat -N CIAO_TM_Daemon -d Domain_1.cdd
