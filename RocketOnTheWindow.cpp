/*********

PROBLEMS: I wasn't planning to add the bottom circle part of the window but I could not manage to delete it because of layering issues.
----------
ADDITIONAL FEATURES:
1- I added a fire under the rocket which grows bigger when rocket goes faster. 
2- If you press the UP key button rocket accelerates and decelerates when the DOWN button being pressed.
3- When you press RIGHT/LEFT key button it switches between day and night. 
4- I worked extra hard for design.
5- When you left click mouse the rocket goes slightly left and right when you press right click button.
6- There's also launch mode if you press 'F1' the rocket launches really fast.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>a
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 900

#define TIMER_PERIOD  20 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

int rock = 0;
int center = 0;
int fire = 0;
int speed = 5;
int cloud = -200;
int cloudd = 200;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

bool activeTimer = false;
bool cloudTimer = true;
bool clouddTimer = true;
bool launch = false;

int mode = 1;



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

void brick() {
    glColor3f(121. / 255, 59. / 255, 49. / 255);
    glRectf(-350, -450, 350, 450);

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    for (int height = 600; height >= -600; height -= 30) {
        glVertex2f(-450, height);
        glVertex2f(450, height);
    }

    for (int width = 450; width >= -450; width -= 60) {
        for (int height = 420; height >= -420; height -= 60) {
            glVertex2f(width, height);
            glVertex2f(width, height - 30);
        }
    }

    for (int width = 430; width >= -430; width -= 60) {
        for (int height = 450; height >= -450; height -= 60) {
            glVertex2f(width, height);
            glVertex2f(width, height - 30);
        }
    }
   
    glEnd();
    glLineWidth(2.0f);
}

void brickbottom() {
    glColor3f(121. / 255, 59. / 255, 49. / 255);
    glRectf(-450, -450, 450, -260);

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);

    for (int height = -210; height >= -600; height -= 30) {
        glVertex2f(-450, height);
        glVertex2f(450, height);
    }

    for (int width = 450; width >= -450; width -= 60) {
        for (int height = -240; height >= -450; height -= 60) {
            glVertex2f(width, height);
            glVertex2f(width, height - 30);
        }
    }

    for (int width = 430; width >= -430; width -= 60) {
        for (int height = -210; height >= -450; height -= 60) {
            glVertex2f(width, height);
            glVertex2f(width, height - 30);
        }
    }

    glEnd();
    glLineWidth(2.0f);
}

void bricktop() {
    glColor3f(121. / 255, 59. / 255, 49. / 255);
    glRectf(-350, 400, 350, 450);

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    for (int height = 600; height >= 400; height -= 30) {
        glVertex2f(-350, height);
        glVertex2f(350, height);
    }

    for (int width = 450; width >= -450; width -= 60) {
        for (int height = 480; height >= 420; height -= 60) {
            glVertex2f(width, height);
            glVertex2f(width, height - 30);
        }
    }

    for (int width = 430; width >= -430; width -= 60) {
        for (int height = 510; height >= 450; height -= 60) {
            glVertex2f(width, height);
            glVertex2f(width, height - 30);
        }
    }

    glEnd();
    glLineWidth(2.0f);
}

void window() {
    

    for (int i = 198; i > 180; i--)
    {
        glColor3f(240. / 255, 240. / 255, 240. / 255);
        circle_wire(0, 200, i);
    }

    
    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(-197, -260, -180, 200);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(197, -260, 180, 200);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(197, -260, -197, -240);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(197, 180, -197, 200);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(-192, -252, -188, 200);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(192, -252, 188, 200);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(192, -252, -192, -248);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(192, 188, -192, 192);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(-10, 200, 10, 380);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(-2, 190, 2, 390);

    for (int i = 191; i > 188; i--)
    {
        glColor3f(180. / 255, 180. / 255, 180. / 255);
        circle_wire(0, 200, i);
    }

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glBegin(GL_QUADS);
    glVertex2f(0, 200);
    glVertex2f(-20, 200);
    glVertex2f(-160, 290);
    glVertex2f(-150, 300);
    glEnd();

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glBegin(GL_QUADS);
    glVertex2f(7, 190);
    glVertex2f(0, 190);
    glVertex2f(-164, 299);
    glVertex2f(-160, 301);
    glEnd();

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glBegin(GL_QUADS);
    glVertex2f(0, 200);
    glVertex2f(20, 200);
    glVertex2f(160, 290);
    glVertex2f(150, 300);
    glEnd();

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glBegin(GL_QUADS);
    glVertex2f(-7, 190);
    glVertex2f(0, 190);
    glVertex2f(164, 299);
    glVertex2f(160, 301);
    glEnd();

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(-203, -260, -220, 200);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(-400, -260, -380, 200);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(-400, 180, -203, 200);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(-400, -260, -203, -240);

    glColor3f(0.2, 0.2, 0.2);
    glRectf(-197, -255, -203, 195);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(-212, -252, -208, 192);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(-392, -252, -388, 192);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(-392, 188, -208, 192);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(-392, -252, -208, -248);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(-380, -145, -220, -125);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(-380, -40, -220, -20);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(-380, 65, -220, 85);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(-392, 73, -208, 77);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(-392, -32, -208, -28);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(-392, -133, -208, -137);






    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(203, -260, 220, 200);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(400, -260, 380, 200);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(400, 180, 203, 200);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(400, -260, 203, -240);

    glColor3f(0.2, 0.2, 0.2);
    glRectf(197, -255, 203, 195);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(212, -252, 208, 192);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(392, -252, 388, 192);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(392, 188, 208, 192);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(392, -252, 208, -248);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(380, -145, 220, -125);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(380, -40, 220, -20);

    glColor3f(240. / 255, 240. / 255, 240. / 255);
    glRectf(380, 65, 220, 85);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(392, 73, 208, 77);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(392, -32, 208, -28);

    glColor3f(180. / 255, 180. / 255, 180. / 255);
    glRectf(392, -133, 208, -137);
}

void cloud1(int var) {
    for (int i = 0; i < 3; i++) {
        glColor3f(0.9, 0.9, 0.9);
        circle(var + 20*i, 150, 20);
    }
    for (int i = 0; i < 4; i++) {
        glColor3f(0.9, 0.9, 0.9);
        circle(var -10 + 20 * i, 135, 20);
    }

}

void cloud2(int var) {

 
    for (int i = 0; i < 3; i++) {
        glColor3f(0.9, 0.9, 0.9);
        circle(var + 20 * i, 90, 20);
    }
    for (int i = 0; i < 4; i++) {
        glColor3f(0.9, 0.9, 0.9);
        circle(var - 10 + 20 * i, 75, 20);
    }

}

void stars() {
    srand(NULL);

    glColor3f(1, 1, 1);
    glPointSize(1);
    glBegin(GL_POINTS);
    for (int i = 0; i < 160; i++) {
        int xstar = rand() % 800 - 380;
        int ystar = rand() % 360 - 180;
        glVertex2f(xstar, ystar);
    }
    glEnd();

    glColor3f(1, 1, 1);
    glPointSize(1);
    glBegin(GL_POINTS);
    for (int i = 0; i < 30; i++) {
        int xstar = rand() % 300 - 150;
        int ystar = rand() % 200 + 180;
        glVertex2f(xstar, ystar);
    }
    glEnd();
    

}

void moon() {

    glColor3f(243. / 255, 246. / 255, 247. / 255);
    circle(300, 130, 40);

}

void background() {
    glColor3f(135. / 255, 206. / 255, 250. / 255);
    glRectf(-380, -240, 380, 180);

    

}

void backgroundtop() {
    glColor3f(135. / 255, 206. / 255, 250. / 255);
    circle(0, 200, 195);



}

void background2() {
    glColor3f(34. / 255, 45. / 255, 90. / 255);
    glRectf(-380, -240, 380, 180);

}

void backgroundtop2() {
    glColor3f(34. / 255, 45. / 255, 90. / 255);
    circle(0, 200, 195);
}

void platform() {
    glColor3f(155. / 255, 118. / 255, 83. / 255);
    glRectf(-380, -240, 380, -200);

    glColor3f(70. / 255, 146. / 255, 8. / 255);
    glRectf(-380, -200, 380, -185);

    glColor3f(50. / 255, 126. / 255, 8. / 255);
    glRectf(-380, -200, 380, -195);

    glColor3f(1. / 255, 80. / 255, 32. / 255);
    glRectf(-30, -185, 30, -130);

    glColor3f(1. / 255, 40. / 255, 10. / 255);
    glRectf(-30, -132, 30, -130);

    glColor3f(1. / 255, 90. / 255, 32. / 255);
    glRectf(-40, -110, 40, -130);
}

void rocketfire() {
    glColor3f(207. / 255, 25. / 255, 35. / 255);
    circle(center + 0, rock - 106, fire + 9);

    glColor3f(251. / 255, 139. / 255, 35. / 255);
    circle(center + 0, rock - 104, fire + 7);

    glColor3f(254. / 255, 222. / 255, 23. / 255);
    circle(center + 0, rock - 102, fire + 5);

}

void rocket() {
   
    

    glColor3f(0.6, 0, 0);
    circle(center + 0, rock + 0, 30);

    glColor3f(86. /255, 95. /255, 107. / 255);
    glRectf(center + -30, rock + 0, center + 30, rock + -90);

    glColor3f(0.5, 0, 0);
    circle(center + 0, rock + -30, 15);

    glColor3f(0, 0, 0);
    circle(center + 0, rock + -30, 10);

    glColor3f(0.6, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(center + -30, rock + -50);
    glVertex2f(center + -30, rock + -80);
    glVertex2f(center + -50, rock + -85);
    glVertex2f(center + -50, rock + -70);
    glEnd();

    glColor3f(0.6, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(center + 30, rock + -50);
    glVertex2f(center + 30, rock + -80);
    glVertex2f(center + 50, rock + -85);
    glVertex2f(center + 50, rock + -70);
    glEnd();

    glColor3f(0.6, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(center + 20, rock + -90);
    glVertex2f(center + -20, rock + -90);
    glVertex2f(center + -30, rock + -100);
    glVertex2f(center + 30, rock + -100);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(center + -10, rock + -80);
    glVertex2f(center + -6, rock + -80);
    glVertex2f(center + 10, rock + -60);
    glVertex2f(center + 6, rock + -60);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(center + 14, rock + -80);
    glVertex2f(center + 4, rock + -80);
    glVertex2f(center + -14, rock + -60);
    glVertex2f(center + -4, rock + -60);
    glEnd();

    glColor3f(86. / 255, 95. / 255, 107. / 255);
    glBegin(GL_QUADS);
    glVertex2f(center + 8, rock + -78);
    glVertex2f(center + 5, rock + -78);
    glVertex2f(center + -8, rock + -62);
    glVertex2f(center + -5, rock + -62);
    glEnd();

    glColor3f(0.6, 0, 0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(center + 28, rock + 10);
    glVertex2f(center + -28, rock + 10);
    glVertex2f(center + 0, rock + 40);
    glEnd();

    

}

void label() {

    glColor3f(70. / 255, 146. / 255, 8. / 255);
    glBegin(GL_POLYGON);
    glVertex2f(200, -300);
    glVertex2f(200, -400);
    glVertex2f(-200, -400);
    glVertex2f(-150, -350);
    glVertex2f(-200, -300);
    glEnd();

    glColor3f(155. / 255, 118. / 255, 83. / 255);
    glBegin(GL_POLYGON);
    glVertex2f(190, -310);
    glVertex2f(190, -390);
    glVertex2f(-180, -390);
    glVertex2f(-140, -350);
    glVertex2f(-180, -310);
    glEnd();


    glColor3f(0, 0, 0);
    vprint(-110, -355, GLUT_BITMAP_HELVETICA_18, "  Rocket Launch by Orhan Koc");

    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-130, -280 + -80);
    glVertex2f(-126, -280 + -80);
    glVertex2f(-110, -280 + -60);
    glVertex2f(-114, -280 + -60);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-106, -280 + -80);
    glVertex2f(-116, -280 + -80);
    glVertex2f(-134, -280 + -60);
    glVertex2f(-124, -280 + -60);
    glEnd();

    glColor3f(155. / 255, 118. / 255, 83. / 255);
    glBegin(GL_QUADS);
    glVertex2f(-112, -280 + -78);
    glVertex2f(-115, -280 + -78);
    glVertex2f(-128, -280 + -62);
    glVertex2f(-125, -280 + -62);
    glEnd();


}

void display() {

    glClearColor(121. / 255, 59. / 255, 49. / 255, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    brick();
    switch (mode)
    {
    case 1: background();
        backgroundtop();
        background();
        cloud1(cloud);
        cloud2(cloudd);
        break;
    case 2: background2();
        backgroundtop2();
        background2();
        stars();
        moon();
        break;
    default:
        break;
    }

    if (activeTimer)
        rocketfire();

    rocket();
    bricktop();
    brickbottom();
    platform();
    window();
    label();

    
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
        activeTimer = !activeTimer;

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
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    }

    if (key == GLUT_KEY_LEFT) {
        if (center > -50)
            center -= 2;
    }

    if (key == GLUT_KEY_RIGHT) {
        if (center < 50)
            center += 2;
    }

    if (key == GLUT_KEY_UP) {
        if (!launch) {
            if (fire < 10) {
                speed++;
                fire += 1;
            }
        }
    }

    if (key == GLUT_KEY_DOWN) {
        if (!launch) {
            if (fire > 0) {
                speed--;
                fire -= 1;
            }
        }
    }

    if (key == GLUT_KEY_F1) {
        if (!launch)
            launch = true;
        else
        {
            launch = false;
        }
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
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
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
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
    if (activeTimer) {

        if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
            if (mode != 1)
                mode--;
            else
                mode = 2;
            
        }

        if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN) {
            if (mode != 2)
                mode++;
            else
                mode = 1;
        }
    }

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
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (activeTimer) {
        if (!launch) {
            if (rock < 480) {
                rock += speed;
            }
            else {
                rock = -500;
            }
        }
        
        if (launch) {
            speed = 30;
            fire = 15;

            if (rock < 480) {
                rock += speed;
            }
            else {
                rock = -500;
                launch = false;
                speed = 10;
                fire = 5;
            }
                
        }


        if (cloud == 330)
            cloudTimer = false;
        else if (cloud == -330)
            cloudTimer = true;

        if (cloudTimer) {
            cloud++;;
        }
        else {
            cloud--;
        }

        if (cloudd == 330)
            clouddTimer = false;
        else if (cloudd == -330)
            clouddTimer = true;

        if (clouddTimer) {
            cloudd++;;
        }
        else {
            cloudd--;
        }
    }







    // to refresh the window it calls display() function
    glutPostRedisplay();

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
    glutCreateWindow("X Rocket Launch - Orhan Kemal Koç");

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