#include <iostream>
#include "scene.h"

scene::scene(const char* path) {
    this->path = path;
    Init();
}

scene::~scene() {
    for (int i = 0, j = this->object.size(); i < j; ++i)
        (this->object)[i].~model();
}

void scene::Init() {
    FILE* scene;
    std::string scene_file = this->path;
    scene_file += "scene.scene";
    scene = fopen(scene_file.c_str(), "r");

	if (!scene) {
        std::cout<< "Can not open scene object File \"" << scene_file << "\" !" << std::endl;
		return;
	}

    while (!feof(scene)) {
        char type[20], obj_file[50];
        fscanf(scene, "%s", type);
        
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
}

model::model(const char* path, const char* obj_file) {
    this->mesh_object = new mesh(path, obj_file);
}

model::~model() {
}
