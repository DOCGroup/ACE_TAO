#!/usr/bin/env python
# encoding: utf-8
"""
Rand_Params.py
Created by Daniel Mack on 2009-10-27.
Generates Random Parameters for a Skeleton
"""

import sys
import os
import networkx as nx
import random as rand

#Uniform 
def flatGen(SANet, tasks, conds):
	effLinks = {}
	preLinks = {}
	priors = {}
	
	#use the tasks as the starting point for assigning probabilities
	for each in tasks:
		effects = SANet.successors(each)
		effLinks[each] = [] 
		effAr = effLinks[each]
		
		for eff in effects:
			assigned = 0
			#uses uniform for the time being
			edges = SANet.edges(eff)
			#print eff
			#print edges
			#for edge in edges:
			#	#print eff
			#	if edge[1] == each:
			#		#print edge
			#		assigned = 1
			#		weight = rand.uniform(-.8,-1)
			#		#print weight
			#		break
				
			if assigned == 0:
				#dire = rand.uniform(0,2) - 1
			
				#if dire >= 0:
				weight = rand.uniform(.8,1)
				#else:
				#	weight = rand.uniform(-.8,-1)	
				#a tuple for efficient look up
			lnk = (eff, weight)
			effAr.append(lnk)
			
		effLinks[each] = effAr
		
		preconds = SANet.predecessors(each)
		
		#this is uniform at .5 probability across all tasks
		prior = 1.0
		preprob = 0.0
		#must have preconditions
		#TEMP#TEMP#TEMP#TEMPTEMP#TEMP#TEMP#TEMP
		#currently all preconditions must be positive
		#TEMP#TEMP#TEMP#TEMP#TEMP#TEMP#TEMP#TEMP
		if len(preconds) > 0:
			#Value is .5 to the power of the number of preconditions
			prior = pow(.5, len(preconds))
			#Each precondition probability is .5 to the number of preconditions - 1
			#Should work for positive and negative, but for now, all preconditions are positive.
			preprob = pow(.5, (len(preconds) - 1))
			
		priors[each] = prior
		
		#Book keeping for the graph. 
		#Assemble information on precondition links
		for pre in preconds:
			if pre in preLinks:
				temp =  preLinks[pre]
				#A pair of task and precondition probability from the given conditon
				temp.append((each,preprob))
				preLinks[pre] =temp
			else:
				lnks = []
				lnks.append((each, preprob))
				preLinks[pre] = lnks	 
	
	#returns the network structure, the priors on the tasks and hashmaps of the links and their values	
	info = (SANet, priors, preLinks, effLinks)
	return info
		
	
	
	
	



