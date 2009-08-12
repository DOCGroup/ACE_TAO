(define (domain ferry)
  (:requirements :strips :equality :typing)

  (:types auto place ferry)
  (:constants the-ferry - ferry)
  (:predicates (at-ferry ?l - place)
	       (at ?x - auto
		   ?y - place)
	       (empty-ferry)
	       (on ?x - auto
		   ?f - ferry))

  (:action board
	     :parameters (?x - auto ?y - place)
	     :precondition (and (at ?x ?y)(at-ferry ?y)(empty-ferry))
	     :effect 
	     (and (on ?x the-ferry)
		   (not (at ?x ?y))
		   (not (empty-ferry))))
  (:action sail
	     :parameters (?x ?y - place)
	     :precondition (and (at-ferry ?x) (not (= ?x ?y)))
	     :effect (and (at-ferry ?y)
			   (not (at-ferry ?x))))
  (:action debark
	     :parameters (?x - auto ?y - place)
	     :precondition (and (on ?x the-ferry)(at-ferry ?y))
	     :effect (and (not (on ?x the-ferry))
			   (at ?x ?y)
			   (empty-ferry))))