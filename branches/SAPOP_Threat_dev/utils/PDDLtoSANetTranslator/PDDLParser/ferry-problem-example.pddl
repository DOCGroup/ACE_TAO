(define (problem ferry1) 
	(:domain ferry)
	(:requirements :strips :equality :typing)
	(:objects a b c - place 
		c1 c2 - auto
		f1 f2 - ferry)
    (:init 
	   (at c1 a)
	   (at c2 a)
	   
	   (at-ferry f1 a)
	   (empty-ferry f1)  
	   (can-sail f1 b c)
	   (can-sail f1 c b)
	   (can-sail f1 a b)
	   (can-sail f1 b a)
	   
	   (at-ferry f2 b)
	   (empty-ferry f2)
	   (can-sail f2 b c)
	   (can-sail f2 c b)
	   
	   (can-debark-at b)
	   (can-debark-at c)
	   
	   (can-board-at a)
	   (can-board-at b)
	   
	   )
  (:goal (and (at c1 c) (at c2 c)))
)