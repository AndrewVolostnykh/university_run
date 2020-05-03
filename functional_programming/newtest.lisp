;;;; Do not mind. This is my first attempts in LISP

(defun myfun (x y z)
  (+ x y z))

(defun bar (x)
  (cond
    ((eql x 5) :hi!)
    ((eql x 10) :bye!)
    ((eql x 15) :boo!)))

;;; Lets try some list
(defun print-n-randoms (n max-limit)
  (dotimes (i n)
    (format t "~r~%" (random max-limit))))

(defun if-attempt (x y)
  (if (= 4 (* x y))
      (print "Don't panic")
      (print "Panic!")))

(defun when-unless-attempt (x y)
  (when (> (* x y) 100)
    (print "Dont panic")
    (print "This is a normal")))

;;; Next function using to get number
;;; from n position in input number

(defun nth-dec-digit (num n)
  (rem
   (truncate
    (/ (abs num) (expt 10 n)))
   10))

;;; I have to use nth-dec-digit to calculate:
;;; does input ticket is happy
(defun happy-ticket-p (num)
  (and (>= num 0) (<= num 999999)
       (= (+ (nth-dec-digit num 0)
	     (nth-dec-digit num 1)
	     (nth-dec-digit num 2))
	  (+ (nth-dec-digit num 3)
	     (nth-dec-digit num 4)
	     (nth-dec-digit num 5)))))

;;; This is my frist attempt to work with list
(defun try-first-list ()
    (defparameter *ages* (list (cons 'john 34)
			       (cons 'mary 23)
			       (cons 'tim 72)))
    (cdr (first (member 'mary *ages* :key 'car))))

;;; i dont undertand local vars in lisp
(defun try-let-var (x)
  (let ((myvar x))
    (if (> myvar 100)
	(print "Panic!")
	(print "All good!"))))

(defparameter *param* ())
(defun try-def-param ()
  (let ((biggest ()))
  (loop for x in '(1 2 3)
     do (setf biggest x))
  (print biggest)))

;;; Attempt to work with lists
(defun first-using-list (list)
  (loop for x in list
     do (print x)))

;;; lets find max in list?
(defun max-in-list (list)
  (let ((biggest 0))
    (loop for x in list
       do (progn
	    (if (< biggest x)
		(setf biggest x))))
    (print biggest)))

(defvar somelist '(11 22 35 66 0 -1 5))

;;; so, i wanna write this with no loop
(defun max-no-loop (list y)
  (if (null list)
      (print y)
      (if (> (car list) y)
	  (max-no-loop (cdr list) (car list))
	  (max-no-loop (cdr list) y))))

(defun outer-max (list)
  (max-no-loop list (car list)))
;;; nice, this works

;;; attempt to work with asoc list
(defvar *alist* ())

(defun asoc-list-add (alist key value)
  (cons (cons key value) alist))

(defun algor-asoc-get (key alist)
    (loop for x in alist
       do (if (= key (car x))
	      (return (cdr x)))))
