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

void light() {
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_position[] = {150.0, 150.0, 150.0, 1.0};

	glShadeModel(GL_SMOOTH);

	// z buffer enable
	glEnable(GL_DEPTH_TEST);

	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

void RenderScene(void) {
    // Clear buffer
    glClearColor(b_red, b_green, b_blue, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glColor3f(red, green, blue);
    glViewport(0, 0, WindowSize[0], WindowSize[1]);

    // Reset transformations
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //float ratio =  WindowSize[0] * 1.0 / WindowSize[1];
    //gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	gluPerspective(60.0, (GLfloat)WindowSize[0]/(GLfloat)WindowSize[1], 1.0, 1000.0);

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    // Set the camera
    gluLookAt(camera_x, 300.0f, camera_z,
            camera_x + camera_lx, 1.0f, camera_z + camera_lz,
            0.0f, 1.0f,  0.0f);

    //glRotatef(angle, 0.0f, 1.0f, 0.0f);
    //glutSolidTeapot(1.0);
    //angle+=0.5f;
    
	light();

	int lastMaterial = -1;
    for (int k = 0, l = scene_obj->object.size(); k < l; ++k) {
        mesh* object = scene_obj->object[k].mesh_object;

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
    }

    glutSwapBuffers();
}

void ProcessNormalKeys(unsigned char key, int x, int y) {
    if (key == 27)
        exit(0);
    float fraction = 0.1f;
    switch (key) {
        case 'a':
            camera_angle -= 0.01f;
            camera_lx = sin(camera_angle);
            camera_lz = -cos(camera_angle);
            break;
        case 'd':
            camera_angle += 0.01f;
            camera_lx = sin(camera_angle);
            camera_lz = -cos(camera_angle);
            break;
        case 'w':
            camera_x += camera_lx * fraction;
            camera_z += camera_lz * fraction;
            break;
        case 's':
            camera_x -= camera_lx * fraction;
            camera_z -= camera_lz * fraction;
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

int main(int argc, char **argv) {

    char file[100];
    std::cout << argv[1] << std::endl;
    scene_obj = new scene(argv[1]);

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(700,700);
    glutCreateWindow("Test");

    // register callbacks
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(Reshape);
    glutIdleFunc(RenderScene);

    // keyboard detect
    glutKeyboardFunc(ProcessNormalKeys);
    glutSpecialFunc(ProcessSpecialKeys);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}
