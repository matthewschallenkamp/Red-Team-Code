# Ski Jumping
Taken from https://open.kattis.com/problems/skijumping

Tags: Math, Derivative, Physics, Quadratic Formula

## Problem Summary
Given the shape of a ski jump and the hill a skiier will land on, determine how a skiier will land.

## Solution
This problem is entirely math-oriented. Before the solution can be written, a number of formulas need to be found from the original formulae given.

See the problem for a picture and description of the ski jump and parameters.

We are given the parameters:
* H: The height of the landing slope
* L: The length of the landing slope 
* p: The drop from the bottom of the jump to the top of the landing slope
* j: The height of the jump 

We also have the gravitational constant g defined g = 9.81 m/s^2

The landing slope is made up of three parts: The top of the slope, the middle of the slope, and the bottom of the slope.
The top and middle are defined by hyperbolic equations given as part of the problem, and the bottom of the slope
is just the horizontal line y=0. These equations make up the piecewise equation h(l).

We're also given f(l), an equation for the path the skier takes through the air.

### Equations
#### Velocity gained by falling
According to basic physics, all potential energy is converted to kinetic over the course of falling from some height. Solve when the formulae given for kinetic and potential energy are equal to each other. Since all the rest of the required formulae use velocity-squared, we skip the square root.

V0^2 = g*j*2

#### Function intersections
To find the horizontal distance traveled, we need to find where the skier's path (f(l)) intersects the slope (h(l)).
To do this, we solve the equation f(l) = h(l) for each part of h(l). The resulting equations, in order top, middle bottom:
* sqrt(-p/(-g/(2*v0_2) + 2*H/(L*L)))
* ((4*H)/L + sqrt((16*H^2)/(L^2) - 4*((2*H)/(L^2) + (g/(2*v0_2)))*(H-p)))/(2*((2*H)/(L^2) + (g/(2*v0_2))))
* sqrt(((-H - p)*2*v0_2)/-g)

Before performing the square root in the first equation, you should check that you are not taking the square root of
a negative number. If the determinant is negative in that equation, it means the skiier does not land on the top part
of the slope.

If the skiier does not land on the top part, you can use the second equation to see where they land on the middle part.
However, you need to check that the resulting distance is < L. If it's > L, then the skiier lands on the horizontal part and the third equation should be used to find the landing location.

Once this distance l is found, it can be plugged into the h(l) function to find the height at the landing
location. Using the velocity equation above, it can be determined how fast the skiier is traveling when they land.
The variable j should be replaced with (j + p + H-h(l)) - This is the entire vertical distance traveled by the skiier. Be sure to take the square root at the end to get v, not v^2.

#### Function derivatives
The last set of calculations needed are the derivatives of all four functions. The derivatives are used to determine
the angle of impact of the skiier. The derivatives in order top, middle, bottom, f(l) follow:
* -4*H*l/(L^2)
* 4*H*l/(L^2) - 4*H/L
* 0
* (-g*l)/v0_2

The angle of impact can be found with the equation (atan(skiier angle) - atan(slope angle)) * 180/pi.
The skiier angle can be found by plugging the landing distance l into the derivative of F(l), and the slope
angle can be found by plugging that l into the derivative corresponding to the part of the slope landed on.

## Implementation notes
For each problem, my solution checks first which part of the slope is landed on and then assigns a couple of function
objects which are used for the rest of the calculations. This allows for more reused code.