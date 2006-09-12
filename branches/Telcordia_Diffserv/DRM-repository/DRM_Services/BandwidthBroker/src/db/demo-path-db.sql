#$Id$
use ARMSBB;

## Insert ServiceClass

INSERT INTO ServiceClass (name, dscp, dropPrecedence, utilLimit)
VALUES
   ('highPriority', 40, 0, 20),   
   ('highReliability', 32, 0, 30),   
   ('highThroughput', 24, 0, 40),
   ('bestEffort', 0, 0, 10);   
 

#Insert BWPerClass
INSERT INTO BWPerClass (bwAllocated, bwTotal, svcClassID, ptpID)
VALUES
   (0, 2000, 1, 1),
   (0, 3000, 2, 1),	
   (0, 4000, 3, 1),	
   (0, 1000, 4, 1),
   (0, 2000, 1, 3),
   (0, 3000, 2, 3),	
   (0, 4000, 3, 3),	
   (0, 1000, 4, 3);

#Insert L3Path
INSERT INTO L3Path (ingressID, egressID)
VALUES
	(1,2),
	(2,1);

#Insert L3Hops
INSERT INTO L3Hops (l3InterfaceID, l3PathID, hopNumber)
VALUES
	(3, 1, 1),
	(1, 2, 1);


# update pool names

UPDATE HostSubnet SET poolName = 'pool1' WHERE hostSubnetAddr = '172.16.14.0';
UPDATE HostSubnet SET poolName = 'pool2' WHERE hostSubnetAddr = '172.16.13.0';
   
   


