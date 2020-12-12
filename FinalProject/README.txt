Name: Vlad Muresan (vmuresan@mymail.mines.edu), Emelyn Pak (epak@mymail.mines.edu), Alex Langfield (alangfield@mymail.mines.edu), Liam Stacy (liamstacy@mymail.mines.edu)

Guild Name: VAOs VBOs and MVPs

Final Project - The Grand Reopening

EPILEPSY WARNING - read the line about bugs/features below for more info.

The main.cpp file draws our scene with our character and some other models. 
Santa is firing ornaments out of a bazooka, we have a Christmas tree with a color-chaniging tree topper, and candy cane spikes. Whenever an ornament missile flies over, our character jumps in fear for a short amount of time.

This program is run like most of the other assignments for this class - WASD to move, click & drag to move the camera, and holding ctrl while clicking & dragging zooms in and out. 
Pressing 1 will switch between freecam and arcball cam, pressing x will move backwards in free cam, pressing the space bar will move you forward in the free cam. 
Pressing 2 will toggle the first person camera. 

There are no special actions needed to compile the code, just make sure all the files are in the working directory.

There are no game-breaking bugs or anything, but our custom shader is a bit interesting. Our first custom shader simulates snow accumulation. 
Whenever a snowflake hits the ground, a point on the ground quad is raised up, and experiences a slight displacement. 
There is another base below it to prevent it from seeing through the floor when it is raised. This is always enabled. 
The second custom shader is dependent on point light attenuation. 
The attenuation quadratic and linear values are dependent on the position of the point light. 
The position values are altered in our cpp and checked against our shaders to determine the color attinuation. 
If you would like to see it, press the V key, but be careful because it contains flashing lights that are maybe bad enough to be dangerous to people with Epilepsy. 
Also, the tree toper is a point light that changes colors.

Our program does not read any files as input.

At first, all of the groupmates worked together on starting the project, then we broke up for a little bit, then we rejoined at the end before submitting. 
When we broke up, Alex and Emelyn worked on the shaders, Vlad worked on adding details to Santa and creating the logic for the moving ornaments, and Liam worked on the snow accumulation on the ground. Here is a more detailed list of contributions:
Vlad: Santa's arms, bazooka, ornament models & motion of ornament models.
Liam: Character animation, snow accumulation/displacement shader, snow spawning and regeneration.
Emelyn: Drew santa, other objects like christmas tree, animated santa's motion, worked on custom lighting shader.
Alex: Implemented snow falling shader with Emelyn. And wrote the attenuation shader with Emelyn.

This project took us around 25 hours (per person).

This assignment was pretty tough. The labs helped maybe 3/10 since they did not really provide any new insights into required features such as creating custom shaders or hierarchical animations.

This assignment was kind of fun. The group agrees on a 5/10.