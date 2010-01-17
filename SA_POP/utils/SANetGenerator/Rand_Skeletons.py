#Daniel Mack
#Random Skeleton Generators

import networkx as nx
import random as rand


#Modified Erdos-Renyi Graph Generator 
def erdosGen(SANet, tasks, conds, inDegree, outDegree):
	inD = {}
	outD = {}
	pairs = {}
	haltGen = 0;
	haltOut = 0;
	haltIn  = 0;

	#continue until the graph has met the in/out degreee limits
	while  haltGen == 0:
		ctask = ""
		ccond = ""
		found = 0
		#while found == 0:
			#Choose Task
		ctask = tasks[rand.randrange(len(tasks))] 
			#Choose Cond
		ccond = conds[rand.randrange(len(conds))]
		key = ctask + "," + ccond
			#if already been used as either an effect or precondition
			#		if key in pairs:
		#			found = 0
		#		else:
				#valid pair for random pairing

			

		#choose direction
		EfforPre = rand.randrange(2)

		#if EfforPre == 0:
		edges = SANet.edges(ccond)
		for each in edges:
			if each[1] == ctask:
				found = 1
				break
		#else:
		edges = SANet.edges(ctask)
		for each in edges:
			if each[1] == ccond:
				found = 1
				break
			
		if found == 0:
			#If 0 then it's a precondition link
			if EfforPre == 0 and haltIn == 0:
				SANet.add_edge(ccond, ctask)
				if ctask in inD:
					inD[ctask] += 1
				else:
					inD[ctask] = 1	
			elif haltOut == 0:
				#otherwise it's an effect
				SANet.add_edge(ctask, ccond)
				if ctask in outD:
					outD[ctask] += 1
				else:
					outD[ctask] = 1

			#Check for Stopping Conditions 		
			totalIn = 0.0
			totalOut = 0.0
			for each in tasks:
				if each in inD:
					totalIn += inD[each]
				if each in outD:
					totalOut += outD[each]
			avgIn = totalIn/len(tasks)
			avgOut = totalOut/len(tasks)


			if avgIn >= inDegree:
				haltIn = 1 
			if avgOut >= outDegree:
				haltOut = 1
			if haltIn == 1 and haltOut == 1:	
				print avgIn
				print avgOut
				haltGen = 1
				
	return SANet