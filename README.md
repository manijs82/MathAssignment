# UnrealMathAssignment
 Futuregames assigment for the Math course

Target Grade: VG

## Game loop
Keep the amount of balls in the field in between 1 and 30 at all times.

## State and context
Balls perform different depending on their position on the platform
https://github.com/manijs82/MathAssignment/blob/2d3b39c02b54b704b840a5b0f919a66ff5e0c212/Source/MathAssignment/Obstacle.cpp#L40C2-L40C2

## Interpolation
Colors of the Balls lerp to different colors depending on their state
https://github.com/manijs82/MathAssignment/blob/2d3b39c02b54b704b840a5b0f919a66ff5e0c212/Source/MathAssignment/Obstacle.cpp#L97

## Intersection
LineLineIntersection is used for calculating the center of the field
https://github.com/manijs82/MathAssignment/blob/2d3b39c02b54b704b840a5b0f919a66ff5e0c212/Source/MathAssignment/Divider.cpp#L50C2-L50C2 <br>
Various shape intersections used as a helper for the collision system
https://github.com/manijs82/MathAssignment/blob/2d3b39c02b54b704b840a5b0f919a66ff5e0c212/Source/MathAssignment/IntersectionHelper.h#L1

