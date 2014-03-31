#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstring>
#include "mesh.h"

class model {
public:
    char obj_file[50];
    float Sx, Sy, Sz, Angle, Rx, Ry, Rz, Tx, Ty, Tz;
    mesh* mesh_object;
    model(const char*);
    ~model();
};

class scene {
public:
    std::vector<model> object;
    std::string path;
    scene(const char*);
    ~scene();
    void Init();
};
