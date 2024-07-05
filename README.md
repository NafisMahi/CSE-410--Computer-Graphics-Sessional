# CSE-410--Computer-Graphics-Sessional



This repository contains all the assignments for the course CSE410.

## Installation of OpenGL on Linux

To install the necessary libraries for OpenGL on a Linux system, run the following commands in your terminal:
```bash
sudo apt update && sudo apt upgrade
sudo apt install freeglut3-dev
```


##Configuring OpenGL in VSCode

To run OpenGL programs in VSCode, follow these steps:

Install Extensions:

Install the C/C++ extension by Microsoft.
Install the Code Runner extension by Jun Han.
Configure Compiler:

Click the settings icon and ensure the correct C++ compiler is selected.
Edit the tasks.json File:

Navigate to the .vscode folder.
Add the following lines to your tasks.json file:

"args": [
    "-lGL",
    "-lGLU",
    "-lglut"
],

##Running OpenGL Programs in the Terminal
To compile and run an OpenGL program from the terminal, use the following command:

g++ <filename>.cpp -o <outputFilename> -lGL -lGLU -lglut
./<outputFilename>


##Assignments
Offline 1 (OpenGL Basics)

Specification
Solution
Offline 2 (Rasterization)

Specification
Solution
Offline 3 (Ray Tracing)

Specification
Solution
