#include <iostream>
#include "scene.h"

scene::scene(const char* path, const char* file) {
    this->path = path;
    this->file_name = file;
    Init();
}

scene::~scene() {
    for (int i = 0, j = this->object.size(); i < j; ++i)
        (this->object)[i].~model();
}

void scene::Init() {
    FILE* scene;

    // Load models
    std::string scene_file = this->path;
    scene_file += this->file_name;
    scene_file += ".scene";
    scene = fopen(scene_file.c_str(), "r");

	if (!scene) {
        std::cout<< "Can not open scene object File \"" << scene_file << "\" !" << std::endl;
		return;
	}

    char type[20], obj_file[50];
    while (fscanf(scene, "%s", type) != EOF) {
        
        if (!strcmp(type, "model")) {
            fscanf(scene, "%s", obj_file);
            std::string object_file = this->path;
            object_file += obj_file;
            model* object_tmp = new model(this->path, object_file.c_str());
            fscanf(scene, "%f%f%f%f%f%f%f%f%f%f", &object_tmp->Sx, &object_tmp->Sy, &object_tmp->Sz, 
                    &object_tmp->Angle, &object_tmp->Rx, &object_tmp->Ry, &object_tmp->Rz, 
                    &object_tmp->Tx, &object_tmp->Ty, &object_tmp->Tz);
            (this->object).push_back(*object_tmp);
        }
    }
    if (scene) fclose(scene);

    // Load view
    scene_file = this->path;
    scene_file += this->file_name;
    scene_file += ".view";
    scene = fopen(scene_file.c_str(), "r");

	if (!scene) {
        std::cout<< "Can not open scene view File \"" << scene_file << "\" !" << std::endl;
		return;
	}
    
    std::cout << "Load view file:\"" << scene_file << "\"" << std::endl;

    while (fscanf(scene, "%s", type) != EOF) {
        
        int eye[3], vat[3], vup[3], fovy, dfar, viewport[4];
        if (!strcmp(type, "eye")) {
            fscanf(scene, "%d%d%d", &this->eye[0], &this->eye[1], &this->eye[2]);
        }
        else if (!strcmp(type, "vat")) {
            fscanf(scene, "%d%d%d", &this->vat[0], &this->vat[1], &this->vat[2]);
        }
        else if (!strcmp(type, "vup")) {
            fscanf(scene, "%d%d%d", &this->vup[0], &this->vup[1], &this->vup[2]);
        }
        else if (!strcmp(type, "fovy")) {
            fscanf(scene, "%d", &this->fovy);
        }
        else if (!strcmp(type, "dnear")) {
            fscanf(scene, "%d", &this->dnear);
        }
        else if (!strcmp(type, "dfar")) {
            fscanf(scene, "%d", &this->dfar);
        }
        else if (!strcmp(type, "viewport")) {
            fscanf(scene, "%d%d%d%d", &this->viewport[0], &this->viewport[1], 
                    &this->viewport[2], &this->viewport[3]);
        }
    }
    if (scene) fclose(scene);
}

model::model(const char* path, const char* obj_file) {
    this->mesh_object = new mesh(path, obj_file);
}

model::~model() {
}
