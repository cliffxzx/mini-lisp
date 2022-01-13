(+) (+ (* 5 2) -) (print-num 1) (print-num 2) (print-num 3) (print-num 4) (print-num 0) (print-num -123) (print-num 456) (+ 1 2 3) (* 4 5 6) (print-num (+ 1 (+ 2 3 4) (* 4 5 6) (/ 8 3) (mod 10 3))) (print-num (mod 10 4)) (print-num (- (+ 1 2) 4)) (print-num -256) (print-num (mod 10 (+ 1 2))) (print-num (* (/ 1 2) 4)) (print-num (- (+ 1 2 3 (- 4 5) 6 (/ 7 8) (mod 9 10)) 11)) (print-bool #t) (print-bool #f) (print-bool (and #t #f)) (print-bool (and #t #t)) (print-bool (or #t #f)) (print-bool (or #f #f)) (print-bool (not #t)) (print-bool (not #f)) (print-bool (or #t #t #f)) (print-bool (or #f (and #f #t) (not #f))) (print-bool (and #t (not #f) (or #f #t) (and #t (not #t)))) (print-num (if #t 1 2)) (print-num (if #f 1 2)) (print-num (if (< 1 2) (+ 1 2 3) (* 1 2 3 4 5))) (print-num (if (= 9 (* 2 5)) 0 (if #t 1 2))) (define x 1) (print-num x) (define y (+ 1 2 3)) (print-num y) (define a (* 1 2 3 4)) (define b (+ 10 -5 -2 -1)) (print-num (+ a b)) (print-num
 ((lambda (x) (+ x 1)) 3))

 (print-num
 ((lambda (a b) (+ a b)) 4 5))

 (define x 0)

 (print-num
 ((lambda (x y z) (+ x (* y z))) 10 20 30))


 (print-num x)

 (define foo
 (lambda (a b c) (+ a b (* b c))))

 (print-num (foo 10 9 8))

 (define bar (lambda (x) (+ x 1)))

 (define bar-z (lambda () 2))

 (print-num (bar (bar-z)))

 (define fact
 (lambda (n) (if (< n 3) n
 (* n (fact (- n 1))))))

 (print-num (fact 2))
 (print-num (fact 3))
 (print-num (fact 4))
 (print-num (fact 10))

 (define fib (lambda (x)
 (if (< x 2) x (+
 (fib (- x 1))
 (fib (- x 2))))))

 (print-num (fib 1))
 (print-num (fib 3))
 (print-num (fib 5))
 (print-num (fib 10))
 (print-num (fib 20))

 (define min
 (lambda (a b)
 (if (< a b) a b)))

 (define max
 (lambda (a b)
 (if (> a b) a b)))

 (define gcd
 (lambda (a b)
 (if (= 0 (mod (max a b) (min a b)))
 (min a b)
 (gcd (min a b) (mod (max a b) (min a b))))))

 (print-num (gcd 100 88))

 (print-num (gcd 1234 5678))

 (print-num (gcd 81 54))

 (+ 1 2 3 (or #t #f)) (define f
 (lambda (x)
 (if (> x 10) 10 (= x 5))))

 (print-num (* 2 (f 4)))

 (define dist-square
 (lambda (x y)
 (define square (lambda (x) (* x x)))
 (+ (square x) (square y))))

 (print-num (dist-square 3 4))

 (define diff
 (lambda (a b)
 (define abs
 (lambda (a)
 (if (< a 0) (- 0 a) a)))
 (abs (- a b))))

 (print-num (diff 1 10))
 (print-num (diff 10 2))

 (define add-x
 (lambda (x) (lambda (y) (+ x y))))

 (define z (add-x 10))

 (print-num (z 1))

 (define foo
 (lambda (f x) (f x)))

 (print-num
 (foo (lambda (x) (- x 1)) 10))
