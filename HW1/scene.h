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
    model(const char*, const char*);
    ~model();
};

class scene {
public:
    int eye[3], vat[3], vup[3], fovy, dnear, dfar, viewport[4];
    std::vector<model> object;
    const char* path;
    const char* file_name;
    scene(const char*, const char*);
    ~scene();
    void Init();
};
