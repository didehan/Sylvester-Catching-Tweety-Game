
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"
#define ENEMY_SPEED 2
#define PLAYER_SPEED 5

#define WINDOW_WIDTH  666
#define WINDOW_HEIGHT 666

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight, count=0;
bool pause= true, gamesitu; // current Window width and height, gamesitu=game over or not
float MouseX, MouseY,A,Sx=0,Sy=0;//A=angle, Sx and Sy = variables to move sylvester
bool out;


typedef struct {
    vec_t pos;
    float angle;
} tweety_t;

typedef struct {
    vec_t pos;
    vec_t vel;  //velocity
    bool  visibility; // visibility: true if the player sees the enemy
} sylvester_t;

tweety_t P = { { MouseX, MouseY }, 45 };
sylvester_t  E = { { 0, 200 } };

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}
void sylvester()
{
    glColor3f(0, 0, 0);//black triange face
    glBegin(GL_TRIANGLES);
    glVertex2f(0+E.pos.x,60+E.pos.y);
    glVertex2f(-30 + E.pos.x,0 + E.pos.y);
    glVertex2f(30 + E.pos.x,0 + E.pos.y);
    glEnd();
    glColor3f(1, 1, 1);//white part of face 
    glBegin(GL_TRIANGLES);
    glVertex2f(0 + E.pos.x, 15 + E.pos.y);
    glVertex2f(-40 + E.pos.x, 0 + E.pos.y);
    glVertex2f(0 + E.pos.x, -10 + E.pos.y);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(0 + E.pos.x, 15 + E.pos.y);
    glVertex2f(40 + E.pos.x, 0 + E.pos.y);
    glVertex2f(0 + E.pos.x, -10 + E.pos.y);
    glEnd();
    glColor3d(0, 0, 0);//mouth
    circle(0 + E.pos.x, 0 + E.pos.y, 5);
    glColor3d(1, 0, 0);//nose
    circle(0 + E.pos.x,7 + E.pos.y,7);
    glColor3d(1, 1, 1);//eyes
    circle(-10 + E.pos.x, 25 + E.pos.y, 6);
    circle(10 + E.pos.x, 25 + E.pos.y, 6);
    circle(-10 + E.pos.x, 21 + E.pos.y, 6);
    circle(10 + E.pos.x, 21 + E.pos.y, 6);
    glColor3d(0, 0, 0);
    circle(-10 + E.pos.x, 22 + E.pos.y, 3);
    circle(10 + E.pos.x, 22 + E.pos.y, 3);
    circle(13 + E.pos.x, 45 + E.pos.y, 7);//ears
    circle(-13 + E.pos.x, 45 + E.pos.y, 7);
    circle(-13 + E.pos.x, 42 + E.pos.y, 7);
    circle(13 + E.pos.x, 42 + E.pos.y, 7);
    glColor3d(1, 1, 1);
    circle(13 + E.pos.x, 45 + E.pos.y, 5);
    circle(-13 + E.pos.x, 45 + E.pos.y, 5);
    circle(-13 + E.pos.x, 42 + E.pos.y, 5);
    circle(13 + E.pos.x, 42 + E.pos.y, 5);
   
}
void tweety()
{
    glColor3d(1, 128/255. , 0);//feet
    circle(P.pos.x+8, P.pos.y -20, 5);
    circle(P.pos.x -7, P.pos.y -20, 5);
    glColor3d(1, 1, 0);//body
    circle(P.pos.x, P.pos.y + 10, 15);
    glRectf(P.pos.x + 10, P.pos.y, P.pos.x -10, P.pos.y -20);
    glBegin(GL_TRIANGLES);//wings
    glVertex2f(P.pos.x -10, P.pos.y);
    glVertex2f(P.pos.x -10, P.pos.y -10);
    glVertex2f(P.pos.x -30, P.pos.y);
    glEnd();
    glBegin(GL_TRIANGLES);//wings
    glVertex2f(P.pos.x + 10, P.pos.y);
    glVertex2f(P.pos.x + 10, P.pos.y -10);
    glVertex2f(P.pos.x + 30, P.pos.y);
    glEnd();
    glColor3d(1, 1, 1);//eyes
    circle(P.pos.x + 6, P.pos.y + 10, 4);
    circle(P.pos.x -6, P.pos.y + 10, 4);
    circle(P.pos.x + 6, P.pos.y + 6, 4);
    circle(P.pos.x -6, P.pos.y + 6, 4);
    glColor3d(102/255. , 178/255. , 1);
    circle(P.pos.x + 3, P.pos.y + 6, 3.5);
    circle(P.pos.x -4, P.pos.y + 6, 3.5);
    glColor3d(0, 0, 0);
    circle(P.pos.x + 3, P.pos.y + 6, 2);
    circle(P.pos.x -4, P.pos.y +6, 2);
    glColor3d(1, 128/255. , 0);//mouth
    circle(P.pos.x, P.pos.y, 4);
    glColor3d(0, 0, 0);
    circle(P.pos.x + 0, P.pos.y + 0, 2);

}
//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    //glClearColor(150/255., 100/255., 180/255., 0);
    glClearColor(0 / 255., 0 / 255., 255 / 255., 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(128/255. , 255 / 255., 0/255.);
    glRectf(-250, -250, 250, 250);
    
    if (pause)
    {
        glColor3b(1, 0, 0);
        vprint(-125, 0, GLUT_BITMAP_8_BY_13, "PRESS [SPACEBAR] TO PLAY WITH TWEETY :)");
        vprint(-80, -20, GLUT_BITMAP_8_BY_13, "USE MOUSE TO MOVE :)");

    }
    if (!pause) {
        sylvester();
        tweety();
        if (out)
            vprint(-120, -280, GLUT_BITMAP_8_BY_13, "mouse is outside, tweety escaped!");
        else
            vprint(-200, -280, GLUT_BITMAP_8_BY_13, "mouse is inside, sylvester is right behind tweety!");
        if (!gamesitu)
        {
            glColor3d(255 / 255., 0 / 255., 0);
            glRectf(-330, -330, 330, 330);
            glColor3d(0 / 255., 0 / 255., 0);
            vprint(-125, 0, GLUT_BITMAP_8_BY_13, "GAME OVER:( TWEETY COULD NOT ESCAPED:(");
            vprint(-80, -20, GLUT_BITMAP_8_BY_13, "PRESS [R] TO CONTINUE PLAYING!");
        }
    }
    glColor3b(1, 0, 0);
    vprint(-250, 300, GLUT_BITMAP_8_BY_13, "didehan topsakal");
    vprint(-250, 280, GLUT_BITMAP_8_BY_13, "22003293");
    vprint(120, 300, GLUT_BITMAP_8_BY_13, "Press [ESC] to exit game");
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ')
        pause = !pause;
    if (key == 'r') {
        count = 0;
        gamesitu = true;
    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y){
    // Write your codes here.
    P.pos.x = x - WINDOW_WIDTH / 2;
    P.pos.y = WINDOW_HEIGHT / 2 - y;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (!pause) 
    {
       

        glColor3f(0, 0, 0);
        if (P.pos.x >= 250 || P.pos.x <= -250 || P.pos.y >= 250 || P.pos.y <= -250)
            out = true;
        else
            out = false;
        if (!out)
            E.vel = mulV(ENEMY_SPEED, unitV(subV(P.pos, E.pos)));

        E.pos = addV(E.pos, E.vel);

        if (E.pos.x > 250 || E.pos.x < -250)
            E.vel.x = E.vel.x * (-1);
        if (E.pos.y > 250 || E.pos.y < -250)
            E.vel.y = E.vel.y * (-1);

        if ((int)E.pos.x == (int)P.pos.x && (int)E.pos.y == (int)P.pos.y)
        {
            count++;
        }
        if (count >= 1)
            gamesitu = false;
        else
            gamesitu = true;
    }
    
    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("HW3-sylvester chasing tweety");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}