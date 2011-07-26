(define (domain ferry)
  (:requirements :strips :equality :typing)

  (:types auto place ferry)
  (:predicates (at-ferry ?f - ferry ?l - place)
	       (at ?x - auto
		   ?y - place)
	       (empty-ferry ?f - ferry)
	       (on ?x - auto
		   ?f - ferry)
		   (can-sail ?f - ferry
		   ?x - place
		   ?y - place
		   )
		   (can-debark-at ?x - place)
		   (can-board-at ?x - place)
	)

  (:action board
	     :parameters (?f - ferry ?x - auto ?y - place)
	     :precondition (and (at ?x ?y)(at-ferry ?f ?y)(empty-ferry ?f)(can-board-at ?y))
	     :effect 
	     (and (on ?x ?f)
		   (not (at ?x ?y))
		   (not (empty-ferry ?f))))
  (:action sail
	     :parameters (?f - ferry ?x ?y - place)
	     :precondition (and (at-ferry ?f ?x) (not (= ?x ?y)) (can-sail ?f ?x ?y))
	     :effect (and (at-ferry ?f ?y)
			   (not (at-ferry ?f ?x))))
  (:action debark
	     :parameters (?f - ferry ?x - auto ?y - place)
	     :precondition (and (on ?x ?f)(at-ferry ?f ?y) (can-debark-at ?y))
	     :effect (and (not (on ?x ?f))
			   (at ?x ?y)
			   (empty-ferry ?f))))