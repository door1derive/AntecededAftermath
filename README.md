# WormholeAftermath
An open source OpenGL Sci-Fi video game.

door1derive is my second github page, after door2u (or door to understanding).

door2u is a page that is motivated by making video games and computer program that attempt to open the user's eyes, if only slightly, and even if that user is me.

The motivation behind door1derive is similar, except that I'm experimenting with Windows Copilot and GitHub Copilot to see what the differences between the projects are, and see if the projects can move faster and produce more useful results.

################

I'm writing this readme at the end of day one. So far a script for laying out a virtual research facility have started. That script is floor.py in Blender/facility_builder/. The script also includes some vector and matrix functions, utility functions, and build functions for Blender. The different function types need to be separated into libraries, and organized into some of the empty scripts that exist elsewhere in the project. Some of the code is definitely usable. There is a python function for performing 3D quaternion rotations, for example. Other functions are sure to have bugs, and some are a work in progress.

The workflow for the project will be to use a python script to export geometry into C++ headers. I will be expanding on an OpenGL engine that I designed for Node Feat, a door2u project. The engine has very minimal functionality so I hope to be expanding on that engine using Copilot. The engine has a basic frame timer, camera and movement system, and I'll be importing some custom system like a basic animation system, path system, and axis-aligned bounding box pre-collision system.

I'm also developing a very long-term project under door2u called "Children of Hope", which is a fantasy game that has a focus on music, story, and art. I've uploaded about an hour of music to soundcloud.com/door2u. Node Feat is a game about programming which uses an in-game node programming system to solve puzzles.

Wormhole Aftermath will be an investigation that involves an incident of radiation overdose at a research facility. I plan to incorporate a system that approaches a simulator for how radiation interacts with people and different materials. Part of the motivation will be to understand, and hopefully teach, how radiation "moves" through the environment. Without being able to see radiation, it's hard to know things like how persistent it is, and how materials might obstruct, defract, or repel it, for example. Also things like how radiation is removed from the body with time and medicine, depending on a persons size, age, build, health, etc. A player could have a dosage score, tools for viewing and blocking radiation, and the ability to get into the nitty-gritty calculations of it if desired.

Windows copilot has so far assisted me with the project preparation stage and some other various elements, like the project and facility names, and some ideas on how the first level might progress. Blender's scene structure will be duplicated in the game to organize scene object's relationship to each other. Copilot suggested a project structure and workflow approach which I'm in the process of implementing. After a level has been created and that geometry is readable as geometry to be passed to OpenGL, a basic camera and mouse-look system will be implemented. We discussed how a game engine might be structured, and what systems would need to be implemented to get the game working as a whole.

As of the first day there is a Blender file facility_builder, and floor.py is loaded into the file. Running it will generate two room floors in a hexagonal layout.
