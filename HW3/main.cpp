#include <iostream>
#include <string>
#include "GL/glew.h"
#include "GL/glut.h"
#include "SOIL/SOIL.h"
#include "scene.h"
#include "shader.h"
#define TEX_NUM 3
#define MIPMAP

// load object
scene* scene_obj;
int WindowSize[2]; // = w & h

// texture
GLuint texObject[TEX_NUM];
std::string picture[3] = {  "TestScene/diffuse.bmp",
                            "TestScene/normal.bmp",
                            "TestScene/specular.bmp" };

// Shader
GLhandleARB MyShader;
    
// object & background
float red = 0.0f, green = 0.0f, blue = 0.0f;
float angle = 0.0f;
float b_red = 0.0f, b_green = 0.0f, b_blue = 0.0f;

void LoadShaders() {
    MyShader = glCreateProgram();
    if(MyShader != 0)
    {   
        ShaderLoad(MyShader, "ShaderCode/PhongShading.vs", GL_VERTEX_SHADER);
        ShaderLoad(MyShader, "ShaderCode/PhongShading.fs", GL_FRAGMENT_SHADER);
    }   
}

GLuint v, f, f2, p, g;			//Handlers for our vertex, geometry, and fragment shaders
char *textFileRead(char *fn) {
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
        		fseek(fp, 0, SEEK_END);
        		count = ftell(fp);
        		rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		
		}
	}

	return content;
}

//Function from: http://www.evl.uic.edu/aej/594/code/ogl.cpp
//Read in a textfile (GLSL program)
// we can use this to write to a text file
int textFileWrite(char *fn, char *s) {
	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}

//Got this from http://www.lighthouse3d.com/opengl/glsl/index.php?oglinfo
// it prints out shader info (debugging!)
void printShaderInfoLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("printShaderInfoLog: %s\n",infoLog);
        free(infoLog);
	}else{
		printf("Shader Info Log: OK\n");
	}
}

//Got this from http://www.lighthouse3d.com/opengl/glsl/index.php?oglinfo
// it prints out shader info (debugging!)
void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("printProgramInfoLog: %s\n",infoLog);
        free(infoLog);
    }else{
		printf("Program Info Log: OK\n");
	}
}

void LoadTexture() {

    int width[3], height[3];
    unsigned char* image[3]; 
    for (int i = 0; i < 3; ++i) {
        image[i] = SOIL_load_image(picture[i].c_str(), &width[i], &height[i], 0, SOIL_LOAD_RGB);
        if (image[i] == NULL)
            std::cout << "Load image has error: " << picture[i] << std::endl;
        else
            std::cout << "Load image: " << picture[i] << std::endl;
    }

    glGenTextures(TEX_NUM, texObject);
    for (int i = 0; i < 3; ++i) {
        glBindTexture(GL_TEXTURE_2D, texObject[i]);
#ifdef MIPMAP
        texObject[i] = SOIL_load_OGL_texture(picture[i].c_str(), SOIL_LOAD_AUTO, 
                SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#else
        texObject[i] = SOIL_load_OGL_texture(picture[i].c_str(), SOIL_LOAD_AUTO, 
                SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif

    }
}

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

    for (int i = 0, j = scene_obj->lights.size(); i < j; ++i) {

        light light_tmp = scene_obj->lights[0];

        GLfloat light_position[] = {light_tmp.x, light_tmp.y, light_tmp.z, 1.0f};
        GLfloat light_specular[] = {light_tmp.sr, light_tmp.sg, light_tmp.sb, 1.0f};
        GLfloat light_diffuse[] = {light_tmp.dr, light_tmp.dg, light_tmp.db, 1.0f};
        GLfloat light_ambient[] = {light_tmp.ar, light_tmp.ag, light_tmp.sb, 1.0f};
        
        glLightfv(GL_LIGHT0 + i, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0 + i, GL_AMBIENT, light_ambient);
        glEnable(GL_LIGHT0 + i);
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

        glUseProgram(MyShader);

        // select texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texObject[0]);
        GLint texture_location = glGetUniformLocation(MyShader, "diffuseTexture");
        if(texture_location == -1) 
            printf("Cant find texture name: diffuseTexture\n");
        else 
            glUniform1i(texture_location, 0); 

        // normal map
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        int normal_location = glGetUniformLocation(MyShader, "normalTexture");  
        if(normal_location == -1) 
            printf("Cant find texture name: normalTexture\n");
        else 
            glUniform1i(normal_location, 1); 
        glBindTexture(GL_TEXTURE_2D, texObject[1]);

        glActiveTexture(GL_TEXTURE2);
        glEnable(GL_TEXTURE_2D);
        int location = glGetUniformLocation(MyShader, "colorTexture");
        if(location == -1) 
            printf("Cant find texture name: colorTexture\n");
        else 
            glUniform1i(location, 2); 
        glBindTexture(GL_TEXTURE_2D, texObject[2]);

        int texture_num = 1;

        for(size_t i=0;i < object->fTotal;++i) {
            // set material property if this face used different material
            if(lastMaterial != object->faceList[i].m) {
                lastMaterial = (int)object->faceList[i].m;
                glMaterialfv(GL_FRONT, GL_AMBIENT  , object->mList[lastMaterial].Ka);
                glMaterialfv(GL_FRONT, GL_DIFFUSE  , object->mList[lastMaterial].Kd);
                glMaterialfv(GL_FRONT, GL_SPECULAR , object->mList[lastMaterial].Ks);
                glMaterialfv(GL_FRONT, GL_SHININESS, &object->mList[lastMaterial].Ns);
            }

            glBegin(GL_TRIANGLES);
            for (size_t j=0;j<3;++j) {
                //textex corrd. object->tList[object->faceList[i][j].t].ptr
                for (int mm = 0; mm < texture_num; ++mm) {
                    glMultiTexCoord3fv(GL_TEXTURE0 + mm, object->tList[object->faceList[i][j].t].ptr);
                }
                glNormal3fv(object->nList[object->faceList[i][j].n].ptr);
                glVertex3fv(object->vList[object->faceList[i][j].v].ptr);	
            }
            glEnd();
        }
        glActiveTexture(GL_TEXTURE2);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE1);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
    }

    glUseProgram(0);
    glutSwapBuffers();
    //glutPostRedisplay();
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

    float fraction = (scene_obj->GetFraction())/100;
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
            fraction = (scene_obj->GetFraction());
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

    scene_obj = new scene("TestScene/", "ShaderAssignment");

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(500,500);
    glutCreateWindow("Shadding");

    GLenum glew_error;
    if((glew_error = glewInit()) != GLEW_OK) return -1;


    // test
	if (glewIsSupported("GL_VERSION_2_1"))
        printf("Ready for OpenGL 2.1\n");
	else {
		printf("OpenGL 2.1 not supported\n");
		exit(1);
	}

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
		printf("Ready for GLSL - vertex, fragment, and geometry units\n");
	else {
		printf("Not totally ready :( \n");
		exit(1);
	}

    // texture
    LoadShaders();
    LoadTexture();

    // register callbacks
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(Reshape);
    glutIdleFunc(RenderScene);

    // shader

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
