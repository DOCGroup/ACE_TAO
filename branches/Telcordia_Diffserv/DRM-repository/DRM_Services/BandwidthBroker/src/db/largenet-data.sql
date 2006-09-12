use ARMSBB;

## Insert routers
INSERT INTO Router (name, equipmentID, vendor, osVersion, adminState,
  operationalState, password, enablePW, managedIPAddr, isProviderEdge,
  lastACL)
VALUES
  ('coresw1', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.1.1', 0, 0),
  ('coresw2', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.2.1', 0, 0),
  ('coresw3', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.3.1', 0, 0),

  ('access1', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   'access1', 0, 0),
  ('access2', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   'access2', 0, 0),
  ('access3', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   'access3', 0, 0),
  ('access4', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   'access4', 0, 0),
  ('access5', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   'access5', 0, 0),
  ('access6', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   'access6', 0, 0);

## Insert PTPs
INSERT INTO PTP (name, adminState, operationalState, rate, phyAddress,
  switchID, routerID)
VALUES
# Interfaces on coresw1
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 1),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 1),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 1),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 1),

# Interfaces on coresw2
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 2),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 2),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 2),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 2),

# Interfaces on coresw3
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 3),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 3),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 3),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 3),

# Interfaces on access1
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 4),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 4),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 4),

# Interfaces on access2
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 5),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 5),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 5),

# Interfaces on access3
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 6),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 6),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 6),

# Interfaces on access4
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 7),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 7),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 7),

# Interfaces on access5
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 8),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 8),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 8),

# Interfaces on access6
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 9),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 9),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 9);


## Insert L3Interface
INSERT INTO L3Interface (name, adminState, operationalState, ipAddress, ipMask,
  vlanID, ptpID, routerID)
VALUES
  ('ethA', 'up', 'up', '192.168.1.1', '255.255.255.0', NULL, 1, 1),
  ('ethB', 'up', 'up', '192.168.3.2', '255.255.255.0', NULL, 2, 1),
  ('ethC', 'up', 'up', '192.168.101.1', '255.255.255.0', NULL, 3, 1),
  ('ethD', 'up', 'up', '192.168.102.1', '255.255.255.0', NULL, 4, 1),

  ('ethA', 'up', 'up', '192.168.2.1', '255.255.255.0', NULL, 5, 2),
  ('ethB', 'up', 'up', '192.168.1.2', '255.255.255.0', NULL, 6, 2),
  ('ethC', 'up', 'up', '192.168.103.1', '255.255.255.0', NULL, 7, 2),
  ('ethD', 'up', 'up', '192.168.104.1', '255.255.255.0', NULL, 8, 2),

  ('ethA', 'up', 'up', '192.168.3.1', '255.255.255.0', NULL, 9, 3),
  ('ethB', 'up', 'up', '192.168.2.2', '255.255.255.0', NULL, 10, 3),
  ('ethC', 'up', 'up', '192.168.105.1', '255.255.255.0', NULL, 11, 3),
  ('ethD', 'up', 'up', '192.168.106.1', '255.255.255.0', NULL, 12, 3),

  ('ethA', 'up', 'up', '192.168.101.2', '255.255.255.0', NULL, 13, 4),
  ('ethB', 'up', 'up', '192.168.201.1', '255.255.255.0', NULL, 14, 4),
  ('ethC', 'up', 'up', '192.168.202.1', '255.255.255.0', NULL, 15, 4),

  ('ethA', 'up', 'up', '192.168.102.2', '255.255.255.0', NULL, 16, 5),
  ('ethB', 'up', 'up', '192.168.203.1', '255.255.255.0', NULL, 17, 5),
  ('ethC', 'up', 'up', '192.168.204.1', '255.255.255.0', NULL, 18, 5),

  ('ethA', 'up', 'up', '192.168.103.2', '255.255.255.0', NULL, 19, 6),
  ('ethB', 'up', 'up', '192.168.205.1', '255.255.255.0', NULL, 20, 6),
  ('ethC', 'up', 'up', '192.168.206.1', '255.255.255.0', NULL, 21, 6),

  ('ethA', 'up', 'up', '192.168.104.2', '255.255.255.0', NULL, 22, 7),
  ('ethB', 'up', 'up', '192.168.207.1', '255.255.255.0', NULL, 23, 7),
  ('ethC', 'up', 'up', '192.168.208.1', '255.255.255.0', NULL, 24, 7),

  ('ethA', 'up', 'up', '192.168.105.2', '255.255.255.0', NULL, 25, 8),
  ('ethB', 'up', 'up', '192.168.209.1', '255.255.255.0', NULL, 26, 8),
  ('ethC', 'up', 'up', '192.168.210.1', '255.255.255.0', NULL, 27, 8),

  ('ethA', 'up', 'up', '192.168.106.2', '255.255.255.0', NULL, 28, 9),
  ('ethB', 'up', 'up', '192.168.211.1', '255.255.255.0', NULL, 29, 9),
  ('ethC', 'up', 'up', '192.168.212.1', '255.255.255.0', NULL, 30, 9);



## Insert ServiceClass

INSERT INTO ServiceClass (name, dscp, dropPrecedence, utilLimit)
VALUES
   ('highPriority', 40, 0, 20),   
   ('highReliability', 32, 0, 30),   
   ('mmVideo', 24, 0, 20),
   ('mmVoice', 24, 0, 20),
   ('bestEffort', 0, 0, 10);   
 

INSERT INTO BWPerClass (ptpID) SELECT id from PTP;
UPDATE BWPerClass set svcClassID=1 where svcClassID=0;
INSERT INTO BWPerClass (ptpID) SELECT id from PTP;
UPDATE BWPerClass set svcClassID=2 where svcClassID=0;
INSERT INTO BWPerClass (ptpID) SELECT id from PTP;
UPDATE BWPerClass set svcClassID=3 where svcClassID=0;
INSERT INTO BWPerClass (ptpID) SELECT id from PTP;
UPDATE BWPerClass set svcClassID=4 where svcClassID=0;
UPDATE BWPerClass set inBound=0;
INSERT INTO BWPerClass (ptpID, svcClassID) SELECT ptpID,svcClassID from BWPerClass where inbound=0;
UPDATE BWPerClass set bwTotal=20000 where svcClassID=1;
UPDATE BWPerClass set bwTotal=30000 where svcClassID=2;
UPDATE BWPerClass set bwTotal=20000 where svcClassID=3;
UPDATE BWPerClass set bwTotal=20000 where svcClassID=4;

#Insert HostSubnet
INSERT INTO HostSubnet (hostSubnetAddr, hostSubnetMask, l3InterfaceID, poolName, l2InterfaceID, l3Gateway, hostName)
VALUES
    ('192.168.201.2', '255.255.255.255', 14, 'Pool-1A', NULL, '192.168.201.1', 'alpha'),
    ('192.168.202.2', '255.255.255.255', 15, 'Pool-1B', NULL, '192.168.202.1', 'bravo'),
    ('192.168.203.2', '255.255.255.255', 17, 'Pool-1B', NULL, '192.168.203.1', 'charlie'),
    ('192.168.204.2', '255.255.255.255', 18, 'Pool-1B', NULL, '192.168.204.1', 'delta'),
    ('192.168.205.2', '255.255.255.255', 20, 'Pool-1B', NULL, '192.168.205.1', 'echo'),
    ('192.168.206.2', '255.255.255.255', 21, 'Pool-1B', NULL, '192.168.206.1', 'foxtrot'),
    ('192.168.207.2', '255.255.255.255', 23, 'Pool-1B', NULL, '192.168.207.1', 'golf'),
    ('192.168.208.2', '255.255.255.255', 24, 'Pool-1B', NULL, '192.168.208.1', 'hotel'),
    ('192.168.209.2', '255.255.255.255', 26, 'Pool-1B', NULL, '192.168.209.1', 'india'),
    ('192.168.210.2', '255.255.255.255', 27, 'Pool-1B', NULL, '192.168.210.1', 'juliet'),
    ('192.168.211.2', '255.255.255.255', 29, 'Pool-1B', NULL, '192.168.211.1', 'kilo'),
    ('192.168.212.2', '255.255.255.255', 30, 'Pool-1B', NULL, '192.168.212.1', 'lima');



#Insert L3Path
INSERT INTO L3Path (ingressID, egressID)
VALUES
	(1,2), (1,3), (1,4), (1,5),(1,6), (1,7), (1,8), (1,9),
	(2,1), (2,3), (2,4), (2,5),(2,6), (2,7), (2,8), (2,9),
	(3,1), (3,2), (3,4), (3,5),(3,6), (3,7), (3,8), (3,9),
	(4,1), (4,2), (4,3), (4,5),(4,6), (4,7), (4,8), (4,9),
	(5,1), (5,2), (5,3), (5,4),(5,6), (5,7), (5,8), (5,9),
	(6,1), (6,2), (6,3), (6,4),(6,5), (6,7), (6,8), (6,9),
	(7,1), (7,2), (7,3), (7,4),(7,5), (7,6), (7,8), (7,9),
	(8,1), (8,2), (8,3), (8,4),(8,5), (8,6), (8,7), (8,9),
	(9,1), (9,2), (9,3), (9,4),(9,5), (9,6), (9,7), (9,8);



# update pool names

#UPDATE HostSubnet SET poolName = 'pool1' WHERE hostSubnetAddr = '192.168.82.0';
#UPDATE HostSubnet SET poolName = 'pool2' WHERE hostSubnetAddr = '192.168.83.0';
   
INSERT INTO Link (aPtpID, zPtpID)
VALUES
  (1, 6),
  (2, 9),
  (3, 13),
  (4, 16),
  (5, 10),
  (7, 19),
  (8, 22),
  (11, 25),
  (12, 28);
