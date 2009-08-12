;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Simple Vehicle domain where a person has to get in to a vehicle,
;;; drive it somewhere, get out, and return to some other location.
;;; Recursion is a problem in this domain because the roads and
;;; bridges go both directions.

(define (domain travel)
  (:requirements :strips :equality)
  (:predicates (road ?from ?to)
	       (at ?thing ?place)
	       (mobile ?thing)
	       (bridge ?from ?to)
	       (person ?p)
	       (vehicle ?v)
	       (driving ?p ?v))
	       
  (:action Drive
	     :parameters (?thing ?from ?to)
	     :precondition (and (road ?from ?to)
				 (at ?thing ?from)
				 (mobile ?thing)
				 (not (= ?from ?to)))
	     :effect (and (at ?thing ?to) (not (at ?thing ?from))))
  (:action Cross
	     :parameters (?thing ?from ?to)
	     :precondition (and (bridge ?from ?to)
				 (at ?thing ?from)
				 (mobile ?thing)
				 (not (= ?from ?to)))
	     :effect (and (at ?thing ?to) (not (at ?thing ?from))))
  (:action Board
	     :parameters (?person ?place ?vehicle)
	     :precondition (and (at ?person ?place)
				 (person ?person)
				 (vehicle ?vehicle)
				 (at ?vehicle ?place))
	     :effect (and (driving ?person ?vehicle)
			   (mobile ?vehicle)
			   (not (at ?person ?place))
			   (not (mobile ?person))))  
  (:action Disembark
	     :parameters (?person ?place ?vehicle)
	     :precondition (and (person ?person)
				 (vehicle ?vehicle)
				 (driving ?person ?vehicle)
				 (at ?vehicle ?place))
	     :effect (and (at ?person ?place)
			   (mobile ?person)
			   (not (driving ?person ?vehicle))
			   (not (mobile ?vehicle))))
  )