# $Id$
use ARMSBB;

## insert switches
INSERT INTO Switch (name, equipmentID, vendor, password, enablePW, managedIPAddr, readCommunity)
VALUES
  ("daisy", "5000", "Cisco", "ngnlab", "mtv", "192.4.20.62", "public"),
  ("daffodil", "5000", "Cisco", "ngnlab", "mtv", "192.4.20.63", "public"),
  ("dummy", "5000", "Cisco", "ngnlab", "mtv", "dummy", "public"),
  ("dummy2", "5000", "Cisco", "ngnlab", "mtv", "dummy2", "public"),
  ("dandelion", "6500", "Cisco", "ngnlab", "mtv", "192.4.20.146", "public"),
  ("dahlia", "6500", "Cisco", "ngnlab", "mtv", "192.4.20.145", "public");

## Insert PTPs
INSERT INTO PTP (name, adminState, operationalState, rate, phyAddress, switchID, routerID)
VALUES
  ('3/1', 'up', 'up', 100000, 'phy', 1, NULL),
  ('3/4', 'up', 'up', 100000, 'phy', 1, NULL),
  ('3/5', 'up', 'up', 100000, 'phy', 1, NULL),
  ('3/12', 'up', 'up', 100000, 'phy', 1, NULL),
  ('3/1', 'up', 'up', 100000, 'phy', 2, NULL),
  ('3/4', 'up', 'up', 100000, 'phy', 2, NULL),
  ('3/5', 'up', 'up', 100000, 'phy', 2, NULL),
  ('3/12', 'up', 'up', 100000, 'phy', 2, NULL),
  ('3/1', 'up', 'up', 100000, 'phy', 3, NULL),
  ('3/4', 'up', 'up', 100000, 'phy', 3, NULL),
  ('3/5', 'up', 'up', 100000, 'phy', 3, NULL),
  ('3/12', 'up', 'up', 100000, 'phy', 3, NULL),
  ('3/5', 'up', 'up', 100000, 'phy', 4, NULL),
  ('3/12', 'up', 'up', 100000, 'phy', 4, NULL),
  ('Fa4/1', 'up', 'up', 100000, 'phy', 5, NULL),
  ('Fa4/2', 'up', 'up', 100000, 'phy', 5, NULL),
  ('4/1', 'up', 'up', 100000, 'phy', 6, NULL),
  ('4/2', 'up', 'up', 100000, 'phy', 6, NULL);



## Insert Links
INSERT INTO Link (aPtpID, zPtpID)
VALUES
  (1, 5),
  (6, 9),
  (10, 13),
  (15, 8);


## Insert appropriate state into the activeVLAN table
INSERT INTO activeVLAN (vlanID, ptpID, switchID)
VALUES
  (100, 1, 1),
  (100, 5, 2),
  (100, 6, 2),
  (100, 9, 3),
  (100, 10, 3),
  (100, 13, 4),
#  (100, 15, 5),
  (100, 8, 2);


## Insert L2Interface
INSERT INTO L2Interface (name, adminState, operationalState, ptpID, switchID)
VALUES
  ('3/1', 'up', 'up', 1, 1),
  ('3/4', 'up', 'up', 2, 1),
  ('3/5', 'up', 'up', 3, 1),
  ('3/12', 'up', 'up', 4, 1),
  ('3/1', 'up', 'up', 5, 2),
  ('3/4', 'up', 'up', 6, 2),
  ('3/5', 'up', 'up', 7, 2),
  ('3/12', 'up', 'up', 8, 2);

