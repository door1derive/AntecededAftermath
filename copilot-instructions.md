Hello.

###################

# The discussion so far.

This project has been started with the help of Windows copilot.

We started off by establishing the desire to create an OpenGL project. Blender files can be thought of as the starting point for the project. An export python script will be created to place the mesh data into geometry arrays inside a C++ header. We discussed exporting vertex, normal, color, material id, and uv coordinates. I have a script that does this on another account, but it's a pretty big mess. It might contain errors with regard to how it pads arrays to keep data arrays parallel. It might be better to just start from scratch.

I have those arrays sorted according to which triangle a particular vertex belongs to. It was suggested that that array can be condensed to reduce the amount of data stored and passed to OpenGL. That idea appeals to me because I will probably be pushing the amount of geometry that OpenGL can handle on my computer.

A basic engine format was discussed. Basic chrono functionality sets up the framerate at 60 frames per second. A game loop first checks for user input. If none is received, the game functions are run. I was previously using a Linux system and calling X11 to check user input. But I want the game to be compatible with Windows, Mac, and Linux systems, so I would like to switch to STL. The game will use basic WASD and mouse look controls. I have functions that track those in another project, door2u/nodefeat, so I won't need help with that. My camera is basically an object holding a location vector and three unit vectors for each axis. Copilot suggested we add clipping planes to the camera and I think that's a good idea. The game will hold off on implementing collisions for now. Node Feat used a path system that tracked which triangle region the character was in. That worked well for me because it was lightweight, stable, and could be used to restrict player movement and determine the player's proximity to other objects. The system is not quite finished but it's close. I will also want to implement a raycast system by tracking bounding boxes that follow targets and sort their minimum and maximum into a game-wide array. Then the ray itself could be separated into x, y, and z to quickly determine the next intersecting bounding box. If an intersection with a bounding box was detected, then maybe the ray could be rotated around the object and tested with an array of AABB around each object polygon. 
