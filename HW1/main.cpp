#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <glm/glm.hpp>

// object
float red = 0.0f, green = 0.0f, blue = 0.0f;
float angle = 0.0f;

// background
float b_red = 0.0f, b_green = 0.0f, b_blue = 0.0f;

// camera
float camera_angle = 0.0f;
float camera_lx = 0.0f, camera_lz = -1.0f; //vector
float camera_x = 0.0f, camera_z = 5.0f; //position of camera


void Reshape(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;

    float ratio =  w * 1.0 / h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}


void RenderScene(void) {

    glClearColor(b_red, b_green, b_blue, 0.0f);

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(red, green, blue);

    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(camera_x, 1.0f, camera_z,
            camera_x + camera_lx, 1.0f,  camera_z + camera_lz,
            0.0f, 1.0f,  0.0f);

    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glutSolidTeapot(1.0);
    //angle+=0.5f;


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

    // OpenGL init
    glEnable(GL_DEPTH_TEST);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}
