#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "scene.h"

// load object
scene* scene_obj;
int WindowSize[2]; // = w & h

// object
float red = 0.0f, green = 0.0f, blue = 0.0f;
float angle = 0.0f;

// background
float b_red = 0.0f, b_green = 0.0f, b_blue = 0.0f;

// camera
float camera_angle = 0.0f;
float camera_lx = -200.0f, camera_lz = -150.0f; //vector
float camera_x = 200.0f, camera_z = 150.0f; //position of camera

void Reshape(GLsizei w, GLsizei h) {
    WindowSize[0] = w;
    WindowSize[1] = h;
}

void Light() {
    glShadeModel(GL_SMOOTH);

    // z buffer enable
    glEnable(GL_DEPTH_TEST);

    // enable lighting
    glEnable(GL_LIGHTING);
    // set light property
    glEnable(GL_LIGHT0);

    for (int i = 0, j = scene_obj->lights.size(); i < j; ++i) {
        light light_tmp = scene_obj->lights[0];

        GLfloat light_position[] = {light_tmp.x, light_tmp.y, light_tmp.z, 1.0f};
        GLfloat light_specular[] = {light_tmp.sr, light_tmp.sg, light_tmp.sb, 1.0f};
        GLfloat light_diffuse[] = {light_tmp.dr, light_tmp.dg, light_tmp.db, 1.0f};
        GLfloat light_ambient[] = {light_tmp.ar, light_tmp.ag, light_tmp.sb, 1.0f};

        glPushMatrix();
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glPopMatrix();
    }

    GLfloat l_ambient[] = {scene_obj->ambient[0], scene_obj->ambient[1], scene_obj->ambient[2], 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, l_ambient);
}

void RenderScene(void) {
    // Clear buffer
    glClearColor(b_red, b_green, b_blue, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(scene_obj->viewport[0], scene_obj->viewport[1], scene_obj->viewport[2], scene_obj->viewport[3]);

    // Reset transformations
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(scene_obj->fovy, (GLfloat)WindowSize[0]/(GLfloat)WindowSize[1], 
            scene_obj->dnear, scene_obj->dfar);

    // viewing and modeling transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the camera
    gluLookAt(scene_obj->eye[0], scene_obj->eye[1], scene_obj->eye[2],
            scene_obj->vat[0], scene_obj->vat[1], scene_obj->vat[2],
            scene_obj->vup[0], scene_obj->vup[1], scene_obj->vup[2]);

    Light();

    int lastMaterial = -1;
    for (int k = 0, l = scene_obj->object.size(); k < l; ++k) {
        mesh* object = scene_obj->object[k].mesh_object;
        model model_tmp = scene_obj->object[k];
        glPushMatrix();
        glTranslatef(model_tmp.Tx, model_tmp.Ty, model_tmp.Tz);
        glRotatef(model_tmp.Angle, model_tmp.Rx, model_tmp.Ry, model_tmp.Rz);
        glScalef(model_tmp.Sx, model_tmp.Sy, model_tmp.Sz);

        for(size_t i=0;i < object->fTotal;++i) {
            // set material property if this face used different material
            if(lastMaterial != object->faceList[i].m) {
                lastMaterial = (int)object->faceList[i].m;
                glMaterialfv(GL_FRONT, GL_AMBIENT  , object->mList[lastMaterial].Ka);
                glMaterialfv(GL_FRONT, GL_DIFFUSE  , object->mList[lastMaterial].Kd);
                glMaterialfv(GL_FRONT, GL_SPECULAR , object->mList[lastMaterial].Ks);
                glMaterialfv(GL_FRONT, GL_SHININESS, &object->mList[lastMaterial].Ns);

                //you can obtain the texture name by object->mList[lastMaterial].map_Kd
                //load them once in the main function before mainloop
                //bind them in display function here
            }

            glBegin(GL_TRIANGLES);
            for (size_t j=0;j<3;++j) {
                //textex corrd. object->tList[object->faceList[i][j].t].ptr
                glNormal3fv(object->nList[object->faceList[i][j].n].ptr);
                glVertex3fv(object->vList[object->faceList[i][j].v].ptr);	
            }
            glEnd();
        }
        glPopMatrix();
    }

    glutSwapBuffers();
}

void ProcessNormalKeys(unsigned char key, int x, int y) {
    if (key == 27)
        exit(0);
    float fraction = 0.01f;
    float vec_x = scene_obj->vat[0] - scene_obj->eye[0];
    float vec_y = scene_obj->vat[1] - scene_obj->eye[1];
    float vec_z = scene_obj->vat[2] - scene_obj->eye[2];
    float up_vec_x = scene_obj->vup[0];
    float up_vec_y = scene_obj->vup[1];
    float up_vec_z = scene_obj->vup[2];
    float cross[] = {vec_y*up_vec_z - vec_z*up_vec_y, 
        vec_z*up_vec_x - vec_x*up_vec_z, vec_x*up_vec_y - vec_y*up_vec_x};

    switch (key) {
        case 'a':
            (*scene_obj).Left(cross[0]*fraction, cross[2]*fraction);
            break;
        case 'd':
            (*scene_obj).Right(cross[0]*fraction, cross[2]*fraction);
            break;
        case 'w':
            (*scene_obj).Forward(vec_x*fraction, vec_z*fraction);
            break;
        case 's':
            (*scene_obj).Backward(vec_x*fraction, vec_z*fraction);
            break;
    }
}

void ProcessSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            red = 1.0;
            green = 0.0;
            blue = 0.0;
            break;
        case GLUT_KEY_F2:
            red = 0.0;
            green = 1.0;
            blue = 0.0;
            break;
        case GLUT_KEY_F3:
            red = 0.0;
            green = 0.0;
            blue = 1.0;
            break;
        case GLUT_KEY_F4:
            b_red = 1.0;
            b_green = 0.0;
            b_blue = 0.0;
            break;
        case GLUT_KEY_F5:
            b_red = 0.0;
            b_green = 1.0;
            b_blue = 0.0;
            break;
        case GLUT_KEY_F6:
            b_red = 0.0;
            b_green = 0.0;
            b_blue = 1.0;
            break;
    }
}

int old_x = 0, old_y = 0;
void ProcessMouseActiveMotion(int x, int y) {

    float fraction = 0.01f;
    float vec_x = scene_obj->vat[0] - scene_obj->eye[0];
    float vec_y = scene_obj->vat[1] - scene_obj->eye[1];
    float vec_z = scene_obj->vat[2] - scene_obj->eye[2];
    float up_vec_x = scene_obj->vup[0];
    float up_vec_y = scene_obj->vup[1];
    float up_vec_z = scene_obj->vup[2];
    float cross[] = {vec_y*up_vec_z - vec_z*up_vec_y, 
        vec_z*up_vec_x - vec_x*up_vec_z, vec_x*up_vec_y - vec_y*up_vec_x};

    // setting red to be relative to the mouse 
    // position inside the window
    int dis_x = x > old_x? x - old_x: old_x - x;
    int dis_y = y > old_y? y - old_y: old_y - y;
    int max_dis = 50;
    if (dis_x > dis_y && dis_x < max_dis) {
        if (x < 0 || x > WindowSize[0]) {}
        else {
            if (old_x > x) {
                fraction *= (old_x - x);
                (*scene_obj).TurnLeft(fraction, cross[0], cross[2]);
            }
            else if (old_x < x){
                fraction *= (x - old_x);
                (*scene_obj).TurnRight(fraction, cross[0], cross[2]);
            }
        }
    }
    else if (dis_y > dis_x && dis_y < max_dis) {
        // setting green to be relative to the mouse 
        // position inside the window
        if (y < 0 || y > WindowSize[1]) {}
        else {
            fraction = 1.0f;
            if (old_y > y) {
                fraction *= (old_y - y);
                (*scene_obj).TurnUp(fraction, cross[0], cross[1], cross[2]);
            }
            else if (old_y < y){
                fraction *= (y - old_y);
                (*scene_obj).TurnDown(fraction, cross[0], cross[1], cross[2]);
            }
        }
    }

    old_x = x;
    old_y = y;
}

int main(int argc, char **argv) {

    char file[100];
    std::cout << argv[1] << std::endl;
    if (argc != 3) {
        std::cout << "Usage: ./main {scene dir} {main scene file}\n" << std::endl;
        return 0;
    }
    scene_obj = new scene(argv[1], argv[2]);

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(800,800);
    glutCreateWindow("Test");

    // register callbacks
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(Reshape);
    glutIdleFunc(RenderScene);

    // keyboard detect
    glutKeyboardFunc(ProcessNormalKeys);
    glutSpecialFunc(ProcessSpecialKeys);

    // mouse detect
    //glutMouseFunc(ProcessMouse);
    glutMotionFunc(ProcessMouseActiveMotion);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}
