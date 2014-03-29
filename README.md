2014 Computer Graphic homework.

## Assignment 1 Phase 1: Draw Object with Lighting
Program Description
---
You are required to implement an OpenGL program that can read object files and display 3D models correctly on the screen.  You also need to implement keyboard and mouse motion functions to control the camera. 


Requirement
---
- The object files, material files, viewing files, light files, and scene files would be given.  
    - Object files (\*.obj) : describe the coordinate of the vertices and the normal vectors of a 3D model.
    - Material files (\*.mtl) : describe the material attribution of the 3D model.
    - Viewing files (\*.view) : describe the camera, the view frustum and viewport.
    - Light files (\*.light) : describe the lights and the ambient color.
    - Scene files (\*.scene) : a list of 3D models, also describe the translation vector of each model.
- The object file loader is already given. You can use it to read the object and material files.
- You need implement the scene, view, and light file loader.
- You need to use OpenGL functions to setup the camera, view frustum, light source, vertex coordinate, normal vector, and the material. Then display the model correctly on the screen. All information you need is described in the files above.
- The scene file describes the scaling, rotation, and translation of each model.
    - First, you should scale the object.
    - Next, you should rotate it to the right direction.
    - Finally, you should move each of model to the correct position.
- You should implement a camera class.
    - Keyboard : Move the camera.
        - 'w' : forward
        - 'a' : left
        - 's' : backward
        - 'd' : right 
    - Mouse : Rotate the camera.
        - Right : turn right
        - Left : turn left
        - Up : turn up
        - Down : turn down 
