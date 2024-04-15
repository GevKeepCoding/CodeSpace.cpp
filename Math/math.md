# [What Every Computer Scientist Should Know About Floating-Point Arithmetic]https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
# [Floating Points](https://randomascii.wordpress.com/category/floating-point/)

todo: read above article


---
## [sometimes floating point math is perfect](https://randomascii.wordpress.com/2017/06/19/sometimes-floating-point-math-is-perfect/)

* Integers that are small enough will be converted to float/double with no error,
	and vice-versa
* Floating-point addition, subtraction, and multiplication of integral values will be
	exact as long as the inputs are exact and the results are small enough
* The square root of a perfect square(such as sqrt(1369.0)) will be exact
* Multiplication and division by power of two is exact as long as the result is in
	normalized range
* If you subtract two numbers that are close to each other, where ‘close’ means within a factor of two of each (Sterbenz’s lemma) other then the result will be exact (although exact in this case sometimes just means catastrophic cancellation)


---
[](https://randomascii.wordpress.com/2012/01/23/stupid-float-tricks-2/)
1. Adjacent floats have adjacent integer representations
1. Incrementing the integer representation of a float moves to the next representable float, moving away from zero

---
## [nextafter](https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/h0dff77w(v=vs.100)?redirectedfrom=MSDN)
_nextafter(x, y) => returns the closest representable neighbor of x in the direction toward y.

---
TODO:https://cp-algorithms.com



---
### Absract Algebra
- [ ] Abstract Algebra, 3rd Edition
