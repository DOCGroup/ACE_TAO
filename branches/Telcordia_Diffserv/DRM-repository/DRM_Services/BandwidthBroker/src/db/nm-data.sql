#$Id$
use ARMSBB;

#update HostSubnetTable with the measurement hosts
use ARMSBB;
INSERT INTO HostSubnet (hostSubnetAddr, hostSubnetMask, l3InterfaceID, poolName, l2InterfaceID, l3Gateway, hostName)
VALUES
    ('172.26.4.0', '255.255.255.0', 1, 'Pool-1C', 1, '172.26.4.1', 'rondo-mon08'),
    ('172.27.4.0', '255.255.255.0', 2, 'Pool-1D', 2, '172.27.4.1', 'rondo-mon09');

#update FlowRecord to test measurement using flow token
INSERT INTO FlowRecord (status, purgeStatus, srcIP, srcMask, srcPort, srcPortHigh, destIP, destMask, destPort, destPortHigh, protocol, dscp, bandwidthMin,extrabudget,bwAllocated,acl,svcClassID, inInterfaceID, outInterfaceID,priority,l2Endpoints,reference)
VALUES
('commited',0,'172.26.4.6', '255.255.255.0',20,20,'172.27.4.6','255.255.255.0',-1, -1, 'tcp',1, 2000, 10, 2000, 0, 2, 1, 2, 1,0,NULL);
