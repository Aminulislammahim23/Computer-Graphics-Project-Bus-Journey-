#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <cstdlib>
#include <ctime>
using namespace std;


///NAME : AMINUL ISLAM MAHIM
///ID : 22-46183-1
///PART 1

int currentScreen = 1;
float position = 0.0f;
float bus1 = 0.0f;
float bus2 = 0.0f;
float cloud = 0.0f;
float move33 = 0.0f;
//float Busspeed = 8.1f;
float speed11 = 0.0f;
float rotationAngle = 0.0f;
bool isBus1Moving = false;
bool isBus2Moving = false;
bool isPlaneMoving = true;
float rainDropY[100];
float rainDropX[100];
bool isRotating = true;
bool isRaining = true;
bool isDay = false;
bool ledOn = false;

// Function to initialize all screens
void initScreen() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 800); // Unified viewport for all screens
}

void drawCircle(float centerX, float centerY, float radius, float rColor, float gColor, float bColor) {
    glColor3f(rColor, gColor, bColor);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float pi = 3.1416;
        float angle = (i * 2 * pi) / 180;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(centerX + x, centerY + y);
    }
    glEnd();
}

void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

/// WHEELS ///
void drawWheel(float x, float y) {
        glPushMatrix();
        glTranslatef(x, y, 0);
        glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);

        // Outer tire
        drawCircle(0, 0, 35, 0.0, 0.0, 0.0);

        // Inner rim
        drawCircle(0, 0, 25, 0.7, 0.7, 0.7);

        // Spokes
        float angleStep = 360.0f / 12;
        glColor3f(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < 12; ++i) {
            float angle = i * angleStep * (3.1416 / 180.0f);
            float xEnd = 35 * cos(angle);
            float yEnd = 35 * sin(angle);

            glBegin(GL_LINES);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(xEnd, yEnd);
            glEnd();
        }

        // Hub
        drawCircle(0, 0, 15, 0.0, 0.0, 0.0);
        glPopMatrix();
    };


void wheelPower(float X) {
    if (!isRotating) return; // If not rotating, do nothing

    rotationAngle -= X;
    if (rotationAngle <= -360.0f)
        rotationAngle += 360.0f;
}

void toggleLED(int value) {
    if (currentScreen == 1) {  // Only blink LED when on screen 1
        ledOn = !ledOn;
        glutPostRedisplay();
        glutTimerFunc(2000, toggleLED, 0);
    }
}


////////////////////////////////////////// UPDATE FUNCTIONS FOR DISPLAY 1/////////////////////////////////////////////////////////////////////////////
void Bus1Move(int value) {
    if (isBus1Moving) {
        if (speed11 < 0.1f || speed11 > 10.0f) {
            speed11 = 5.0f;}
        bus1 += speed11;
        wheelPower(speed11);
        if (bus1 >= 2500) {
            if (currentScreen < 5) {
                currentScreen++;
                bus1 = -200;
            } else {
                isBus1Moving = false;
                bus1 = 1200;}
        }
        glutPostRedisplay();
    }
    glutTimerFunc(20, Bus1Move, 0);
}

void drawRain() {
    glColor3f(0.6, 0.6, 1.0);  // Light blue raindrops
    for (int i = 0; i < 100; i++) {
        glBegin(GL_LINES);
        glVertex2f(rainDropX[i], rainDropY[i]);
        glVertex2f(rainDropX[i], rainDropY[i] - 10);
        glEnd();
    }
}

void updateRain() {
    for (int i = 0; i < 100; i++) {
        rainDropY[i] -= 5;  // Move drops down
        if (rainDropY[i] < 0) {
            rainDropY[i] = 800;  // Reset to top
            rainDropX[i] = rand() % 1200;  // Random X position
        }
    }
}

void MovingPart(int value) {
    cloud += 2.0f;
    if (cloud > 1200)
        cloud = -250;

    if (isRaining)
        updateRain();
    glutPostRedisplay();
    glutTimerFunc(20, MovingPart, 0);
}

void PlaneMove(int value)
{
    if (isPlaneMoving) {
        move33 -= 10.8;
        if (move33 < -1200) {
            move33 = 1200;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(20, PlaneMove, 0);  // Correct function pointer usage
}




void Bus2Move(int value)
{
    if (isBus2Moving) {
        bus2 += 4.0f;
        if (bus2 > 800)
            bus2 = 800;
        wheelPower(4.0f);
        if(bus2 >= 800)
           isRotating = false;
    }
    glutPostRedisplay();
    glutTimerFunc(20, Bus2Move, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawSky()
{
    if (isDay) {
        glBegin(GL_QUADS);
        glColor3ub(102,178,255); // Daytime sky
        glVertex2f(0, 500);
        glVertex2f(1200, 500);
        glColor3ub(0,128,255);
        glVertex2f(1200, 800);
        glVertex2f(0, 800);
        glEnd();
        // Draw sun
        drawCircle(1000, 700, 50, 1.0, 1.0, 0.0);
    } else {
        glBegin(GL_QUADS);
        glColor3ub(0,0,102); //Nighttime sky
        glVertex2f(0, 500);
        glVertex2f(1200, 500);
        glColor3ub(0,0,51);
        glVertex2f(1200, 800);
        glVertex2f(0, 800);
        glEnd();
        // Draw moon
        drawCircle(1000, 700, 35, 1.0, 1.0, 1.0);
    }
}

void drawCloud(){
    glPushMatrix();
    glTranslatef(cloud,0.0,0.0);
    //cloud 1
    drawCircle(30, 700, 30,1.0,1.0,1.0);
    drawCircle(70, 700, 30,1.0,1.0,1.0);
    drawCircle(110, 700, 30,1.0,1.0,1.0);
    drawCircle(70, 720, 30,1.0,1.0,1.0);

    //cloud 2
    drawCircle(200, 750, 30,1.0,1.0,1.0);
    drawCircle(240, 750, 30,1.0,1.0,1.0);
    drawCircle(280, 750, 30,1.0,1.0,1.0);
    drawCircle(240, 770, 30,1.0,1.0,1.0);

    glPopMatrix();
}

void drawMiddle()
{
    if (isDay) {
        glColor3ub(0,204,0);// Darker grass for night
        glBegin(GL_QUADS);
        glVertex2f(0,300);
        glVertex2f(1200,300);
        glColor3ub(0,255,0);
        glVertex2f(1200,500);
        glVertex2f(0,500);

    } else {
        glColor3ub(0,255,0); // Green grass for day
        glBegin(GL_QUADS);
        glVertex2f(0,300);
        glVertex2f(1200,300);
        glColor3ub(0,102,0);
        glVertex2f(1200,500);
        glVertex2f(0,500);
    }

    glEnd();
}

void drawRoad()
{
    ///layer 1
    if(isDay){
        glColor3ub(32,32,32);
        glBegin(GL_QUADS);
        glVertex2f(0,160);
        glVertex2f(1200,160);
        glColor3ub(32,32,32);
        glVertex2f(1200,300);
        glVertex2f(0,300);
    glEnd();
    }
    else{
       glColor3ub(96,96,96);
    glBegin(GL_QUADS);
        glVertex2f(0,160);
        glVertex2f(1200,160);
        glColor3ub(70,70,70);
        glVertex2f(1200,300);
        glVertex2f(0,300);
    glEnd();
    }

    ///layer 2
    if(isDay)
        glColor3ub(32,32,32);
    else
        glColor3ub(128,128,128);
        glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(600,0);
        glColor3ub(32,32,32);
        glVertex2f(900,500);
        glVertex2f(0,500);
    glEnd();


    ///Road Line
    glPushMatrix();
    for (int i = 0; i <= 120; i += 05){
    glTranslatef(i,0.0,0.0);
    glBegin(GL_QUADS);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(0,230);
        glVertex2f(40,230);
        glVertex2f(40,234);
        glVertex2f(0,234);
    glEnd();
    }
    glPopMatrix();

    ///2
    glBegin(GL_QUADS);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(0,230+100);
        glVertex2f(200,230+100);
        glVertex2f(220,234+130);
        glVertex2f(0,234+130);
    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(0,230+110);
        glVertex2f(200,230+110);
        glVertex2f(210,234+120);
        glVertex2f(0,234+120);
    glEnd();

    ///3
    glBegin(GL_QUADS);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(0,230+200);
        glVertex2f(300,230+200);
        glVertex2f(320,234+230);
        glVertex2f(0,234+230);
    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(0,230+210);
        glVertex2f(300,230+210);
        glVertex2f(310,234+220);
        glVertex2f(0,234+220);
    glEnd();
}

void human()
{
    // Head
    drawCircle(700,460,15,1.0,1.0,1.0);

    // Body
    glBegin(GL_QUADS);
    glVertex2f(690, 450);
    glVertex2f(710,450);
    glVertex2f(710,430);
    glVertex2f(690,430);
    // Arms
    glBegin(GL_QUADS);
    glVertex2f(680, 450);
    glVertex2f(720,450);
    glVertex2f(720,440);
    glVertex2f(680,440);
    // Legs
    glBegin(GL_QUADS);
    glVertex2f(700, 420);
    glVertex2f(705,420);
    glVertex2f(705,440);
    glVertex2f(695,440);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(705, 420);
    glVertex2f(705,420);
    glVertex2f(700,440);
    glVertex2f(695,440);
    glEnd();
}

void drawGround()
{
    glBegin(GL_QUADS);
    if (isDay) {
        glColor3ub(0,204,0);
        glVertex2f(0,0);
        glVertex2f(1200,0);
        glColor3ub(0,202,0);
        glVertex2f(1200,200);
        glVertex2f(0,200);
        glEnd();
    } else {
        glColor3ub(0,102,0);
        glVertex2f(0,0);
        glVertex2f(1200,0);
        glColor3ub(0,255,0);
        glVertex2f(1200,200);
        glVertex2f(0,200);
        glEnd();
    }
    //Garden
    glBegin(GL_QUADS);
        glColor3f(0.255, 0.242,0.204);
        glVertex2f(650,30);
        glVertex2f(1200,30);
        glVertex2f(1200,130);
        glVertex2f(700,130);
    glEnd();

    ///Chair

    glPushMatrix();
    for(int i=0; i <= 200; i += 50){
    glPushMatrix();
    glTranslatef(i,0.0,0.0);
    //Right leg
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(900,80);
        glVertex2f(900,60);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(890,70);
        glVertex2f(900,60);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(910,95);
        glVertex2f(900,60);
    glEnd();
    //Left leg
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(800,80);
        glVertex2f(800,60);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(790,70);
        glVertex2f(800,60);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(815,95);
        glVertex2f(800,60);
    glEnd();
    ///Base
    glBegin(GL_QUADS);
        glColor3ub(255,242,204);
        glVertex2f(790,70);
        glVertex2f(890,70);
        glVertex2f(910,95);
        glVertex2f(815,95);
    glEnd();
    //middle
    glBegin(GL_QUADS);
        glColor3ub(153,76,0);
        glVertex2f(800,80);
        glVertex2f(900,80);
        glVertex2f(900,110);
        glVertex2f(800,110);
    glEnd();
    glPopMatrix();
}
glPopMatrix();
}

void drawCityLights(int x) {


    // Draw bases of light poles
    glColor3f(0.0, 0.0, 0.0); // Black color for poles
    for (int i = 700; i < 1200; i += x) {
        glBegin(GL_QUADS);
        glVertex2f(i, 160);
        glVertex2f(i + 10, 160);
        glVertex2f(i + 10, 290);
        glVertex2f(i, 290);
        glEnd();
    }

    // Draw upper parts of poles
    glColor3f(0.0, 0.0, 0.0); // Black color for upper part
    for (int i = 700; i < 1200; i += x) {
        glBegin(GL_QUADS);
        glVertex2f(i, 290);
        glVertex2f(i + 10, 290);
        glVertex2f(i + 100, 330);
        glVertex2f(i + 90, 330);
        glEnd();
    }

    //lights
    if(isDay)
    glColor3f(1.0, 1.0, 1.0);
    else
    glColor3f(1.0,1.0,0.0);// Yellow city light
    for(int i = 780; i < 1200; i += x){
    glBegin(GL_QUADS);
    glVertex2f(i, 320);
    glVertex2f(i+30, 320);
    glVertex2f(i+30, 330);
    glVertex2f(i+10, 330);
    glEnd();

    int cl;
    if(isDay){
        glBegin(GL_LINES);
    glColor3ub(96,96,96);
    glVertex2f(710,280);
    glVertex2f(1200,280);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(96,96,96);
    glVertex2f(710,270);
    glVertex2f(1200,270);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(96,96,96);
    glVertex2f(710,260);
    glVertex2f(1200,260);
    glEnd();
    }
    else{
       glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex2f(710,280);
    glVertex2f(1200,280);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex2f(710,270);
    glVertex2f(1200,270);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex2f(710,260);
    glVertex2f(1200,260);
    glEnd();
    }


    }
}


void drawBus1() {
    glPushMatrix();
    glScalef(0.5f, 0.5f, 1.0f);
    glTranslatef(bus1, 0.0f, 0.0f);

    /// BUS BODY ///
    //Up
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(30, 290+90);
    glVertex2f(700, 290+90);
    glColor3ub(102,0,0);
    glVertex2f(680, 490+90);
    glVertex2f(40, 490+90);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("GREEN LINE",350,340+100);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("HINO RM2",50,340+100);

    //Down
    glBegin(GL_QUADS);
    glColor3ub(64,64,64);
    glVertex2f(30, 290+90);
    glVertex2f(700, 290+90);
    glVertex2f(700, 340+90);
    glVertex2f(30, 340+90);
    glEnd();

    //headlight
    glBegin(GL_QUADS);
    if(isDay){glColor3ub(255,255,255);}
    else{glColor3ub(255,255,0);}
    glVertex2f(690, 320+90);
    glVertex2f(700, 320+90);
    glVertex2f(700, 340+90);
    glVertex2f(690, 340+90);
    glEnd();

    if(!isDay){
    glBegin(GL_TRIANGLES);
    if(isDay){glColor3ub(255,255,255);}
    else{glColor3ub(255,255,0);}
    glVertex2f(700, 320+90);
    glVertex2f(780, 300+90);
    glVertex2f(780, 340+90);
    glEnd();}
    else{}



    ///LED
    glPushMatrix();

    for(int i=40;i<=600;i+=200){
    glPushMatrix();
    glTranslatef(i,0.0,0.0);
    glBegin(GL_QUADS);

    if (ledOn){
        glColor3f(255,255,0);} //(ON)
    else{
        glColor3f(0.0, 0.0, 0.0); // Black (OFF)
    }
    glVertex2f(50, 330+90);
    glVertex2f(65, 330+90);
    glVertex2f(65, 340+90);
    glVertex2f(50, 340+90);
    glEnd();
    ///upper

    glPopMatrix();
    }
    glPopMatrix();

    //looking glass
    glBegin(GL_QUADS);
    glColor3ub(153,0,0);
    glVertex2f(680, 480+90);
    glVertex2f(720, 480+90);
    glVertex2f(720, 490+90);
    glVertex2f(680, 490+90);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(102,0,0);
    glVertex2f(720, 450+90);
    glVertex2f(740, 450+90);
    glVertex2f(730, 490+90);
    glVertex2f(710, 490+90);
    glEnd();

    /// ROOF TOP ///
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(40, 490+90);
    glVertex2f(320, 490+90);
    glVertex2f(300, 520+90);
    glVertex2f(40, 520+90);
    glEnd();

    /// WINDOWS ///

    for (int i = 0; i <= 500; i += 100) {
        glPushMatrix();
        glTranslatef(i, 0, 0);
        glBegin(GL_QUADS);

        if(isDay)
        glColor3f(0.529, 0.808, 0.922);
        else
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(45, 400+90);
        glVertex2f(135, 400+90);
        glVertex2f(135, 480+90);
        glVertex2f(45, 480+90);
        glEnd();
        glPopMatrix();
    }

    /// LG Boxes ///
    for (int i = 280; i <= 500; i += 80) {
        glPushMatrix();
        glTranslatef(i, 0, 0);
        glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(45, 300+80);
        glVertex2f(115, 300+80);
        glVertex2f(115, 340+80);
        glVertex2f(45, 340+80);
        glEnd();

        for(int j=0;j<=10;j+=5){
        glTranslatef(0.0,j,0.0);
        glBegin(GL_LINES);
        glColor3ub(255,255,255);
        glVertex2f(45,400-15);
        glVertex2f(115,400-15);
        glEnd();
        }
        glPopMatrix();
    }

    /// DOOR ///
    glBegin(GL_QUADS);
    if(isDay)
        glColor3f(0.180, 0.200, 0.220);
        else
        glColor3f(0.102, 0.178, 0.255);
    glVertex2f(620, 290+90);
    glVertex2f(690, 290+90);
    glVertex2f(670, 480+90);
    glVertex2f(620, 480+90);
    glEnd();

    //Wheels
    drawWheel(600, 290+90);
    drawWheel(250, 290+90);
    drawWheel(170, 290+90);

    glPopMatrix();
}


void drawBus2() {
    glPushMatrix();
    glScalef(0.5f, 0.5f, 1.0f);
    glTranslatef(bus2, 0.0f, 0.0f);

    /// BUS BODY ///
    //Up
    glBegin(GL_QUADS);
    glColor3ub(102,0,51);
    glVertex2f(30, 290+500);
    glVertex2f(700, 290+500);
    glColor3ub(204,0,102);
    glVertex2f(680, 490+500);
    glVertex2f(40, 490+500);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("ENA",540,340+510);
    glColor3f(1.0f, 1.0f, 0.0f);
    drawText("VOLVO B11R",40,340+510);

    //Down
    glBegin(GL_QUADS);
    glColor3ub(51,0,0);
    glVertex2f(30, 290+500);
    glVertex2f(700, 290+500);
    glVertex2f(700, 340+500);
    glVertex2f(30, 340+500);
    glEnd();
    ///LED
    glPushMatrix();

    for(int i=0;i<=400;i+=100){
    glPushMatrix();
    glTranslatef(i,0.0,0.0);
    glBegin(GL_QUADS);

    if (ledOn){
        glColor3f(255,255,0);} //(ON)
    else{
        glColor3f(0.0, 0.0, 0.0); // Black (OFF)
    }
    glVertex2f(50, 330+500);
    glVertex2f(65, 330+500);
    glVertex2f(65, 340+500);
    glVertex2f(50, 340+500);
    glEnd();
    glPopMatrix();
    }
    glPopMatrix();

    //looking glass
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(680, 480+500);
    glVertex2f(720, 480+500);
    glVertex2f(720, 490+500);
    glVertex2f(680, 490+500);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(720, 450+500);
    glVertex2f(740, 450+500);
    glVertex2f(730, 490+500);
    glVertex2f(710, 490+500);
    glEnd();

    /// ROOF TOP ///
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(40, 490+500);
    glVertex2f(320, 490+500);
    glVertex2f(300, 520+500);
    glVertex2f(40, 520+500);
    glEnd();

    /// WINDOWS ///
    for (int i = 0; i <= 500; i += 100) {
        glPushMatrix();
        glTranslatef(i, 0, 0);
        glBegin(GL_QUADS);

        if(isDay)
        glColor3f(0.529, 0.808, 0.922);
        else
        glColor3f(0.102, 0.178, 0.255);

        glVertex2f(45, 400+500);
        glVertex2f(135, 400+500);
        glVertex2f(135, 480+500);
        glVertex2f(45, 480+500);
        glEnd();
        glPopMatrix();
    }

    /// LG Boxes ///
    for (int i = 280; i <= 500; i += 80) {
        glPushMatrix();
        glTranslatef(i, 0, 0);
        glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(45, 300+500);
        glVertex2f(115, 300+500);
        glVertex2f(115, 340+500);
        glVertex2f(45, 340+500);
        glEnd();
        glPopMatrix();
    }

    /// DOOR ///
    glBegin(GL_QUADS);
    if(isDay)
        glColor3f(0.180, 0.200, 0.220);
        else
        glColor3f(0.102, 0.178, 0.255);
    glVertex2f(620, 290+500);
    glVertex2f(690, 290+500);
    glVertex2f(670, 480+500);
    glVertex2f(620, 480+500);
    glEnd();

    //Wheels
    drawWheel(600, 290+500);
    drawWheel(250, 290+500);
    drawWheel(170, 290+500);

    glPopMatrix();
}


void drawHomeUp()
{
    ///Pink Home
    if(isDay){
        glColor3ub(255,0,255);
         glBegin(GL_QUADS);
        glVertex2f(1100,500);
        glVertex2f(1176,500);
        glVertex2f(1176,650);
        glVertex2f(1100,650);
    glEnd();
    }
    else{
        glColor3ub(255,0,255);
    glBegin(GL_QUADS);
        glVertex2f(1100,500);
        glVertex2f(1176,500);
        glColor3ub(102,0,0);
        glVertex2f(1176,650);
        glVertex2f(1100,650);
    glEnd();
    }
    //Windows
    glPushMatrix();
    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(1110,510);
        glVertex2f(1130,510);
        glVertex2f(1130,540);
        glVertex2f(1110,540);
    glEnd();
    glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(1145,510);
        glVertex2f(1165,510);
        glVertex2f(1165,540);
        glVertex2f(1145,540);
    glEnd();
    glPopMatrix();
    }
    glPopMatrix();

    ///Yellow Home
    glBegin(GL_QUADS);
        glColor3ub(255,255,153);
        glVertex2f(1000,500);
        glVertex2f(1100,500);
        if(isDay){glColor3ub(255,255,0);}
        else{glColor3ub(153,153,0);}
        glVertex2f(1100,600);
        glVertex2f(1000,600);
    glEnd();

    //windows
    for (int i = 0; i <=50; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(1010,510);
        glVertex2f(1030,510);
        glVertex2f(1030,540);
        glVertex2f(1010,540);
    glEnd();
    glPopMatrix();
    }
    for (int i = 0; i <=50; i +=50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(1040,510);
        glVertex2f(1060,510);
        glVertex2f(1060,540);
        glVertex2f(1040,540);
    glEnd();
    glPopMatrix();
    }
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(1070,500);
        glVertex2f(1097,500);
        glVertex2f(1097,600);
        glVertex2f(1070,600);
    glEnd();

    ///Glass Home
    glBegin(GL_QUADS);
    if(isDay){glColor3f(1.0, 0.156, 0.0);}
    else{glColor3ub(102,0,0);}
        glVertex2f(900,500);
        glVertex2f(1000,500);
        glVertex2f(1000,640);
        glVertex2f(900,640);
    glEnd();
    //windows
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(902,500);
        glVertex2f(998,500);
        glVertex2f(998,625);
        glVertex2f(902,625);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(948,500);
        glVertex2f(948,625);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(924,500);
        glVertex2f(924,625);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(972,500);
        glVertex2f(972,625);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(902,562.5f);
        glVertex2f(998,562.5f);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
        glVertex2f(902,595.25f);
        glVertex2f(998,595.25f);
    glEnd();
}


void drawPlane()
{
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glTranslatef(0,300,0);
    glScalef(.6,.6,1);
    {

	glTranslatef(move33,0.0f, 0.0f);
     glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);  //base
    glVertex2f(1100-30, 700);
    glVertex2f(1200, 700);
    glVertex2f(1200, 750);
    glVertex2f(1100-30, 750);
    glEnd();

     glBegin(GL_QUADS);
    glColor3f(0., 0., .5);  // front
    glVertex2f(1070-30, 710);
    glVertex2f(1100-30, 700);
    glVertex2f(1100-30, 725);
    glVertex2f(1070-30, 725);
    glEnd();

     glBegin(GL_QUADS);
    glColor3f(0.4, 0., .4);  // front
    glVertex2f(1070-30, 725);
     glVertex2f(1100-30, 725);
      glVertex2f(1100-30, 750);
    glVertex2f(1090-30, 750);
    glEnd();

         glBegin(GL_QUADS);
    glColor3f(1., 1., 0.);  // wings
    glVertex2f(1150-30, 680);
     glVertex2f(1190-30, 680);
      glVertex2f(1180-30, 720);
    glVertex2f(1140-30, 720);
    glEnd();

       glBegin(GL_QUADS);
    glColor3f(1., 1., 0.);  // wings
    glVertex2f(1130-30, 750);
     glVertex2f(1170-30, 750);
      glVertex2f(1180-30, 790);
    glVertex2f(1145-30, 790);
    glEnd();

           glBegin(GL_QUADS);
    glColor3f(1., 1., 0.);  // back of plane
    glVertex2f(1180, 750);
     glVertex2f(1200, 750);
      glVertex2f(1200, 790);
    glVertex2f(1190, 790);
    glEnd();
        }

    glPopMatrix();
}

void drawBG_home(){

///backgound homes

    //1
    glBegin(GL_QUADS);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(1000,500);
        glVertex2f(1100,500);
        glVertex2f(1100,650);
        glVertex2f(1000,650);
    glEnd();
    ///win
    //left
    glPushMatrix();
    for(int i = 0; i <= 80; i += 20) {
    for(int j = 0; j <= 120; j += 20) {
        glPushMatrix();
        glTranslatef(i, j, 0.0);
        glBegin(GL_QUADS);
        if(isDay){glColor3ub(153, 204,255);}
        else{glColor3ub(255,255,102);}
        glVertex2f(1010,505);
        glVertex2f(1020,505);
        glVertex2f(1020,520);
        glVertex2f(1010,520);
        glEnd();

        glPopMatrix();}}
        glPopMatrix();

    //2
    glBegin(GL_QUADS);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(1100,500);
        glVertex2f(1200,500);
        glVertex2f(1200,750);
        glVertex2f(1100,750);
    glEnd();
    ///win
    //left
    glPushMatrix();
    for(int i = 0; i <= 80; i += 20) {
    for(int j = 0; j <= 220; j += 20) {
        glPushMatrix();
        glTranslatef(i, j, 0.0);
        glBegin(GL_QUADS);
        if(isDay){glColor3ub(153, 204,255);}
        else{glColor3ub(255,255,102);}
        glVertex2f(1110,505);
        glVertex2f(1120,505);
        glVertex2f(1120,515);
        glVertex2f(1110,515);
        glEnd();

        glPopMatrix();}}
        glPopMatrix();

    //3
    glBegin(GL_QUADS);
    if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(800+20,500);
        glVertex2f(900+30,500);
        glVertex2f(900+30,750);
        glVertex2f(800+20,750);
    glEnd();
    ///win
    //left
    glPushMatrix();
    for(int i = 0; i <= 80; i += 20) {
    for(int j = 0; j <= 220; j += 20) {
        glPushMatrix();
        glTranslatef(i, j, 0.0);

        glBegin(GL_QUADS);
        if(isDay){glColor3ub(153, 204,255);}
        else{glColor3ub(255,255,102);}
        glVertex2f(830,505);
        glVertex2f(840,505);
        glVertex2f(840,520);
        glVertex2f(830,520);
        glEnd();

        glPopMatrix();
    }
}
glPopMatrix();

    //4
    glBegin(GL_QUADS);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(710,500);
        glVertex2f(800,500);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(800,680);
        glVertex2f(710,680);
    glEnd();

    glPushMatrix();
    for(int i=0;i <=160;i += 10){
        glPushMatrix();
        glTranslatef(0.0,i,0.0);
        glBegin(GL_LINES);
        glColor3ub(153,204,255);
        glVertex2f(710,510);
        glVertex2f(800,510);
    glEnd();
    glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    for(int i=0;i <=60;i += 20){
        glPushMatrix();
        glTranslatef(i,0.0,0.0);
        glBegin(GL_LINES);
        glColor3ub(153,204,255);
        glVertex2f(730,670);
        glVertex2f(730,500);
    glEnd();
    glPopMatrix();
    }
    glPopMatrix();

    //5
    glBegin(GL_QUADS);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(600,500);
        glVertex2f(700,500);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(700,750);
        glVertex2f(600,750);
    glEnd();
    ///win
    //left
    glPushMatrix();
    for(int i = 0; i <= 75; i += 15) {
    for(int j = 0; j <= 220; j += 20) {
        glPushMatrix();
        glTranslatef(i, j, 0.0);
        glBegin(GL_QUADS);
        if(isDay){glColor3ub(153, 204,255);}
        else{glColor3ub(255,255,102);}
        glVertex2f(610,505);
        glVertex2f(620,505);
        glVertex2f(620,520);
        glVertex2f(610,520);
        glEnd();

        glPopMatrix();
    }
}
glPopMatrix();

    //6
    glBegin(GL_QUADS);
        glColor3ub(153,204,255);
        glVertex2f(450,500);
        glVertex2f(600,500);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(600,700);
        glVertex2f(450,700);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(153,204,255);
        glVertex2f(500,500);
        glVertex2f(550,500);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(550,760);
        glVertex2f(500,760);
    glEnd();

    glBegin(GL_TRIANGLES);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(450,700);
        glVertex2f(600,700);
        glVertex2f(525,740);
    glEnd();

    //7
    glBegin(GL_QUADS);
        if(isDay){glColor3ub(153,204,255);}
        else{glColor3ub(32,32,32);}
        glVertex2f(300,500);
        glVertex2f(370,500);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(370,740);
        glVertex2f(300,680);
    glEnd();


    glBegin(GL_QUADS);
        if(isDay){glColor3ub(153,204,255);}
        else{glColor3ub(32,32,32);}
        glVertex2f(200,500);
        glVertex2f(270,500);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(270,680);
        glVertex2f(200,740);
    glEnd();

    glBegin(GL_QUADS);
        if(isDay){glColor3ub(153,204,255);}
        else{glColor3ub(0,51,102);}
        glVertex2f(200,700);
        glVertex2f(370,700);
        if(isDay){glColor3ub(153,204,255);}
        else{glColor3ub(0,25,51);}
        glVertex2f(370,720);
        glVertex2f(200,720);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(255,255,255);
    glVertex2f(270,680);
    glVertex2f(300,720);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(255,255,255);
    glVertex2f(300,680);
    glVertex2f(270,720);
    glEnd();

    //8
    glBegin(GL_QUADS);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(10,500);
        glVertex2f(170,500);
        glVertex2f(170,670);
        glVertex2f(10,670);
    glEnd();
    glBegin(GL_QUADS);
        if(isDay){glColor3ub(0,102,204);}
        else{glColor3ub(32,32,32);}
        glVertex2f(50,500);
        glVertex2f(130,500);
        glVertex2f(130,780);
        glVertex2f(50,780);
    glEnd();
    ///win
    //Left
    glPushMatrix();
    for(int i =0; i <=140; i += 20){
            for(int j =0;j<=120;j+=20){
        glPushMatrix();
        glTranslatef(j,i,0.0);
        glBegin(GL_QUADS);
        if(isDay){glColor3ub(153, 204,255);}
        else{glColor3ub(255,255,102);}
        glVertex2f(20,505);
        glVertex2f(30,505);
        glVertex2f(30,520);
        glVertex2f(20,520);
    glEnd();
    glPopMatrix();
    }}
    glPopMatrix();
    //up
    glPushMatrix();
    for(int i =0; i <=80; i += 20){
            for(int j =0;j<=60;j+=20){
        glPushMatrix();
        glTranslatef(j,i,0.0);
        glBegin(GL_QUADS);
        if(isDay){glColor3ub(153, 204,255);}
        else{glColor3ub(255,255,102);}
        glVertex2f(55,670);
        glVertex2f(65,670);
        glVertex2f(65,685);
        glVertex2f(55,685);
    glEnd();
    glPopMatrix();
    }}
    glPopMatrix();
}


void drawBStop()
{
    glBegin(GL_QUADS);
        glColor3ub(255,0,0);  //base
        glVertex2f(1000,320);
        glVertex2f(1150,320);
        glVertex2f(1150,350+30);
        glVertex2f(1000,350+30);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(160,160,160); //shade
        glVertex2f(990,340+30);
        glVertex2f(1140,340+30);
        glVertex2f(1150,350+30);
        glVertex2f(1000,350+30);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(32,32,32);  //groud
        glVertex2f(980,340-50);
        glVertex2f(1140,340-50);
        glVertex2f(1150,350-30);
        glVertex2f(1000,350-30);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(255,255,255);
    glVertex2f(990,370);
    glVertex2f(990,300);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(255,255,255);
    glVertex2f(1140,370);
    glVertex2f(1140,300);
    glEnd();



}

void drawTrees()
{
    glPushMatrix();
    for(int i=70; i <=200; i++){
    glTranslatef(i,0.0,0.0);
    ///base
    glBegin(GL_QUADS);
    glColor3f(0.222, 0.184, 0.135);
    glVertex2f(895, 390);
    glVertex2f(906, 390);
    glVertex2f(906, 440);
    glVertex2f(895, 440);
    glEnd();

    //middle leaf
    drawCircle(895,440,20,0.0,0.155,0.0);
    drawCircle(915,440,20,0.0,0.155,0.0);
    drawCircle(906,460,20,0.0,0.155,0.0);
}
    glPopMatrix();

}

void drawBusStand()
{
    ///Base
    glBegin(GL_QUADS);
    if(isDay){glColor3ub(0,153,153);}
    else{glColor3f(0.102, 0.178, 0.255);}
        glVertex2f(100,500);
        glVertex2f(800,500);
        glVertex2f(800,600);
        glVertex2f(100,600);
    glEnd();

    //counter
    glPushMatrix();
    for(int i = 0; i <= 600; i += 110) {
    glPushMatrix();
    glTranslatef(i, 0.0, 0.0);
    glBegin(GL_QUADS);
    if(isDay){glColor3ub(255,0,0);}
    else{glColor3ub(255,0,25);}
        glVertex2f(110,500);
        glVertex2f(180,500);
        glVertex2f(180,560);
        glVertex2f(110,560);
    glEnd();

    glBegin(GL_QUADS);
    if(isDay){glColor3ub(255,220,0);}
    else{glColor3ub(0,0,255);}
        glVertex2f(110,550);
        glVertex2f(180,550);
        glVertex2f(180,560);
        glVertex2f(110,560);
    glEnd();

    glBegin(GL_QUADS);
    if(isDay){glColor3ub(0,0,51);}
    else{glColor3ub(110,110,255);}
        glVertex2f(110,520);
        glVertex2f(180,520);
        glVertex2f(180,550);
        glVertex2f(110,550);
    glEnd();

    glBegin(GL_QUADS);
    if(isDay){glColor3ub(255,255,255);}
    else{glColor3ub(255,255,255);}
        glVertex2f(140,520);
        glVertex2f(150,520);
        glVertex2f(150,550);
        glVertex2f(140,550);
    glEnd();
    glPopMatrix();
    }
    glPopMatrix();

    ///shade
    glPushMatrix();
    for(int i = 0; i <= 600; i += 100) {
    glPushMatrix();
    glTranslatef(i, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    if(isDay){
        glColor3ub(255,255,204);
    }
    else{
        glColor3ub(102,102,0);
    }
        glVertex2f(100, 600);
        glVertex2f(200, 600);
        glVertex2f(150, 650);
    glEnd();

    //piller
     glBegin(GL_QUADS);
        glColor3f(0.102, 0.0, 0.255);
        glVertex2f(200,500);
        glVertex2f(210,500);
        glVertex2f(210,600);
        glVertex2f(200,600);
    glEnd();

    glPopMatrix();
    }
    glPopMatrix();

    //front shade
    if(isDay){
        glColor3ub(160,160,160);
    }
    else{
        glColor3ub(64,64,64);
    }
    glBegin(GL_QUADS);
        glVertex2f(80,580);
        glVertex2f(780,580);
        glVertex2f(800,600);
        glVertex2f(100,600);
    glEnd();

    //floor
    glBegin(GL_QUADS);
        glColor3ub(204,204,0);
        glVertex2f(80,480);
        glVertex2f(780,480);
        glVertex2f(800,500);
        glVertex2f(100,500);
    glEnd();

    ///Sign Board
    if(isDay){
        glColor3ub(255,102,102);
    }
    else{
        glColor3ub(255,0,0);
    }
    glBegin(GL_QUADS);
        glVertex2f(200,600);
        glVertex2f(700,600);
        glVertex2f(700,630);
        glVertex2f(200,630);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("Jatrabari Bus Stand . DHAKA",300,610);

}



void display1() {
    drawSky();
    drawCloud();
    if(!isDay){
    drawPlane();}
    drawBG_home();
    drawHomeUp();
    drawMiddle();
    drawGround();
    drawRoad();
    drawBusStand();
    drawTrees();
    drawBStop();
    if (isRaining)
        drawRain();
    //human();
    drawBus1();
    drawBus2();
    drawCityLights(200);

}

void mouse1(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        switch (button) {
            case GLUT_LEFT_BUTTON:
                if (currentScreen == 1) {
                    isPlaneMoving = !isPlaneMoving;
                }
                break;
            case GLUT_RIGHT_BUTTON:
                if (currentScreen == 1) {
                    isPlaneMoving = !isPlaneMoving;
                }
                break;
            case GLUT_MIDDLE_BUTTON:
                if (currentScreen == 1) {
                    isBus1Moving = false;
                    isBus2Moving = false;
                    isRotating = false;
                }
                break;
            default:
                break;
        }
    }
}


/////////////////////////////////////////////////////////// DISPLAY 2 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///NAME : MAESA ANJUM
///ID : 22-46240-1
///PART 2


void updateF(int);
float moveA = 0.0f;
float moveB = 0.0f;
float moveC = 0.0f;
float moveF = 0.0f;
float moveV=0.0f;
float moveS=0.0f;
float speed = 0.0f;
float savedSpeed = 0.0f;
bool paused = false;
float _angle1 = 45.0f;
float _angle2 = 0.0f;
float _angle3 = 90.0f;
float _angle4 = 135.0f;
float _angle5 = 180.0f;
float _angle6 = 225.0f;
float _angle7 = 275.0f;
float _angle8 = 315.0f;
bool isMorning=true;

void initScreen2() {
    glClearColor(.8, 1.0, 0.6, .0);       // Set the background color to Mon Moto
    glColor3f(1.0, 1.0, 1.0);               // Set the drawing color to white
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 1200, 0, 800);            // Set the orthographic projection
}

/////////////////////////////////////  UPDATE FUNCTION FOR SCREEN 2  ///////////////////////////////////////////////////////////////////////////

void updateM(int value) {
_angle1 -=  1.0f;
	if (_angle1 < -360) {
		_angle1 += 360;
	}
	_angle2 -=  1.0f;
	if (_angle2 < -360) {
		_angle2 += 360;
	}
	_angle3 -=  1.0f;
	if (_angle3 < -360) {
		_angle3 += 360;
	}
_angle4 -=  1.0f;
	if (_angle4 < -360) {
		_angle4 += 360;
	}

	_angle5 -=  1.0f;
	if (_angle5 < -360) {
		_angle5 += 360;
	}
	_angle6 -=  1.0f;
	if (_angle6 < -360) {
		_angle6 += 360;
	}
	_angle7 -=  1.0f;
	if (_angle7 < -360) {
		_angle7 += 360;
	}
_angle8 -=  1.0f;
	if (_angle8 < -360) {
		_angle8 += 360;
	}

     moveB -= 10.8;

     if(moveB< -1200)
    {
        moveB=1200;
    }
    glutPostRedisplay();  // Notify GLUT that the display has changed
    glutTimerFunc(20, updateM, 0);  // Update every 20 milliseconds
}


void updateF(int value)
{

if (speed > 1.0f || speed < 0.0f) {
    speed = 0.0f;
}
moveF -= speed;
if (moveF < -1200) {
    moveF = 0;
}
	glutPostRedisplay(); //Tell GLUT that the display has changed
	glutTimerFunc(25, updateF, 0);
}

void updateV(int value) { /// Megh2

     moveV += 10.8;

     if(moveV > 1200)
    {
        moveV=-200;
    }
     glutPostRedisplay();  // Notify GLUT that the display has changed
    glutTimerFunc(20, updateV, 0);  // Update every 20 milliseconds
}
void updateS(int value) {  /// Megh

     moveS += 5.8;

     if(moveS > 1200)
    {
        moveS=-200;
    }
     glutPostRedisplay();  // Notify GLUT that the display has changed
    glutTimerFunc(20, updateS, 0);  // Update every 20 milliseconds
}


/////////////////////////////////////////// DRAW ITEMS ////////////////////////////////////////////

void Noksha()
{glLineWidth(12);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(505,470);
        glVertex2f(505,593);
    glEnd();

    glLineWidth(3);
   glBegin(GL_LINES);
       glColor3ub(1,1,1);
        glVertex2f(355,470);
        glVertex2f(505,590);
    glEnd();

        glLineWidth(3);
   glBegin(GL_LINES);
       glColor3ub(1,1,1);
        glVertex2f(385,470);
        glVertex2f(505,590);
    glEnd();

    glLineWidth(3);
   glBegin(GL_LINES);
       glColor3ub(1,1,1);
        glVertex2f(415,470);
        glVertex2f(505,590);
    glEnd();

    glLineWidth(3);
   glBegin(GL_LINES);
       glColor3ub(1,1,1);
        glVertex2f(445,470);
        glVertex2f(505,590);
    glEnd();
        glLineWidth(3);
   glBegin(GL_LINES);
       glColor3ub(1,1,1);
        glVertex2f(475,470);
        glVertex2f(505,590);
    glEnd();

        glLineWidth(3);
   glBegin(GL_LINES);
       glColor3ub(1,1,1);
        glVertex2f(535,470);
        glVertex2f(505,590);
    glEnd();

    glLineWidth(3);
   glBegin(GL_LINES);
       glColor3ub(1,1,1);
        glVertex2f(565,470);
        glVertex2f(505,590);
    glEnd();

    glLineWidth(3);
   glBegin(GL_LINES);
       glColor3ub(1,1,1);
        glVertex2f(595,470);
        glVertex2f(505,590);
    glEnd();

    glLineWidth(3);
   glBegin(GL_LINES);
       glColor3ub(1,1,1);
        glVertex2f(625,470);
        glVertex2f(505,590);
    glEnd();

        glLineWidth(3);
   glBegin(GL_LINES);
       glColor3ub(1,1,1);
        glVertex2f(655,470);
        glVertex2f(505,590);
    glEnd();

    glLineWidth(12);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(805,470);
        glVertex2f(805,593);
    glEnd();

        glLineWidth(3);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(775,470);
        glVertex2f(805,590);
    glEnd();

        glLineWidth(3);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(745,470);
        glVertex2f(805,590);
    glEnd();

        glLineWidth(3);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(715,470);
        glVertex2f(805,590);
    glEnd();

    glLineWidth(3);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(685,470);
        glVertex2f(805,590);
    glEnd();

    glLineWidth(3);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(655,470);
        glVertex2f(805,590);
    glEnd();

    glLineWidth(03);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(835,470);
        glVertex2f(805,590);
    glEnd();

    glLineWidth(03);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(865,470);
        glVertex2f(805,590);
    glEnd();

    glLineWidth(03);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(895,470);
        glVertex2f(805,590);
    glEnd();

    glLineWidth(3);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(925,470);
        glVertex2f(805,590);
    glEnd();

    glLineWidth(3);
   glBegin(GL_LINES);

       glColor3ub(1,1,1);
        glVertex2f(955,470);
        glVertex2f(805,590);
    glEnd();
}
void drawWheel() {
    // Outer tire
    glColor3f(0.0, 0.0, 0.0);  // Tire color (black)
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float pi = 3.1416;
        float angle = (i * 2 * pi) / 180;
        float r = 35;
        float x = r * cos(angle);
        float y = r * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Inner rim
    glColor3f(0.7, 0.7, 0.7);  // Rim color (gray)
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float pi = 3.1416;
        float angle = (i * 2 * pi) / 180;
        float r = 25;
        float x = r * cos(angle);
        float y = r * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Hub
    glColor3f(0.0, 0.0, 0.0);  // Hub color (black)
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float pi = 3.1416;
        float angle = (i * 2 * pi) / 180;
        float r = 15;
        float x = r * cos(angle);
        float y = r * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawMiddlePartM()
{
        if(isMorning)
    {
         glColor3ub(166,195,140);      // Set the background color to Mon Moto
    }
   else{
    glColor3ub(106,125,90);
   }
    glBegin(GL_QUADS);

    glVertex2f(0, 400);
    glVertex2f(1200, 400);
    glVertex2f(1200, 600);
    glVertex2f(0, 600);
    glEnd();
}

void drawSkyM() {
    if (isMorning) {
        glColor3f(0.53, 0.81, .92);  // Sky color (sky)
    } else {
        glColor3ub(210,130,146); // Evening
    }
    glBegin(GL_QUADS);

    glVertex2f(0, 600);
    glVertex2f(1200, 600);
    glVertex2f(1200, 800);
    glVertex2f(0, 800);
    glEnd();
}
void drawPlaneM()
{

if(isMorning)
{
    return;
}
else{
{
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glTranslatef(0,300,0);
    glScalef(.6,.6,1);
    {

	glTranslatef(moveB,0.0f, 0.0f);
     glBegin(GL_QUADS);
    glColor3ub(150,140,140);  // BODY of the PlAnE
    glVertex2f(1100-30, 700);
    glVertex2f(1200, 700);
    glVertex2f(1200, 750);
    glVertex2f(1100-30, 750);
    glEnd();

     glBegin(GL_QUADS);
     glColor3ub(150,140,140);  // MUKH of the PlAnE
    glVertex2f(1070-30, 710);
    glVertex2f(1100-30, 700);
    glVertex2f(1100-30, 725);
    glVertex2f(1070-30, 725);
    glEnd();

     glBegin(GL_QUADS);
    glColor3ub(86,76,76); // MUKH of the PlAnE
    glVertex2f(1070-30, 725);
     glVertex2f(1100-30, 725);
      glVertex2f(1100-30, 750);
    glVertex2f(1090-30, 750);
    glEnd();

         glBegin(GL_QUADS);
    glColor3ub(190,187,187);  // PanKhA of the PlAnE
    glVertex2f(1150-30, 680);
     glVertex2f(1190-30, 680);
      glVertex2f(1180-30, 720);
    glVertex2f(1140-30, 720);
    glEnd();

       glBegin(GL_QUADS);
   glColor3ub(190,187,187); // PanKhA of the PlAnE
    glVertex2f(1130-30, 750);
     glVertex2f(1170-30, 750);
      glVertex2f(1180-30, 790);
    glVertex2f(1145-30, 790);
    glEnd();

           glBegin(GL_QUADS);
    glColor3ub(54,48,48);// LEnjAA of the PlAnE
    glVertex2f(1180, 750);
     glVertex2f(1200, 750);
      glVertex2f(1200, 790);
    glVertex2f(1190, 790);
    glEnd();
        }

    glPopMatrix();
}
}
}
void drawMegh()
{
    if(isMorning)
    {
glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glTranslatef(moveS, 0.0f, 0.0f);
         drawCircle(300, 700, 35, 204.0, 201.0, 192.0);
          drawCircle(260, 700, 35, 204.0, 201.0, 192.0);
           drawCircle(340, 700, 35, 204.0, 201.0, 192.0);
            drawCircle(300, 720, 35, 204.0, 201.0, 192.0);
             drawCircle(300, 680, 35, 204.0, 201.0, 192.0);
             glPopMatrix();
    }
    else
    {
        return;
    }

}
void drawMegh2()
{
    if(isMorning)
    {    glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glTranslatef(moveV, 0.0f, 0.0f);


         drawCircle(80, 720, 29, 1.0, 1.0, 1.0);
          drawCircle(150, 720, 25, 1.0, 1.0, 1.0);
           drawCircle(130, 725, 29, 1.0, 1.0, 1.0);
            drawCircle(100, 740, 25, 1.0, 1.0, 1.0);
             drawCircle(85, 700, 25, 1.0, 1.0, 1.0);
              drawCircle(120, 685, 25, 1.0, 1.0, 1.0);
              glPopMatrix();
    }
    else
    {
        return;
    }

}
void drawSunM()
{ if(isMorning){
    glPushMatrix();
glColor3ub(252,186,6);
glTranslatef(650,720,0);
glScalef(1,1,0);
glutSolidSphere(40,40,40);
glPopMatrix();
}
else{

    return ;
}
}

void drawGroundM() {
    if(isMorning)
    {
       glColor3f(0.124, 0.252, 0.0);  // Ground color (green)
    }
    else
    {
        glColor3ub(1,50,32); ///Darker for thr Evening
    }
    glBegin(GL_QUADS);

    glVertex2f(0, 0);
    glVertex2f(1200, 0);
    glVertex2f(1200, 400);
    glVertex2f(0, 400);
    glEnd();
}

void drawMelarChakka()
{glPushMatrix();
    glTranslatef(600,350,0);
glScalef(.5,.5,0);
{


    glBegin(GL_TRIANGLES);
    glColor3d(1,0,0);
    glVertex2f(920,350);
     glVertex2f(1050,500);
    glVertex2f(970,350);
glEnd();

    glBegin(GL_TRIANGLES);
    glColor3d(1,0,0);
    glVertex2f(1120,350);
     glVertex2f(1050,500);
    glVertex2f(1170,350);
glEnd();
       // Middle Blue
{
    if (isMorning)
    {
       glColor3f(0.0,0.0,1.0);
    }
    else
    {
       glColor3ub(232,218,16);
    }

    glPushMatrix();

glTranslatef(1050,500,0);
glScalef(1,1,0);
glutSolidSphere(40,40,40);
glPopMatrix();
}

float centerX5 = 1050.0f;
float centerY5= 500.0f;
float r = 100.0f;

glPushMatrix();
    glColor3f(.0, .0, .0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 400; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 400;


        float x = r * cos(A);
        float y = r * sin(A);


        glVertex2f(x + centerX5, y + centerY5);
    }
    glEnd();
glPopMatrix();

glPushMatrix();
glTranslatef(1050.0f, 500.0f, 0.0f);
glRotatef(_angle1, 0.0, 0.0, 1.0);

float centerX = 80.0f;
float centerY = 80.0f;
glColor3f(0.40, .0, 0.80);
glBegin(GL_POLYGON);
for (int i = 0; i < 400; i++) {
    float pi = 3.1416;
    float A = (i * 2 * pi) / 400;
    float r = 30;
    float x = r * cos(A);
    float y = r * sin(A);
    glVertex2f(x + centerX, y + centerY);
}
glEnd();

glPopMatrix();

glPushMatrix();
glTranslatef(1050.0f, 500.0f, 0.0f);
glRotatef(_angle2, 0.0, 0.0, 1.0);
float centerX1 = 80.0f;
float centerY1 = 80.0f;
    glColor3f(.40, .0, .80);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 400; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 400;
        float r = 30;
        float x = r * cos(A);
        float y = r * sin(A);


        glVertex2f(x + centerX1, y + centerY1);
    }
    glEnd();
glPopMatrix();

glPushMatrix();
glTranslatef(1050.0f, 500.0f, 0.0f);
glRotatef(_angle3, 0.0, 0.0, 1.0);
float centerX3 = 80.0f;
float centerY3 = 80.0f;
    glColor3f(.40, .0, .80);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 400; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 400;
        float r = 30;
        float x = r * cos(A);
        float y = r * sin(A);


        glVertex2f(x + centerX3, y + centerY3);
    }
    glEnd();
glPopMatrix();


glPushMatrix();
glTranslatef(1050.0f, 500.0f, 0.0f);
glRotatef(_angle4, 0.0, 0.0, 1.0);
float centerX4 = 80.0f;
float centerY4 = 80.0f;
    glColor3f(.40, .0, .80);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 400; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 400;
        float r = 30;
        float x = r * cos(A);
        float y = r * sin(A);


        glVertex2f(x + centerX4, y + centerY4);

glPushMatrix();
glTranslatef(1050.0f, 500.0f, 0.0f);
glRotatef(_angle5, 0.0, 0.0, 1.0);
float centerX5 = 80.0f;
float centerY5 = 80.0f;
    glColor3f(.40, .0, .80);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 400; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 400;
        float r = 30;
        float x = r * cos(A);
        float y = r * sin(A);


        glVertex2f(x + centerX5, y + centerY5);
    }
    glEnd();
glPopMatrix();


glPushMatrix();
glTranslatef(1050.0f, 500.0f, 0.0f);
glRotatef(_angle6, 0.0, 0.0, 1.0);
float centerX6 = 80.0f;
float centerY6 = 80.0f;
    glColor3f(.40, .0, .80);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 400; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 400;
        float r = 30;
        float x = r * cos(A);
        float y = r * sin(A);


        glVertex2f(x + centerX6, y + centerY6);
    }
    glEnd();
glPopMatrix();

glPushMatrix();
glTranslatef(1050.0f, 500.0f, 0.0f);
glRotatef(_angle7, 0.0, 0.0, 1.0);
float centerX7 = 80.0f;
float centerY7 = 80.0f;
    glColor3f(.40, .0, .80);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 400; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 400;
        float r = 30;
        float x = r * cos(A);
        float y = r * sin(A);
        glVertex2f(x + centerX7, y + centerY7);
    }
    glEnd();
glPopMatrix();

glPushMatrix();
glTranslatef(1050.0f, 500.0f, 0.0f);
glRotatef(_angle8, 0.0, 0.0, 1.0);
float centerX8 = 80.0f;
float centerY8 = 80.0f;
    glColor3f(.4, 0.0, .80);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 400; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 400;
        float r = 30;
        float x = r * cos(A);
        float y = r * sin(A);


        glVertex2f(x + centerX7, y + centerY7);
    }
    glEnd();
glPopMatrix();
    }

glPopMatrix();

}
}
void drawBuildings1()
{
   //BUILDING  1

glPushMatrix();
glTranslatef(0,350,0);
glScalef(.5,.5,0);

     glBegin(GL_QUADS);

       glColor3ub(178,155,186);
        glVertex2f(0,500);
        glVertex2f(200,500);
        glVertex2f(215,520);
        glVertex2f(0,520);
    glEnd();

      glBegin(GL_QUADS);
       glColor3d(0,0,0);
        glVertex2f(185-50,510);
        glVertex2f(190-50,510);
        glVertex2f(190-50,610);
        glVertex2f(185-50,610);
    glEnd();
          glBegin(GL_QUADS);
       glColor3d(0,0,0);
        glVertex2f(155-50,510+30);
        glVertex2f(220-50,510+30);
        glVertex2f(220-50,510+35);
        glVertex2f(155-50,510+35);
    glEnd();

              glBegin(GL_QUADS);
       glColor3d(0,0,0);
        glVertex2f(155+10-50,510+60);
        glVertex2f(220-10-50,510+60);
        glVertex2f(220-10-50,510+65);
        glVertex2f(155+10-50,510+65);
    glEnd();

                  glBegin(GL_QUADS);
       glColor3d(0,0,0);
        glVertex2f(155+20-50,510+90);
        glVertex2f(220-20-50,510+90);
        glVertex2f(220-20-50,510+95);
        glVertex2f(155+20-50,510+95);
    glEnd();
{
    if(isMorning)
    {
       glColor3ub(80,71,66);
    }
    else
    {
          glColor3ub(118,171,166);
    }
    glBegin(GL_QUADS);
        glVertex2f(0,300);
        glVertex2f(200,300);
        glVertex2f(200,500);
        glVertex2f(0,500);
    glEnd();
}

{
    if(isMorning)
    {
       glColor3d(.8,.8,0);
    }
    else
    {
       glColor3d(1.,1.,0);
    }

        glBegin(GL_QUADS);
        glVertex2f(60,300);
        glVertex2f(70,300);
        glVertex2f(70,500);
        glVertex2f(60,500);
    glEnd();

            glBegin(GL_QUADS);
        glVertex2f(130,300);
        glVertex2f(140,300);
        glVertex2f(140,500);
        glVertex2f(130,500);
    glEnd();


            glBegin(GL_QUADS);
        glVertex2f(0,390);
        glVertex2f(200,390);
        glVertex2f(200,410);
        glVertex2f(0,410);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(200,300);
        glVertex2f(215,300);
        glVertex2f(215,520);
        glVertex2f(200,500);
    glEnd();


}
glPopMatrix();

}

void drawBuildings2()
{
   //BUILDING  2
glPushMatrix();
glTranslatef(0,350,0);
glScalef(.5,.5,0);
      glBegin(GL_QUADS);
       glColor3ub(184,148,66);
        glVertex2f(215,300);
        glVertex2f(350,300);
        glVertex2f(350,620);
        glVertex2f(215,620);
    glEnd();
     glBegin(GL_QUADS);
        glColor3ub(66,120,113);
        glVertex2f(225,300);
        glVertex2f(340,300);
        glVertex2f(340,610);
        glVertex2f(225,610);
    glEnd();
{glPushMatrix();

    if(isMorning)
    {
              glColor3ub(66,205,161);
    }
    else{
glColor3ub(244,245,61);
    }
     glBegin(GL_QUADS);

        glVertex2f(235,600);
        glVertex2f(280,600);
        glVertex2f(280,560);
        glVertex2f(235,560);
    glEnd();
                glBegin(GL_QUADS);
        glVertex2f(235+50,600);
        glVertex2f(280+50,600);
        glVertex2f(280+50,560);
        glVertex2f(235+50,560);
    glEnd();
      glBegin(GL_QUADS);
        glVertex2f(235,600-100);
        glVertex2f(280,600-100);
        glVertex2f(280,560-100);
        glVertex2f(235,560-100);
    glEnd();

      glBegin(GL_QUADS);
         glVertex2f(235+50,600-100);
        glVertex2f(280+50,600-100);
        glVertex2f(280+50,560-100);
        glVertex2f(235+50,560-100);
    glEnd();
         glBegin(GL_QUADS);
        glVertex2f(235,600-50);
        glVertex2f(280,600-50);
        glVertex2f(280,560-50);
        glVertex2f(235,560-50);
    glEnd();

            glBegin(GL_QUADS);
        glVertex2f(235+50,600-50);
        glVertex2f(280+50,600-50);
        glVertex2f(280+50,560-50);
        glVertex2f(235+50,560-50);
    glEnd();

      glBegin(GL_QUADS);
        glVertex2f(235,600-150);
        glVertex2f(280,600-150);
        glVertex2f(280,560-150);
        glVertex2f(235,560-150);
    glEnd();

            glBegin(GL_QUADS);
        glVertex2f(235+50,600-150);
        glVertex2f(280+50,600-150);
        glVertex2f(280+50,560-150);
        glVertex2f(235+50,560-150);
    glEnd();


      glBegin(GL_QUADS);
        glVertex2f(235,600-250);
        glVertex2f(280,600-250);
        glVertex2f(280,560-250);
        glVertex2f(235,560-250);
    glEnd();

            glBegin(GL_QUADS);
        glVertex2f(235+50,600-250);
        glVertex2f(280+50,600-250);
        glVertex2f(280+50,560-250);
        glVertex2f(235+50,560-250);
    glEnd();

          glBegin(GL_QUADS);
        glVertex2f(235,600-200);
        glVertex2f(280,600-200);
        glVertex2f(280,560-200);
        glVertex2f(235,560-200);
    glEnd();

            glBegin(GL_QUADS);
        glVertex2f(235+50,600-200);
        glVertex2f(280+50,600-200);
        glVertex2f(280+50,560-200);
        glVertex2f(235+50,560-200);
    glEnd();

glPopMatrix();
}
glPopMatrix();
}

void drawBuildings3()
{


   //BUILDING  3
glPushMatrix();
glTranslatef(0,350,0);
glScalef(.5,.5,0);
      glBegin(GL_QUADS);
       glColor3ub(140,109,140);
        glVertex2f(350,300);
        glVertex2f(500,300);
        glVertex2f(500,400);
        glVertex2f(350,400);
    glEnd();

      glBegin(GL_QUADS);
       glColor3ub(140,109,140);
        glVertex2f(375,400);
        glVertex2f(475,400);
        glVertex2f(475,500);
        glVertex2f(375,500);
    glEnd();

     glBegin(GL_QUADS);
       glColor3ub(140,109,140);
        glVertex2f(400,500);
        glVertex2f(450,500);
        glVertex2f(450,580);
        glVertex2f(400,580);
    glEnd();


  {

    if(isMorning)
    {
        glColor3ub(66,120,113);
    }
    else
    {
        glColor3d(1.0,1.0,.0);
    }
             glBegin(GL_QUADS);
        glVertex2f(410,520);
        glVertex2f(440,520);
        glVertex2f(440,560);
        glVertex2f(410,560);
    glEnd();


              glBegin(GL_QUADS);
        glVertex2f(450-15,430);
        glVertex2f(475-15,430);
        glVertex2f(475-15,470);
        glVertex2f(450-15,470);
    glEnd();

         glBegin(GL_QUADS);
        glVertex2f(400-15,430);
        glVertex2f(425-15,430);
        glVertex2f(425-15,470);
        glVertex2f(400-15,470);
    glEnd();

                 glBegin(GL_QUADS);
        glVertex2f(440+15,330);
        glVertex2f(465+15,330);
        glVertex2f(465+15,370);
        glVertex2f(440+15,370);
    glEnd();

                  glBegin(GL_QUADS);
        glVertex2f(395+15,330);
        glVertex2f(420+15,330);
        glVertex2f(420+15,370);
        glVertex2f(395+15,370);
    glEnd();
           glBegin(GL_QUADS);
        glVertex2f(350+15,330);
        glVertex2f(375+15,330);
        glVertex2f(375+15,370);
        glVertex2f(350+15,370);
    glEnd();


    }
glPopMatrix();
}
void drawRoadM() {
    if(isMorning)
    {
          glColor3f(0.3, 0.3, 0.3);  // Road color (gray)
    }
    else
        {
                glColor3ub(62,60,76);    // Road color Darker
  }
    // Road layer 1
    glBegin(GL_QUADS);

    glVertex2f(0, 160);
    glVertex2f(1200, 160);
    glVertex2f(1200, 310);
    glVertex2f(0, 310);
    glEnd();


}
void drawRoad2M() {
        if(isMorning)
    {
          glColor3f(0.22, 0.22, 0.22);  // Road color (gray)
    }
    else
        {
              glColor3ub(40,40,40);  // Road color Darker

    }
    // Road layer 2
    glBegin(GL_QUADS);
    glVertex2f(0, 160+190);
    glVertex2f(1200, 160+190);
    glVertex2f(1200, 300+190);
    glVertex2f(0, 300+190);
    glEnd();

}

void drawRoadLinesM()
{
     //glLineWidth(10);
    glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(0,230);
    glVertex2f(100,230);
    glVertex2f(100,240);
    glVertex2f(0,240);
    glEnd();

        glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(125,230);
    glVertex2f(225,230);
    glVertex2f(225,240);
    glVertex2f(125,240);
    glEnd();

        glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(250,230);
    glVertex2f(350,230);
    glVertex2f(350,240);
    glVertex2f(250,240);
    glEnd();

        glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(375,230);
    glVertex2f(475,230);
    glVertex2f(475,240);
    glVertex2f(375,240);
    glEnd();

        glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(500,230);
    glVertex2f(600,230);
    glVertex2f(600,240);
    glVertex2f(500,240);
    glEnd();

            glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(625,230);
    glVertex2f(725,230);
    glVertex2f(725,240);
    glVertex2f(625,240);
    glEnd();

                glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(750,230);
    glVertex2f(850,230);
    glVertex2f(850,240);
    glVertex2f(750,240);
    glEnd();
glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(875,230);
    glVertex2f(975,230);
    glVertex2f(975,240);
    glVertex2f(875,240);
    glEnd();
glBegin(GL_QUADS);

    glColor3d(1,1,0);
    glVertex2f(900,230);
    glVertex2f(1000,230);
    glVertex2f(1000,240);
    glVertex2f(900,240);
    glEnd();

    glBegin(GL_QUADS);

    glColor3d(1,1,0);
    glVertex2f(1025,230);
    glVertex2f(1125,230);
    glVertex2f(1125,240);
    glVertex2f(1025,240);
    glEnd();

    glBegin(GL_QUADS);

    glColor3d(1,1,0);
    glVertex2f(1150,230);
    glVertex2f(1200,230);
    glVertex2f(1200,240);
    glVertex2f(1150,240);
    glEnd();
}

void drawRoadLines2M()
{
     //glLineWidth(10);
    glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(0,230+180);
    glVertex2f(100,230+180);
    glVertex2f(100,240+180);
    glVertex2f(0,240+180);
    glEnd();

        glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(125,230+180);
    glVertex2f(225,230+180);
    glVertex2f(225,240+180);
    glVertex2f(125,240+180);
    glEnd();

        glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(250,230+180);
    glVertex2f(350,230+180);
    glVertex2f(350,240+180);
    glVertex2f(250,240+180);
    glEnd();

        glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(375,230+180);
    glVertex2f(475,230+180);
    glVertex2f(475,240+180);
    glVertex2f(375,240+180);
    glEnd();

        glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(500,230+180);
    glVertex2f(600,230+180);
    glVertex2f(600,240+180);
    glVertex2f(500,240+180);
    glEnd();

            glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(625,230+180);
    glVertex2f(725,230+180);
    glVertex2f(725,240+180);
    glVertex2f(625,240+180);
    glEnd();

                glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(750,230+180);
    glVertex2f(850,230+180);
    glVertex2f(850,240+180);
    glVertex2f(750,240+180);
    glEnd();
glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(875,230+180);
    glVertex2f(975,230+180);
    glVertex2f(975,240+180);
    glVertex2f(875,240+180);
    glEnd();
glBegin(GL_QUADS);

    glColor3d(1,1,0);
    glVertex2f(900,230+180);
    glVertex2f(1000,230+180);
    glVertex2f(1000,240+180);
    glVertex2f(900,240+180);
    glEnd();

    glBegin(GL_QUADS);

    glColor3d(1,1,0);
    glVertex2f(1025,230+180);
    glVertex2f(1125,230+180);
    glVertex2f(1125,240+180);
    glVertex2f(1025,240+180);
    glEnd();

    glBegin(GL_QUADS);

    glColor3d(1,1,0);
    glVertex2f(1150,230+180);
    glVertex2f(1200,230+180);
    glVertex2f(1200,240+180);
    glVertex2f(1150,240+180);
    glEnd();


}
void drawRailBridgeM()
{      if(isMorning)
    {
               glColor3ub(107,16,16);
    }
    else
    {
        glColor3ub(60,0,8); ///Darker for thr Evening
    }
    glBegin(GL_QUADS);

        glVertex2f(0,440);
        glVertex2f(1200,440);
        glVertex2f(1200,470);
        glVertex2f(0,470);
    glEnd();
}

void drawRailBridgeLinesM()
{      if(isMorning)
    {
                     glColor3ub(96,116,99);
    }
    else
    {
       glColor3ub(91,95,125); ///Darker for thr Evening
    }

    glBegin(GL_QUADS);

        glVertex2f(40,325);
        glVertex2f(70,325);
        glVertex2f(70,440);
        glVertex2f(40,440);
    glEnd();
    glPushMatrix();
    glTranslatef(150,0,0);
        glBegin(GL_QUADS);

        glVertex2f(40,325);
        glVertex2f(70,325);
        glVertex2f(70,440);
        glVertex2f(40,440);
    glEnd();
    glPopMatrix();
glPushMatrix();
     glTranslatef(300,0,0);
        glBegin(GL_QUADS);

        glVertex2f(40,325);
        glVertex2f(70,325);
        glVertex2f(70,440);
        glVertex2f(40,440);
    glEnd();
    glPopMatrix();
glPushMatrix();
     glTranslatef(450,0,0);
        glBegin(GL_QUADS);

        glVertex2f(40,325);
        glVertex2f(70,325);
        glVertex2f(70,440);
        glVertex2f(40,440);
    glEnd();
    glPopMatrix();
glPushMatrix();
     glTranslatef(600,0,0);
        glBegin(GL_QUADS);

        glVertex2f(40,325);
        glVertex2f(70,325);
        glVertex2f(70,440);
        glVertex2f(40,440);
    glEnd();
    glPopMatrix();
glPushMatrix();
     glTranslatef(750,0,0);
        glBegin(GL_QUADS);

        glVertex2f(40,325);
        glVertex2f(70,325);
        glVertex2f(70,440);
        glVertex2f(40,440);
    glEnd();
    glPopMatrix();
glPushMatrix();
     glTranslatef(900,0,0);
        glBegin(GL_QUADS);

        glVertex2f(40,325);
        glVertex2f(70,325);
        glVertex2f(70,440);
        glVertex2f(40,440);
    glEnd();
    glPopMatrix();
glPushMatrix();
     glTranslatef(1050,0,0);
        glBegin(GL_QUADS);

        glVertex2f(40,325);
        glVertex2f(70,325);
        glVertex2f(70,440);
        glVertex2f(40,440);
    glEnd();
    glPopMatrix();
}
void drawTrainM()      /// Picher part
{

    glPushMatrix();
    glTranslatef(moveF,0.0f, 0.0f);
    {
     glBegin(GL_QUADS);
       glColor3ub(56,50,36);
        glVertex2f(1050,470);
        glVertex2f(1200,470);
        glVertex2f(1200,530);
        glVertex2f(1050,530);
    glEnd();

    glBegin(GL_QUADS);
       glColor3ub(65,80,126);
        glVertex2f(1055,520);
        glVertex2f(1200,520);
        glVertex2f(1200,525);
        glVertex2f(1055,525);
    glEnd();

        glBegin(GL_QUADS);
       glColor3ub(65,80,126);
        glVertex2f(1055,475);
        glVertex2f(1200,475);
        glVertex2f(1200,480);
        glVertex2f(1055,480);
    glEnd();

        glBegin(GL_QUADS);
       glColor3ub(228,229,235);
        glVertex2f(1055,485);
        glVertex2f(1200,485);
        glVertex2f(1200,515);
        glVertex2f(1055,515);
    glEnd();

            glBegin(GL_QUADS);
       glColor3ub(145,167,247);
        glVertex2f(1060,490);
        glVertex2f(1080,490);
        glVertex2f(1080,510);
        glVertex2f(1060,510);
    glEnd();

                glBegin(GL_QUADS);
       glColor3ub(145,167,247);
        glVertex2f(1090,490);
        glVertex2f(1110,490);
        glVertex2f(1110,510);
        glVertex2f(1090,510);
    glEnd();

                    glBegin(GL_QUADS);
       glColor3ub(145,167,247);
        glVertex2f(1120,490);
        glVertex2f(1140,490);
        glVertex2f(1140,510);
        glVertex2f(1120,510);
    glEnd();

                        glBegin(GL_QUADS);
       glColor3ub(145,167,247);
        glVertex2f(1150,490);
        glVertex2f(1170,490);
        glVertex2f(1170,510);
        glVertex2f(1150,510);
    glEnd();

        glBegin(GL_QUADS);
       glColor3ub(145,167,247);
        glVertex2f(1180,490);
        glVertex2f(1195,490);
        glVertex2f(1195,510);
        glVertex2f(1180,510);
    glEnd();

glPushMatrix();
glTranslatef(-160,0,0);   ///FRONT PART

{

      if(isMorning==false)
    {

glBegin(GL_QUADS);
       glColor3f( 1.,1.,0.55);
        glVertex2f(923,471);
        glVertex2f(1000,482);
        glVertex2f(1020,485);
        glVertex2f(930,515);
    glEnd();
      drawCircle(930.,493., 22.00, 1.,1.,0.26);
    }
   glBegin(GL_QUADS);
       glColor3ub(56,50,36);
        glVertex2f(990,470);
        glVertex2f(1200,470);
        glVertex2f(1200,530);
        glVertex2f(1050,530);
    glEnd();

    glBegin(GL_QUADS);
       glColor3ub(65,80,126);
        glVertex2f(1055,520);
        glVertex2f(1200,520);
        glVertex2f(1200,525);
        glVertex2f(1060,525);
    glEnd();

        glBegin(GL_QUADS);
       glColor3ub(65,80,126);
        glVertex2f(1005,475);
        glVertex2f(1200,475);
        glVertex2f(1200,480);
        glVertex2f(1010,480);
    glEnd();

        glBegin(GL_QUADS);
       glColor3ub(228,229,235);
        glVertex2f(1025,485);
        glVertex2f(1200,485);
        glVertex2f(1200,515);
        glVertex2f(1055,515);
    glEnd();

            glBegin(GL_QUADS);
       glColor3ub(145,167,247);
        glVertex2f(1040,490);
        glVertex2f(1080,490);
        glVertex2f(1080,510);
        glVertex2f(1060,510);
    glEnd();

                glBegin(GL_QUADS);
       glColor3ub(145,167,247);
        glVertex2f(1090,490);
        glVertex2f(1110,490);
        glVertex2f(1110,510);
        glVertex2f(1090,510);
    glEnd();

                    glBegin(GL_QUADS);
       glColor3ub(145,167,247);
        glVertex2f(1120,490);
        glVertex2f(1140,490);
        glVertex2f(1140,510);
        glVertex2f(1120,510);
    glEnd();

                        glBegin(GL_QUADS);
       glColor3ub(145,167,247);
        glVertex2f(1150,490);
        glVertex2f(1170,490);
        glVertex2f(1170,510);
        glVertex2f(1150,510);
    glEnd();


        glBegin(GL_QUADS);
       glColor3ub(145,167,247);
        glVertex2f(1180,490);
        glVertex2f(1195,490);
        glVertex2f(1195,510);
        glVertex2f(1180,510);
    glEnd();

    }
    glPopMatrix();

    ///       KALA JOG

        glBegin(GL_QUADS);
       glColor3ub(1,1,1);
        glVertex2f(1036,473);
        glVertex2f(1055,473);
        glVertex2f(1055,482);
        glVertex2f(1036,482);
    glEnd();
    }
    glPopMatrix();
}



////////////////////////////DISPLAY & CONTROL UNIT /////////////////////////

void specialKeysM(int key, int x, int y) {
    switch (key) {
      case GLUT_KEY_UP:
          if(currentScreen == 2)
          speed+=0.05f;
          updateF(0);
          break;
      case GLUT_KEY_DOWN:
          if(currentScreen == 2)
          speed-=0.05f;
          updateF(0);
          break;
    }
}

void mouseM(int button, int state, int x, int y) {
    if(currentScreen == 2)
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // Pause/resume
      paused = !paused;         // Toggle state
      if (paused) {
         savedSpeed = speed;  // Save parameters for restore later
         speed = 0;            // Stop movement

      } else {
         speed = savedSpeed;  // Restore parameters
      }
   }
}


void display2()
{
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen
        DrawMiddlePartM();
    drawSkyM();
    drawSunM();
    drawGroundM();
    drawBuildings3();
    drawRoadM();
    drawRoad2M();
    drawRoadLinesM();
    drawRoadLines2M();
    drawBuildings1();
    drawBuildings2();
 drawMelarChakka();
 drawRailBridgeM();
 drawPlaneM();
 drawRailBridgeLinesM();
 drawTrainM();
 Noksha();
 drawMegh();
 drawMegh2();
//    drawBus1();
    glFlush();
    glutSwapBuffers();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////UPDATE 3////////////////////////////////////////////////////
float cloud3 = 0.0f;
bool isCloudMoving = false;
bool isBoatMoving = false;
float boat3= 0.0f;
float boat1X = 0.0f;
//bool isLamppostOn = false;
bool isNight = true;


void update2_11(int value) {
   if (isCloudMoving) {
        cloud3 += 0.5f;
        if (cloud3 >= 1300) {
            cloud3 = -100;
        }
    }
     if(isBoatMoving){
        boat3 +=0.2f;
        if(boat3 >= 330){
            boat3 = 330;
        }
    }
    if (isRaining)
        updateRain();
    glutPostRedisplay();
    glutTimerFunc(20, update2_11, 0);
}

//////////////////////////////////////// DRAW ITEM////////////////////////////////////////////////////////////

void drawBridge()
{
    // Road surface
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray for road
    glVertex2f(0, 250-80);
    glVertex2f(1200, 250-80);
    glVertex2f(1200, 450-80);
    glVertex2f(0, 450-80);
    glEnd();

    // Road divider
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f); // Yellow for divider
    glVertex2f(0, 345-80);
    glVertex2f(1200, 345-80);
    glVertex2f(1200, 350-80);
    glVertex2f(0, 350-80);
    glEnd();

    // Leftmost pillar
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f); // Light gray for pillars
    glVertex2f(100, 0-80);
    glVertex2f(140, 0-80);
    glVertex2f(140, 250-80);
    glVertex2f(100, 250-80);
    glEnd();

    // Second pillar
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f); // Light gray for pillars
    glVertex2f(340, 100-80);
    glVertex2f(380, 100-80);
    glVertex2f(380, 250-80);
    glVertex2f(340, 250-80);
    glEnd();

    // Middle pillar
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f); // Light gray for pillars
    glVertex2f(580, 0-80);
    glVertex2f(620, 0-80);
    glVertex2f(620, 250-80);
    glVertex2f(580, 250-80);
    glEnd();

    // Fourth pillar
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f); // Light gray for pillars
    glVertex2f(820, 100-80);
    glVertex2f(860, 100-80);
    glVertex2f(860, 250-80);
    glVertex2f(820, 250-80);
    glEnd();

    // Rightmost pillar
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f); // Light gray for pillars
    glVertex2f(1060, 0-80);
    glVertex2f(1100, 0-80);
    glVertex2f(1100, 250-80);
    glVertex2f(1060, 250-80);
    glEnd();

    // Safety rails on the bridge
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.6f, 0.6f); // Gray for safety rails
    glVertex2f(0, 450-80);
    glVertex2f(1200, 450-80);
    glVertex2f(1200, 460-80);
    glVertex2f(0, 460-80);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.6f, 0.6f); // Gray for safety rails
    glVertex2f(0, 240-80);
    glVertex2f(1200, 240-80);
    glVertex2f(1200, 250-80);
    glVertex2f(0, 250-80);
    glEnd();
}

void drawSky3()
{

    glBegin(GL_QUADS);
        glColor3f(0.165, 0.165, 0.208);
        glVertex2f(0,600);
        glVertex2f(1200,600);
        glVertex2f(1200,800);
        glVertex2f(0,800);
    glEnd();

}

void drawMoon3(){

    drawCircle(900, 750, 35,0.9,0.9,0.9);  //X,Y,Radius,r,g,b

}
void drawSun3()
{
    drawCircle(800, 750, 35,1.0,1.0,0.0);  //X,Y,Radius,r,g,b
}
void drawCloud3() {
    drawCircle(60 + cloud3, 725, 20, 1.0, 1.0, 1.0);
    drawCircle(75 + cloud3, 730, 20, 1.0, 1.0, 1.0); // Part 1
    drawCircle(130 + cloud3, 720, 20, 1.0, 1.0, 1.0);
    drawCircle(155 + cloud3, 725, 20, 1.0, 1.0, 1.0); // Part 2
    drawCircle(200 + cloud3, 720, 20, 1.0, 1.0, 1.0);
    drawCircle(215 + cloud3, 725, 20, 1.0, 1.0, 1.0); // Part 3
    drawCircle(400, 750, 20,0.6,0.6,0.6);
    drawCircle(420, 755, 20,0.6,0.6,0.6);//1
    drawCircle(470, 750, 20,0.6,0.6,0.6);
    drawCircle(490, 760, 20,0.6,0.6,0.6);//2
    drawCircle(550, 745, 20,0.6,0.6,0.6);
    drawCircle(570, 750, 20,0.6,0.6,0.6);//3
    drawCircle(940, 750, 20,0.6,0.6,0.6);
    drawCircle(960,755, 20,0.6,0.6,0.6);//4
    drawCircle(1000, 745, 20,0.6,0.6,0.6);
    drawCircle(1020, 750, 20,0.6,0.6,0.6);//5

}
void drawGround3()
{
    glBegin(GL_TRIANGLES);
        glColor3f(0.124, 0.252, 0.0);
        glVertex2f(1200,0);
        glVertex2f(1200,600);
        glVertex2f(700,600);
    glEnd();


    glBegin(GL_TRIANGLES);
        glColor3f(0.124, 0.252, 0.0);
        glVertex2f(0,0);
        glVertex2f(0,600);
        glVertex2f(400,600);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0,0,0);
    glVertex2f(350,510);
    glVertex2f(350,570);
    glVertex2f(347,570);
    glVertex2f(347,510);
    glEnd();


}

void drawBoat()
{
    // Boat base (rectangle)
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.2f, 0.0f); // 1 Brownish color for the boat base
    glVertex2f(450 ,600);
    glVertex2f(500  ,600);
    glVertex2f(490  ,580);
    glVertex2f(460 ,580);
    glEnd();

    // Boat sail (triangle)
    glBegin(GL_TRIANGLES);
    glColor3f(0.5f, 0.5f, 0.5f); //  sail
     glVertex2f(460 ,600);
    glVertex2f(470 ,620);
    glVertex2f(480 ,600);
    glEnd();

    glBegin(GL_QUADS);//2
    glColor3f(0.0f, 0.0f, 0.0f); //  boat base
    glVertex2f(350 +boat3,530);
    glVertex2f(430+boat3,530);
    glVertex2f(410+boat3,510);
    glVertex2f(370+boat3,510);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.725, 0.984, 0.753); // White sail
    glVertex2f(370+boat3,530);
    glVertex2f(380+boat3,550);
    glVertex2f(390+boat3,530);
    glEnd();
    glBegin(GL_QUADS);
     glColor3f(0.5f, 0.0f, 0.0f); //  boat base
    glVertex2f(380+boat3,550);
    glVertex2f(400+boat3,550);
    glVertex2f(410+boat3,530);
    glVertex2f(390+boat3,530);
    glEnd();
      glBegin(GL_QUADS);
     glColor3f(0.5f, 0.0f, 0.0f); //  boat base
    glVertex2f(415+boat3,550);
    glVertex2f(420+boat3,550);
    glVertex2f(418+boat3,510);
    glVertex2f(420+boat3,510);
    glEnd();


}

void drawRiver() {
    // River (existing code)
    glBegin(GL_QUADS);
    glColor3f(0.11f, 0.35f, 0.43f); // Blue color for the river
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1200.0f, 0.0f);
    glVertex2f(700.0f, 600.0f);
    glVertex2f(400.0f, 600.0f);
    glEnd();


}

void drawHills()
{
   glBegin(GL_TRIANGLES);
    glColor3f(0.1, 0.26, 0.09);
    glVertex2f(0, 600);   // Left base
    glVertex2f(60, 720); // Peak
    glVertex2f(120, 600); // Right base
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.1, 0.35, 0.09);
    glVertex2f(80, 600);   // Left base
    glVertex2f(120, 720); // Peak
    glVertex2f(160, 600); // Right base
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.06, 0.38, 0.04);
    glVertex2f(140, 600);   // Left base
    glVertex2f(200, 720); // Peak
    glVertex2f(260, 600); // Right base
    glEnd();
}
void drawHomeUp11()
{
    glBegin(GL_QUADS);//1
        glColor3f(0.4, 0.3, 0.4);
        glVertex2f(1100,540);
        glVertex2f(1176,540);
        glVertex2f(1176,690);
        glVertex2f(1100,690);
    glEnd();

    ///Windows
    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.7, 0.8, 0.0);
        glVertex2f(1110,550);
        glVertex2f(1130,550);
        glVertex2f(1130,580);
        glVertex2f(1110,580);
    glEnd();
    glPopMatrix();
    }

    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.7, 0.8, 0.05);
        glVertex2f(1145,550);
        glVertex2f(1165,550);
        glVertex2f(1165,580);
        glVertex2f(1145,580);
    glEnd();
    glPopMatrix();
    }
     glBegin(GL_QUADS);//2
        glColor3f(0.4, 0.3, 0.4);
        glVertex2f(1000,540);
        glVertex2f(1076,540);
        glVertex2f(1076,690);
        glVertex2f(1000,690);
    glEnd();
      ///Windows
    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.7, 0.8, 0.05);
        glVertex2f(1010,550);
        glVertex2f(1030,550);
        glVertex2f(1030,580);
        glVertex2f(1010,580);
    glEnd();
    glPopMatrix();
    }

    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.7, 0.8, 0.0);
        glVertex2f(1045,550);
        glVertex2f(1065,550);
        glVertex2f(1065,580);
        glVertex2f(1045,580);
    glEnd();
    glPopMatrix();
    }
    glBegin(GL_QUADS);//3
        glColor3f(0.4, 0.3, 0.4);
        glVertex2f(900,530);
        glVertex2f(976,530);
        glVertex2f(976,680);
        glVertex2f(900,680);
    glEnd();
        ///Windows
    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.7, 0.8, 0.0);
        glVertex2f(910,540);
        glVertex2f(930,540);
        glVertex2f(930,570);
        glVertex2f(910,570);
    glEnd();
    glPopMatrix();
    }

    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.7, 0.8, 0.0);
        glVertex2f(945,540);
        glVertex2f(965,540);
        glVertex2f(965,570);
        glVertex2f(945,570);
    glEnd();
    glPopMatrix();
    }
    glBegin(GL_TRIANGLES);//4
    glColor3f(0.1,0.1,0.1);
    glVertex2f(0,580);
    glVertex2f(10,600);
    glVertex2f(20,580);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.698, 0.255, 0.145);
    glVertex2f(0,580);
    glVertex2f(20,580);
    glVertex2f(20,560);
    glVertex2f(0,560);
    glEnd();
    glBegin(GL_QUADS);//roof
    glColor3f(0.3, 0.3, 0.3);
    glVertex2f(10,600);
    glVertex2f(20,580);
    glVertex2f(45,580);
    glVertex2f(30,600);
    glEnd();
    glBegin(GL_QUADS);//front
    glColor3f(0.725, 0.984, 0.753);
    glVertex2f(20,580);
    glVertex2f(20,560);
    glVertex2f(45,560);
    glVertex2f(45,580);
    glEnd();
     glBegin(GL_TRIANGLES);//5
    glColor3f(0.15,0.15,0.15);
    glVertex2f(50,580);
    glVertex2f(60,600);
    glVertex2f(70,580);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.608, 0.753, 0.388);
    glVertex2f(50,580);
    glVertex2f(70,580);
    glVertex2f(70,560);
    glVertex2f(50,560);
    glEnd();
    glBegin(GL_QUADS);//roof
    glColor3f(0.3, 0.3, 0.3);
    glVertex2f(60,600);
    glVertex2f(70,580);
    glVertex2f(115,580);
    glVertex2f(100,600);
    glEnd();
    glBegin(GL_QUADS);//front
    glColor3f(0.725, 0.984, 0.753);
    glVertex2f(70,580);
    glVertex2f(70,560);
    glVertex2f(115,560);
    glVertex2f(115,580);
    glEnd();
     glBegin(GL_QUADS);//gate
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(90,575);
    glVertex2f(90,560);
    glVertex2f(100,560);
    glVertex2f(100,575);
    glEnd();

    glBegin(GL_QUADS);//6 front
    glColor3f(0.725, 0.984, 0.753);
    glVertex2f(120,580);
    glVertex2f(160,580);
    glVertex2f(160,560);
    glVertex2f(120,560);
    glEnd();
    glBegin(GL_QUADS);//roof
    glColor3f(0.3, 0.3, 0.3);
    glVertex2f(120,580);
    glVertex2f(160,580);
    glVertex2f(170,600);
    glVertex2f(130,600);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.15,0.15,0.15);
    glVertex2f(160,580);
    glVertex2f(170,600);
    glVertex2f(180,580);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.608, 0.753, 0.388);
    glVertex2f(160,580);
    glVertex2f(160,560);
    glVertex2f(180,560);
    glVertex2f(180,580);
    glEnd();
    glBegin(GL_QUADS);// door
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(140,575);
    glVertex2f(150,575);
    glVertex2f(150,560);
    glVertex2f(140,560);
    glEnd();


}
void drawRoadup()
{
    // Draw road
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray for the road
    glVertex2f(840, 520);
    glVertex2f(820, 500);
    glVertex2f(1200, 500);
    glVertex2f(1200, 520);
    glEnd();

    // Draw ramp
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray for the ramp
    glVertex2f(860, 520);
    glVertex2f(900, 600);
    glVertex2f(880, 600);
    glVertex2f(820, 500);
    glEnd();

    glBegin(GL_QUADS);//left
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray for the ramp
    glVertex2f(0, 550);
    glVertex2f(200, 550);
    glVertex2f(200, 530);
    glVertex2f(0, 530);
    glEnd();
    glBegin(GL_QUADS);//left
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray for the ramp
    glVertex2f(200, 550);
    glVertex2f(260, 600);
    glVertex2f(280, 600);
    glVertex2f(200, 530);
    glEnd();

    // Draw car 1
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 1.0f); // Red car body
    glVertex2f(850, 520);
    glVertex2f(850, 540);
    glVertex2f(890, 540);
    glVertex2f(890, 520);
    glEnd();

    drawCircle(860,520,5,1.0f,1.0f,1.0f);
    drawCircle(880,520,5,1.0f,1.0f,1.0f);//w for c1

    // Draw car 2 (spaced from car 1)
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue car body
    glVertex2f(970, 520); // X coordinates shifted to create a gap
    glVertex2f(970, 540);
    glVertex2f(1010, 540);
    glVertex2f(1010, 520);
    glEnd();
    drawCircle(980,520,5,1.0f,1.0f,1.0f);
    drawCircle(1000,520,5,1.0f,1.0f,1.0f);//w for 2c

}

void drawDay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glBegin(GL_QUADS);//sky//////////
    glColor3f(0.16, 0.61, 0.92);
    glVertex2f(0,600);
    glVertex2f(1200,600);
    glVertex2f(1200,800);
    glVertex2f(0,800);
    glEnd();

     glBegin(GL_QUADS);//river////////////
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for the river
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1200.0f, 0.0f);
    glVertex2f(700.0f, 600.0f);
    glVertex2f(400.0f, 600.0f);
    glEnd();

    drawCircle(800, 750, 35,1.0,1.0,0.0);  //X,Y,Radius,r,g,b//sun//////////////

    drawCircle(60 + cloud3, 725, 20, 1.0, 1.0, 1.0);//cloud////////////
    drawCircle(75 + cloud3, 730, 20, 1.0, 1.0, 1.0); // Part 1
    drawCircle(130 + cloud3, 720, 20, 1.0, 1.0, 1.0);
    drawCircle(155 + cloud3, 725, 20, 1.0, 1.0, 1.0); // Part 2
    drawCircle(200 + cloud3, 720, 20, 1.0, 1.0, 1.0);
    drawCircle(215 + cloud3, 725, 20, 1.0, 1.0, 1.0); // Part 3
    drawCircle(940, 750, 20, 0.9,0.9,0.9);
    drawCircle(960,755, 20,0.9,0.9,0.9);//4
    drawCircle(1000, 745, 20,0.9,0.9,0.9);
    drawCircle(1020, 750, 20,0.9,0.9,0.9);//5

     glBegin(GL_TRIANGLES);//ground/////////////
        glColor3f(0.608, 0.463, 0.325);
        glVertex2f(1200,0);
        glVertex2f(1200,600);
        glVertex2f(700,600);
    glEnd();


    glBegin(GL_TRIANGLES);
        glColor3f(0.608, 0.463, 0.32);
        glVertex2f(0,0);
        glVertex2f(0,600);
        glVertex2f(400,600);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0,0,0);
    glVertex2f(350,510);
    glVertex2f(350,570);
    glVertex2f(347,570);
    glVertex2f(347,510);
    glEnd();

    // Road surface
    glBegin(GL_QUADS);//bridge///////////////
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray for road
    glVertex2f(0, 250);
    glVertex2f(1200, 250);
    glVertex2f(1200, 450);
    glVertex2f(0, 450);
    glEnd();

    // Road divider
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f); // Yellow for divider
    glVertex2f(0, 345);
    glVertex2f(1200, 345);
    glVertex2f(1200, 350);
    glVertex2f(0, 350);
    glEnd();

    // Leftmost pillar
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f); // Light gray for pillars
    glVertex2f(100, 0);
    glVertex2f(140, 0);
    glVertex2f(140, 250);
    glVertex2f(100, 250);
    glEnd();

    // Second pillar
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f); // Light gray for pillars
    glVertex2f(340, 100);
    glVertex2f(380, 100);
    glVertex2f(380, 250);
    glVertex2f(340, 250);
    glEnd();

    // Middle pillar
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f); // Light gray for pillars
    glVertex2f(580, 0);
    glVertex2f(620, 0);
    glVertex2f(620, 250);
    glVertex2f(580, 250);
    glEnd();

    // Fourth pillar
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f); // Light gray for pillars
    glVertex2f(820, 100);
    glVertex2f(860, 100);
    glVertex2f(860, 250);
    glVertex2f(820, 250);
    glEnd();

    // Rightmost pillar
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f); // Light gray for pillars
    glVertex2f(1060, 0);
    glVertex2f(1100, 0);
    glVertex2f(1100, 250);
    glVertex2f(1060, 250);
    glEnd();

    // Safety rails on the bridge
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.6f, 0.6f); // Gray for safety rails
    glVertex2f(0, 450);
    glVertex2f(1200, 450);
    glVertex2f(1200, 460);
    glVertex2f(0, 460);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.6f, 0.6f); // Gray for safety rails
    glVertex2f(0, 240);
    glVertex2f(1200, 240);
    glVertex2f(1200, 250);
    glVertex2f(0, 250);
    glEnd();

    glBegin(GL_TRIANGLES);//hills////////////
    glColor3f(0.19, 0.26, 0.15);
    glVertex2f(0, 600);   // Left base
    glVertex2f(60, 720); // Peak
    glVertex2f(120, 600); // Right base
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.18, 0.5, 0.02);
    glVertex2f(80, 600);   // Left base
    glVertex2f(120, 720); // Peak
    glVertex2f(160, 600); // Right base
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.18, 0.5, 0.02);
    glVertex2f(140, 600);   // Left base
    glVertex2f(200, 720); // Peak
    glVertex2f(260, 600); // Right base
    glEnd();

     glBegin(GL_QUADS);//1//home///////////
        glColor3f(0.0, 1.3, 1.3);
        glVertex2f(1100,540);
        glVertex2f(1176,540);
        glVertex2f(1176,690);
        glVertex2f(1100,690);
    glEnd();

    ///Windows
    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(1110,550);
        glVertex2f(1130,550);
        glVertex2f(1130,580);
        glVertex2f(1110,580);
    glEnd();
    glPopMatrix();
    }

    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(1145,550);
        glVertex2f(1165,550);
        glVertex2f(1165,580);
        glVertex2f(1145,580);
    glEnd();
    glPopMatrix();
    }
     glBegin(GL_QUADS);//2
        glColor3f(0.0, 1.3, 1.3);
        glVertex2f(1000,540);
        glVertex2f(1076,540);
        glVertex2f(1076,690);
        glVertex2f(1000,690);
    glEnd();
      ///Windows
    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(1010,550);
        glVertex2f(1030,550);
        glVertex2f(1030,580);
        glVertex2f(1010,580);
    glEnd();
    glPopMatrix();
    }

    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(1045,550);
        glVertex2f(1065,550);
        glVertex2f(1065,580);
        glVertex2f(1045,580);
    glEnd();
    glPopMatrix();
    }
    glBegin(GL_QUADS);//3
        glColor3f(0.0, 1.3, 1.3);
        glVertex2f(900,530);
        glVertex2f(976,530);
        glVertex2f(976,680);
        glVertex2f(900,680);
    glEnd();
        ///Windows
    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(910,540);
        glVertex2f(930,540);
        glVertex2f(930,570);
        glVertex2f(910,570);
    glEnd();
    glPopMatrix();
    }

    for (int i = 0; i <=100; i += 50){
    glPushMatrix();
    glTranslatef(0, i, 0);
    glBegin(GL_QUADS);
        glColor3f(0.102, 0.178, 0.255);
        glVertex2f(945,540);
        glVertex2f(965,540);
        glVertex2f(965,570);
        glVertex2f(945,570);
    glEnd();
    glPopMatrix();
    }
    glBegin(GL_TRIANGLES);//4
    glColor3f(0.2,0.2,0.2);
    glVertex2f(0,580);
    glVertex2f(10,600);
    glVertex2f(20,580);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.698, 0.255, 0.145);
    glVertex2f(0,580);
    glVertex2f(20,580);
    glVertex2f(20,560);
    glVertex2f(0,560);
    glEnd();
    glBegin(GL_QUADS);//roof
    glColor3f(0.725, 0.984, 0.753);
    glVertex2f(10,600);
    glVertex2f(20,580);
    glVertex2f(45,580);
    glVertex2f(30,600);
    glEnd();
    glBegin(GL_QUADS);//front
    glColor3f(0.0f,1.0f,0.0f);
    glVertex2f(20,580);
    glVertex2f(20,560);
    glVertex2f(45,560);
    glVertex2f(45,580);
    glEnd();
     glBegin(GL_TRIANGLES);//5
    glColor3f(0.2,0.2,0.2);
    glVertex2f(50,580);
    glVertex2f(60,600);
    glVertex2f(70,580);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.698, 0.255, 0.145);
    glVertex2f(50,580);
    glVertex2f(70,580);
    glVertex2f(70,560);
    glVertex2f(50,560);
    glEnd();
    glBegin(GL_QUADS);//roof
    glColor3f(0.725, 0.984, 0.753);
    glVertex2f(60,600);
    glVertex2f(70,580);
    glVertex2f(115,580);
    glVertex2f(100,600);
    glEnd();
    glBegin(GL_QUADS);//front
    glColor3f(0.0f,1.0f,0.0f);
    glVertex2f(70,580);
    glVertex2f(70,560);
    glVertex2f(115,560);
    glVertex2f(115,580);
    glEnd();
     glBegin(GL_QUADS);//gate
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(90,575);
    glVertex2f(90,560);
    glVertex2f(100,560);
    glVertex2f(100,575);
    glEnd();

    glBegin(GL_QUADS);//6 front
    glColor3f(0.0f,1.0f,0.0f);
    glVertex2f(120,580);
    glVertex2f(160,580);
    glVertex2f(160,560);
    glVertex2f(120,560);
    glEnd();
    glBegin(GL_QUADS);//roof
    glColor3f(0.725, 0.984, 0.753);
    glVertex2f(120,580);
    glVertex2f(160,580);
    glVertex2f(170,600);
    glVertex2f(130,600);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.2,0.2,0.2);
    glVertex2f(160,580);
    glVertex2f(170,600);
    glVertex2f(180,580);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.698, 0.255, 0.145);
    glVertex2f(160,580);
    glVertex2f(160,560);
    glVertex2f(180,560);
    glVertex2f(180,580);
    glEnd();
    glBegin(GL_QUADS);// door
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(140,575);
    glVertex2f(150,575);
    glVertex2f(150,560);
    glVertex2f(140,560);
    glEnd();

    glBegin(GL_QUADS);//road////////////////
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray for the road
    glVertex2f(840, 520);
    glVertex2f(820, 500);
    glVertex2f(1200, 500);
    glVertex2f(1200, 520);
    glEnd();

    // Draw ramp
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray for the ramp
    glVertex2f(860, 520);
    glVertex2f(900, 600);
    glVertex2f(880, 600);
    glVertex2f(820, 500);
    glEnd();

    glBegin(GL_QUADS);//left
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray for the ramp
    glVertex2f(0, 550);
    glVertex2f(200, 550);
    glVertex2f(200, 530);
    glVertex2f(0, 530);
    glEnd();
    glBegin(GL_QUADS);//left
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray for the ramp
    glVertex2f(200, 550);
    glVertex2f(260, 600);
    glVertex2f(280, 600);
    glVertex2f(200, 530);
    glEnd();

    // Draw car 1
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 1.0f); // Red car body
    glVertex2f(850, 520);
    glVertex2f(850, 540);
    glVertex2f(890, 540);
    glVertex2f(890, 520);
    glEnd();

    drawCircle(860,520,5,1.0f,1.0f,1.0f);
    drawCircle(880,520,5,1.0f,1.0f,1.0f);//w for c1

    // Draw car 2 (spaced from car 1)
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue car body
    glVertex2f(970, 520); // X coordinates shifted to create a gap
    glVertex2f(970, 540);
    glVertex2f(1010, 540);
    glVertex2f(1010, 520);
    glEnd();
    drawCircle(980,520,5,1.0f,1.0f,1.0f);
    drawCircle(1000,520,5,1.0f,1.0f,1.0f);//w for 2c////


    glBegin(GL_QUADS);//boat///////////
    glColor3f(0.4f, 0.2f, 0.0f); // 1 Brownish color for the boat base
    glVertex2f(450 ,600);
    glVertex2f(500  ,600);
    glVertex2f(490  ,580);
    glVertex2f(460 ,580);
    glEnd();

    // Boat sail (triangle)
    glBegin(GL_TRIANGLES);
    glColor3f(0.725, 0.984, 0.753); //  sail
     glVertex2f(460 ,600);
    glVertex2f(470 ,620);
    glVertex2f(480 ,600);
    glEnd();
    glBegin(GL_QUADS);//2
    glColor3f(0.0f, 0.0f, 0.0f); //  boat base
    glVertex2f(350 +boat3,530);
    glVertex2f(430+boat3,530);
    glVertex2f(410+boat3,510);
    glVertex2f(370+boat3,510);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.725, 0.984, 0.753); // White sail
    glVertex2f(370+boat3,530);
    glVertex2f(380+boat3,550);
    glVertex2f(390+boat3,530);
    glEnd();
    glBegin(GL_QUADS);
     glColor3f(0.5f, 0.0f, 0.0f); //  boat base
    glVertex2f(380+boat3,550);
    glVertex2f(400+boat3,550);
    glVertex2f(410+boat3,530);
    glVertex2f(390+boat3,530);
    glEnd();
      glBegin(GL_QUADS);
     glColor3f(0.5f, 0.0f, 0.0f); //  boat base
    glVertex2f(415+boat3,550);
    glVertex2f(420+boat3,550);
    glVertex2f(418+boat3,510);
    glVertex2f(420+boat3,510);
    glEnd();

}

void drawSky6()
{

    glBegin(GL_QUADS);
        glColor3f(0.165, 0.165, 0.208);
        glVertex2f(0,600);
        glVertex2f(1200,600);
        glVertex2f(1200,800);
        glVertex2f(0,800);
    glEnd();

}

void drawMoon6(){

    drawCircle(900, 750, 35,0.9,0.9,0.9);  //X,Y,Radius,r,g,b

}
void drawSun6()
{
    drawCircle(800, 750, 35,1.0,1.0,0.0);  //X,Y,Radius,r,g,b
}
void drawCloud6() {
    drawCircle(60 + cloud3, 725, 20, 1.0, 1.0, 1.0);
    drawCircle(75 + cloud3, 730, 20, 1.0, 1.0, 1.0); // Part 1
    drawCircle(130 + cloud3, 720, 20, 1.0, 1.0, 1.0);
    drawCircle(155 + cloud3, 725, 20, 1.0, 1.0, 1.0); // Part 2
    drawCircle(200 + cloud3, 720, 20, 1.0, 1.0, 1.0);
    drawCircle(215 + cloud3, 725, 20, 1.0, 1.0, 1.0); // Part 3
    drawCircle(400, 750, 20,0.6,0.6,0.6);
    drawCircle(420, 755, 20,0.6,0.6,0.6);//1
    drawCircle(470, 750, 20,0.6,0.6,0.6);
    drawCircle(490, 760, 20,0.6,0.6,0.6);//2
    drawCircle(550, 745, 20,0.6,0.6,0.6);
    drawCircle(570, 750, 20,0.6,0.6,0.6);//3
    drawCircle(940, 750, 20,0.6,0.6,0.6);
    drawCircle(960,755, 20,0.6,0.6,0.6);//4
    drawCircle(1000, 745, 20,0.6,0.6,0.6);
    drawCircle(1020, 750, 20,0.6,0.6,0.6);//5

}

void initScreen3() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,1200,0,800);
}


void display3()
{
    drawSky6();
    drawSun6();
    drawMoon6();
    drawCloud6();
    drawRiver();
    drawBoat();
    drawGround3();
    drawBridge();
    drawHills();
    drawRoadup();
    drawHomeUp11();
}
////////////////////////////////////////////////////////////// DISPLAY 4 //////////////////////////////////////////////////////////////////////////////////////
float cloud4 = 0.0f;
float boat4 = 0.0f;


void initScreen4()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 800); // Unified viewport for all screens
}



void Rectangle(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void Boat4() {
    glPushMatrix();
    glTranslatef(boat4, 0.0f, 0.0f); // Ensure boat4 is within 0-1200
    int x = 850;
    int y = 450;

    if (isDay) {
        // Boat Hull
        glColor3ub(139, 69, 19);
        glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 20, y - 10);
        glVertex2f(x + 100, y - 10);
        glVertex2f(x + 120, y);
        glEnd();

        // Boat Deck
        glColor3ub(160, 82, 45);
        glBegin(GL_POLYGON);
        glVertex2f(x + 20, y);
        glVertex2f(x + 100, y);
        glVertex2f(x + 80, y + 10);
        glVertex2f(x + 40, y + 10);
        glEnd();
    } else {
        // Night Colors
        glColor3ub(109, 29, 19);
        glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 20, y - 10);
        glVertex2f(x + 100, y - 10);
        glVertex2f(x + 120, y);
        glEnd();

        glColor3ub(100, 42, 25);
        glBegin(GL_POLYGON);
        glVertex2f(x + 20, y);
        glVertex2f(x + 100, y);
        glVertex2f(x + 80, y + 10);
        glVertex2f(x + 40, y + 10);
        glEnd();
    glPopMatrix();
}
}

void Boat14() {

    int x = 850;
    int y = 400;

    if (isDay) {
        // Boat Hull
        glColor3ub(139, 69, 19);
        glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 20, y - 10);
        glVertex2f(x + 100, y - 10);
        glVertex2f(x + 120, y);
        glEnd();

        // Boat Deck
        glColor3ub(160, 82, 45);
        glBegin(GL_POLYGON);
        glVertex2f(x + 20, y);
        glVertex2f(x + 100, y);
        glVertex2f(x + 80, y + 10);
        glVertex2f(x + 40, y + 10);
        glEnd();
    } else {
        // Night Colors
        glColor3ub(109, 29, 19);
        glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 20, y - 10);
        glVertex2f(x + 100, y - 10);
        glVertex2f(x + 120, y);
        glEnd();

        glColor3ub(100, 42, 25);
        glBegin(GL_POLYGON);
        glVertex2f(x + 20, y);
        glVertex2f(x + 100, y);
        glVertex2f(x + 80, y + 10);
        glVertex2f(x + 40, y + 10);
        glEnd();
    }
}

void Tree4(float x, float y) {
    if(isDay) {

    glColor3ub(139, 69, 19); // Brown color
    Rectangle(x, y, 20, 50, 0.545, 0.271, 0.075);

    // tree leaves
    glColor3ub(34, 139, 34); // Green color
    drawCircle(x + 10, y + 60, 30, 0.0, 0.5, 0.0);
    drawCircle(x - 10, y + 50, 25, 0.0, 0.6, 0.0);
    drawCircle(x + 30, y + 50, 25, 0.0, 0.6, 0.0);
    drawCircle(x + 10, y + 80, 25, 0.0, 0.7, 0.0);
    }
    else {
        // tree trunk
    glColor3ub(139, 69, 19); // Brown color
    Rectangle(x, y, 20, 50, 0.545, 0.271, 0.075);

    // tree leaves
    glColor3ub(34, 139, 34); // Green color
    drawCircle(x + 10, y + 60, 30, 0.0, 0.2, 0.0);
    drawCircle(x - 10, y + 50, 25, 0.0, 0.3, 0.0);
    drawCircle(x + 30, y + 50, 25, 0.0, 0.3, 0.0);
    drawCircle(x + 10, y + 80, 25, 0.0, 0.4, 0.0);
    }
}

void Sky4()
{
    if (isDay) {
        glBegin(GL_QUADS);
        glColor3f(0.529, 0.808, 0.922); // Daytime sky
        glVertex2f(0, 500);
        glVertex2f(1200, 500);
        glVertex2f(1200, 800);
        glVertex2f(0, 800);
        glEnd();
        // Draw sun
        drawCircle(1000, 700, 50, 1.0, 1.0, 0.0);
    } else {
        glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.4); // Nighttime sky
        glColor3ub(9,53,74);
        glVertex2f(0, 500);
        glVertex2f(1200, 500);
        glVertex2f(1200, 800);
        glVertex2f(0, 800);
        glEnd();
        // Draw moon
        drawCircle(1000, 700, 35, 1.0, 1.0, 1.0);
    }
}

void Cloud4(){
    glPushMatrix();
    glTranslatef(cloud4,0.0,0.0);
    //cloud 1
    drawCircle(30, 700, 30,0.90,0.90,0.90);
    drawCircle(70, 700, 30,0.90,0.90,0.90);
    drawCircle(110, 700, 30,0.90,0.90,0.90);
    drawCircle(70, 720, 30,0.90,0.90,0.90);

    //cloud 2
    drawCircle(200, 750, 30,0.90,0.90,0.90);
    drawCircle(240, 750, 30,0.90,0.90,0.90);
    drawCircle(280, 750, 30,0.90,0.90,0.90);
    drawCircle(240, 770, 30,0.90,0.90,0.90);

    glPopMatrix();
}

void Mountains4()
{
    glBegin(GL_TRIANGLES);
    if (isDay) {
        glColor3f(0.0, 0.50, 0.0); //  for day
    } else {
        glColor3f(0.0, 0.20, 0.20); // for night
    }
        glVertex2f(0,500);
        glVertex2f(120,500);
        glVertex2f(60,560);
    glEnd();

    glBegin(GL_TRIANGLES);
    if (isDay) {
        glColor3f(0.0, 0.50, 0.0); //  for day
    } else {
        glColor3f(0.0, 0.20, 0.20); // for night
    }
        glVertex2f(120,500);
        glVertex2f(240,500);
        glVertex2f(180,560);
    glEnd();

    glBegin(GL_TRIANGLES);
    if (isDay) {
        glColor3f(0.0, 0.50, 0.0); //  for day
    } else {
        glColor3f(0.0, 0.20, 0.20); // for night
    }
        glVertex2f(240,500);
        glVertex2f(360,500);
        glVertex2f(300,560);
    glEnd();

    glBegin(GL_TRIANGLES);
    if (isDay) {
        glColor3f(0.0, 0.50, 0.0); // for day
    } else {
        glColor3f(0.0, 0.20, 0.20); // for night
    }
        glVertex2f(360,500);
        glVertex2f(480,500);
        glVertex2f(420,560);
    glEnd();

    glBegin(GL_TRIANGLES);
    if (isDay) {
        glColor3f(0.0, 0.50, 0.0); // for day
    } else {
        glColor3f(0.0, 0.20, 0.20); // for night
    }
        glVertex2f(480,500);
        glVertex2f(600,500);
        glVertex2f(540,560);
    glEnd();

    glBegin(GL_TRIANGLES);
    if (isDay) {
        glColor3f(0.0, 0.50, 0.0); // for day
    } else {
        glColor3f(0.0, 0.20, 0.20); // for night
    }
        glVertex2f(600,500);
        glVertex2f(720,500);
        glVertex2f(660,560);
    glEnd();

    glBegin(GL_TRIANGLES);
    if (isDay) {
        glColor3f(0.0, 0.50, 0.0); // for day
    } else {
        glColor3f(0.0, 0.20, 0.20); // for night
    }
        glVertex2f(720,500);
        glVertex2f(840,500);
        glVertex2f(780,560);
    glEnd();

    glBegin(GL_TRIANGLES);
    if (isDay) {
        glColor3f(0.0, 0.50, 0.0); // for day
    } else {
        glColor3f(0.0, 0.20, 0.20); // for night
    }
        glVertex2f(840,500);
        glVertex2f(960,500);
        glVertex2f(900,560);
    glEnd();

    glBegin(GL_TRIANGLES);
    if (isDay) {
        glColor3f(0.0, 0.50, 0.0); // for day
    } else {
        glColor3f(0.0, 0.20, 0.20); // for night
    }
        glVertex2f(960,500);
        glVertex2f(1080,500);
        glVertex2f(1020,560);
    glEnd();
    glBegin(GL_TRIANGLES);
    if (isDay) {
        glColor3f(0.0, 0.50, 0.0); //  for day
    } else {
        glColor3f(0.0, 0.20, 0.20); //  for night
    }
        glVertex2f(1080,500);
        glVertex2f(1200,500);
        glVertex2f(1140,560);
    glEnd();

}

void House(float x, float y) {
    if(isDay) {
        // house base
    Rectangle(x, y, 100, 100, 0.8, 0.4, 0.1); // Brown base

    // roof
    glColor3f(0.7, 0.0, 0.0); // Red color
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 100);
    glVertex2f(x + 100, y + 100);
    glVertex2f(x + 50, y + 150);
    glEnd();

    // door
    Rectangle(x + 40, y, 20, 50, 0.3, 0.2, 0.1); //  brown door

    // windows
    Rectangle(x + 10, y + 60, 20, 20, 0.5, 0.8, 1.0); // Blue window
    Rectangle(x + 70, y + 60, 20, 20, 0.5, 0.8, 1.0); // Blue window
    }
    else {
        // house base
    Rectangle(x, y, 100, 100, 0.4, 0.1, 0.0); // Brown base

    // roof
    glColor3f(0.3, 0.0, 0.0); // darker Red color
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 100);
    glVertex2f(x + 100, y + 100);
    glVertex2f(x + 50, y + 150);
    glEnd();

    //door
    Rectangle(x + 40, y, 20, 50, 0.1, 0.1, 0.0); // Dark brown door

    //windows
    Rectangle(x + 10, y + 60, 20, 20, 0.2, 0.3, 0.50); // Darker Blue window
    Rectangle(x + 70, y + 60, 20, 20, 0.2, 0.3, 0.50); // Darker Blue window
    }

}

void Ground4()
{
    glBegin(GL_QUADS);
    if (isDay) {
        glColor3f(0.196, 0.804, 0.196); //  for day
    } else {
        glColor3f(0.0, 0.392, 0.0); //  for night
    }
        glVertex2f(0,0);
        glVertex2f(1200,0);
        glVertex2f(1200,200);
        glVertex2f(0,200);
    glEnd();

    Tree4(50, 95);
    Tree4(100, 95);
    Tree4(250, 95);
    Tree4(400, 95);
    Tree4(600, 95);
    Tree4(700, 95);
    Tree4(150, 90);
    Tree4(200, 90);
    Tree4(450, 90);
    Tree4(500, 90);
    Tree4(550, 90);
    Tree4(300, 90);
    Tree4(350, 90);
    Tree4(650, 90);
    Tree4(50,0);
    Tree4(100,0);
    Tree4(250,0);
    Tree4(400,0);
    Tree4(600,0);
    Tree4(700,0);
    Tree4(150,0);
    Tree4(200,0);
    Tree4(500,0);
    Tree4(550,0);
    Tree4(300,0);
    Tree4(350,0);
    Tree4(450,0);
    Tree4(650,0);
}

void Upperside4()
{
    glBegin(GL_POLYGON);
    if (isDay) {
        glColor3f(0.196, 0.804, 0.196); // for day
    } else {
        glColor3f(0.0, 0.392, 0.0); //  for night
    }
        glVertex2f(0,375);
        glVertex2f(0,500);
        glVertex2f(800,500);
        glVertex2f(850,470);
        glVertex2f(780,420);
        glVertex2f(810,380);
        glVertex2f(730,375);
    glEnd();

    House(300,390);
    House(0,390);
    House(500,390);

    Tree4(100, 380);
    Tree4(250, 380);
    Tree4(400, 380);
    Tree4(600, 380);
    Tree4(700, 380);
    Tree4(150, 380);
    Tree4(200, 380);
    Tree4(450, 380);
    Tree4(650, 380);
}

void River4()
{
    glBegin(GL_POLYGON);
    if (isDay) {
        glColor3f(0.10, 0.04, 0.90); //  for day
    } else {
        glColor3f(0.0, 0.0, 0.40); //  for night
    }
        glVertex2f(1200,375);
        glVertex2f(730,375);
        glVertex2f(810,380);
        glVertex2f(780,420);
        glVertex2f(850,470);
        glVertex2f(800,500);
        glVertex2f(1200,500);
    glEnd();

}

void Highway4()
{
    glBegin(GL_QUADS);
    if (isDay) {
        glColor3f(0.0, 0.20, 0.20); //  for day
    } else {
        glColor3f(0.0, 0.10, 0.10); // for night
    }
        glVertex2f(0,200);
        glVertex2f(800,200);
        glVertex2f(800,375);
        glVertex2f(0,375);
    glEnd();

    glBegin(GL_QUADS);
    if (isDay) {
        glColor3f(1.0, 1.0, 0.0); // for day
    } else {
        glColor3f(0.40, 0.40, 0.0); //  for night
    }
        glVertex2f(500,280);
        glVertex2f(600,280);
        glVertex2f(600,288);
        glVertex2f(500,288);
    glEnd();

    glBegin(GL_QUADS);
    if (isDay) {
        glColor3f(1.0, 1.0, 0.0); //  for day
    } else {
        glColor3f(0.40, 0.40, 0.0); // for night
    }
        glVertex2f(100,280);
        glVertex2f(200,280);
        glVertex2f(200,288);
        glVertex2f(100,288);
    glEnd();

    glBegin(GL_QUADS);
    if (isDay) {
        glColor3f(1.0, 1.0, 0.0); // for day
    } else {
        glColor3f(0.40, 0.40, 0.0); // for night
    }
        glVertex2f(300,280);
        glVertex2f(400,280);
        glVertex2f(400,288);
        glVertex2f(300,288);
    glEnd();

    glBegin(GL_QUADS);
    if (isDay) {
        glColor3f(1.0, 1.0, 0.0); // for day
    } else {
        glColor3f(0.40, 0.40, 0.0); // for night
    }
        glVertex2f(700,280);
        glVertex2f(800,280);
        glVertex2f(800,288);
        glVertex2f(700,288);
    glEnd();

}

void Bridge4()
{
    glBegin(GL_QUADS);
    if (isDay) {
        glColor3f(0.0, 0.20, 0.20); //  for day
    } else {
        glColor3f(0.0, 0.10, 0.10); // for night
    }
        glVertex2f(800,200);
        glVertex2f(1200,200);
        glVertex2f(1200,375);
        glVertex2f(800,375);
    glEnd();

    glBegin(GL_QUADS);
    if (isDay) {
        glColor3f(1.0, 1.0, 0.0); // for day
    } else {
        glColor3f(0.40, 0.40, 0.0); // for night
    }
        glVertex2f(900,280);
        glVertex2f(1000,280);
        glVertex2f(1000,288);
        glVertex2f(900,288);
    glEnd();

    glBegin(GL_QUADS);
    if (isDay) {
        glColor3f(1.0, 1.0, 0.0); // for day
    } else {
        glColor3f(0.40, 0.40, 0.0); // for night
    }
        glVertex2f(1100,280);
        glVertex2f(1200,280);
        glVertex2f(1200,288);
        glVertex2f(1100,288);
    glEnd();


    glBegin(GL_POLYGON);
    if (isDay) {
        glColor3f(0.50, 0.50, 0.50); //  for day
    } else {
        glColor3f(0.30, 0.30, 0.30); // for night
    }
        glVertex2f(800,200);
        glVertex2f(840,150);
        glVertex2f(1200,150);
        glVertex2f(1200,200);
    glEnd();

    glBegin(GL_POLYGON);
    if (isDay) {
        glColor3f(0.50, 0.50, 0.50); //  for day
    } else {
        glColor3f(0.30, 0.30, 0.30); // for night
    }
        glVertex2f(1100,200);
        glVertex2f(1100,0);
        glVertex2f(1130,0);
        glVertex2f(1130,200);
    glEnd();

    glBegin(GL_POLYGON);
    if (isDay) {
        glColor3f(0.50, 0.50, 0.50); //  for day
    } else {
        glColor3f(0.30, 0.30, 0.30); // for night
    }
        glVertex2f(900,200);
        glVertex2f(900,0);
        glVertex2f(930,0);
        glVertex2f(930,200);
    glEnd();

    glBegin(GL_POLYGON);
    if (isDay) {
        glColor3f(0.50, 0.50, 0.50); //  for day
    } else {
        glColor3f(0.30, 0.30, 0.30); // for night
    }
        glVertex2f(1000,200);
        glVertex2f(1000,0);
        glVertex2f(1030,0);
        glVertex2f(1030,200);
    glEnd();

}

void RiverBottom()
{
    glBegin(GL_POLYGON);
    if (isDay) {
        glColor3f(0.10, 0.04, 0.90); //  for day
    } else {
        glColor3f(0.0, 0.0, 0.40); //  for night
    }
        glVertex2f(820,200);
        glVertex2f(770,175);
        glVertex2f(810,140);
        glVertex2f(790,110);
        glVertex2f(840,70);
        glVertex2f(800,40);
        glVertex2f(770,0);
        glVertex2f(1200,0);
        glVertex2f(1200,200);
    glEnd();
}

///////////////////UPDATE FOR SCREEN 4 ////////////////////////////////////
void update_44(int value) {
        cloud4 += 2.0f;
        if (cloud4 > 1200)
            cloud4 = -250;
        if (isRaining)
        updateRain();

    glutPostRedisplay();
    glutTimerFunc(20, update_44, 0);
}

void updateBoat(int value) {
    boat4 += 2.0f;   // Update boat position
    if (boat4 > 1200) boat4 = 0;
    glutPostRedisplay();
    glutTimerFunc(50, updateBoat, 0);
}




void display4()
{
    //glClear(GL_COLOR_BUFFER_BIT);
    //glLoadIdentity();
    Sky4();
    Cloud4();
    River4();
    RiverBottom();
    Ground4();
    Highway4();
    Bridge4();
    Mountains4();
    Upperside4();
    Boat4();
    //glFlush();
    //glutSwapBuffers();
}
/////////////////////////////////////////////////////////////////////// DISPLAY 5 ////////////////////////////////////////////////////////////

bool nightScenary = true;
GLfloat posCar1 = 0.0f;
GLfloat speedCar1 = 0.01f;
float posCar2 = 0.0f;
float speedCar12 = 0.01f;

//////////////////////// UPDATE FOR DISPLAY 5 ///////////////////////////////////////////
void updatedD5(int value)
{
    position += speedCar1; //right
    if (position > 1.0f)
    {
        position = -1.0f;
    }
    posCar2 +=speedCar12;
      if (posCar2 > 1.0f)
    {
        posCar2 = -1.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(100, updatedD5, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////

void drawSunDay()
{
    // Sun position and radius
    GLfloat centerX = 650.0f;
    GLfloat centerY = 500.0f;
    GLfloat radius = 50.0f;


    glColor3f(0.98, 1, 0.09);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);  // Center of the sun
    for (int i = 0; i <= 360; i++)
    {
        GLfloat angle = i * 3.14159f / 180.0f;  // Convert degrees to radians
        GLfloat x = centerX + radius * cos(angle);
        GLfloat y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawMoonNight()
{
    // Sun position and radius
    GLfloat centerX = 650.0f;
    GLfloat centerY = 500.0f;
    GLfloat radius = 50.0f;


    glColor3f(0.86, 0.81, 0.81);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);  // Center of the sun
    for (int i = 0; i <= 360; i++)
    {
        GLfloat angle = i * 3.14159f / 180.0f;  // Convert degrees to radians
        GLfloat x = centerX + radius * cos(angle);
        GLfloat y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}


void drawBenches()
{
    for (int i = 0; i < 11; i++)
    {
        float x = i * 70 + 35;

        // Bench seat
        glColor3f(0.55f, 0.27f, 0.07f);
        glBegin(GL_QUADS);
        glVertex2f(x - 15, 260);
        glVertex2f(x + 15, 260);
        glVertex2f(x + 15, 265);
        glVertex2f(x - 15, 265);
        glEnd();

        // Bench legs
        glBegin(GL_QUADS);
        glVertex2f(x - 12, 250);
        glVertex2f(x - 10, 250);
        glVertex2f(x - 10, 260);
        glVertex2f(x - 12, 260);
        glEnd();

        glBegin(GL_QUADS);
        glVertex2f(x + 10, 250);
        glVertex2f(x + 12, 250);
        glVertex2f(x + 12, 260);
        glVertex2f(x + 10, 260);
        glEnd();
    }
}

void drawDay3()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glScalef(1.5,1.35,1.);
    // Sky
    glColor3f(0.16, 0.61, 0.92);
    glBegin(GL_QUADS);
    glVertex2f(0, 600);
    glVertex2f(800, 600);
    glColor3f(0.38, 0.6, 0.71);
    glVertex2f(800, 300);
    glVertex2f(0, 300);
    glEnd();

    // Green hill 1
    glColor3f(0.19, 0.26, 0.15);
    glBegin(GL_POLYGON);
    glVertex2f(510.3506290118808, 405.3397492062188);
    glVertex2f(466.3406936760255, 428.2020532767929);
    glVertex2f(423.473873543699, 453.3505877544245);
    glVertex2f(387.4657446325447, 435.0607444979652);
    glVertex2f(343.4558092966896, 409.9122100203336);
    glVertex2f(315.4494868102362, 392.765481967403);
    glVertex2f(254.2928234214504, 360.1866986668348);
    glVertex2f(203.9957544661873, 327.6079153662666);
    glVertex2f(136.9233583757059, 290.2514402512625);
    glVertex2f(496.1425713386362, 275.5561088118699);
    glVertex2f(590.3686932588903, 371.0462931003575);
    glEnd();

    // Green hill 2
    glColor3f(0.18, 0.5, 0.02);
    glBegin(GL_POLYGON);
    glVertex2f(485.1471331652317, 325.5892345736768);
    glVertex2f(450.8281643952328, 311.4578944919123);
    glVertex2f(422.2290237535669, 302.7099220603438);
    glVertex2f(401.3684741090577, 292.9525681943637);
    glVertex2f(724.7069935989501, 287.9056610223049);
    glVertex2f(771.4750000600271, 354.5248356934802);
    glVertex2f(737.1560312900282, 382.1145949007345);
    glVertex2f(712.5944163860092, 402.975144545244);
    glVertex2f(682.3129733536572, 428.5461408836749);
    glVertex2f(673.9014614002261, 431.9107456650473);
    glVertex2f(666.1628704030695, 431.9107456650473);
    glVertex2f(658.76073988405, 426.5273780148514);
    glVertex2f(638.5731111958154, 411.3866564986752);
    glVertex2f(602.9083005132675, 389.1802649416168);
    glVertex2f(561.5236617023863, 366.3009524282838);
    glVertex2f(508.6993666348388, 339.7205746554412);
    glEnd();

    // Green hill 3
    glColor3f(0.42, 0.78, 0.28);
    glBegin(GL_POLYGON);
    glVertex2f(655.2613949161106, 331.8265529875498);
    glVertex2f(631.1674600767025, 327.1632107605675);
    glVertex2f(612.5140911687736, 323.6657040903308);
    glVertex2f(588.0315444771169, 315.1162433408633);
    glVertex2f(553.0564777747503, 296.4628744329342);
    glVertex2f(805.654181736287, 294.5198151716916);
    glVertex2f(803.7111224750445, 355.5318759747096);
    glVertex2f(781.560246896879, 362.526889315183);
    glVertex2f(764.4613253979442, 364.8585604286741);
    glVertex2f(738.812943149542, 364.4699485764256);
    glVertex2f(720, 360);
    glVertex2f(700.7289816291872, 348.9254744864848);
    glVertex2f(677.4122704942761, 338.4329544757746);
    glEnd();

    // Green ground
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, 300);
    glVertex2f(800, 300);
    glVertex2f(800, 250);
    glVertex2f(0, 250);
    glEnd();


    // Blue sky area
    glColor3f(0.529f, 0.808f, 0.922f);
    glBegin(GL_QUADS);
    glVertex2f(0, 250);
    glVertex2f(800, 250);
    glVertex2f(800, 200);
    glVertex2f(0, 200);
    glEnd();

    // Road
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0, 200);
    glVertex2f(800, 200);
    glVertex2f(800, 80);
    glVertex2f(0, 80);
    glEnd();

    //road middle
    glColor3f(0.87, 0.94, 0.33);
    glBegin(GL_QUADS);
    glVertex2f(0, 140);
    glVertex2f(90, 140);
    glVertex2f(90, 148);
    glVertex2f(0, 148);
    glEnd();

    glColor3f(0.87, 0.94, 0.33);
    glBegin(GL_QUADS);
    glVertex2f(210, 140);
    glVertex2f(300, 140);
    glVertex2f(300, 148);
    glVertex2f(210, 148);
    glEnd();

    glColor3f(0.87, 0.94, 0.33);
    glBegin(GL_QUADS);
    glVertex2f(410, 140);
    glVertex2f(500, 140);
    glVertex2f(500, 148);
    glVertex2f(410, 148);
    glEnd();

    glColor3f(0.87, 0.94, 0.33);
    glBegin(GL_QUADS);
    glVertex2f(610, 140);
    glVertex2f(700, 140);
    glVertex2f(700, 148);
    glVertex2f(610, 148);
    glEnd();

    glColor3f(0.87, 0.94, 0.33);
    glBegin(GL_QUADS);
    glVertex2f(790, 140);
    glVertex2f(900, 140);
    glVertex2f(900, 148);
    glVertex2f(790, 148);
    glEnd();

    //Road divider 1
    for (int i = 0; i < 16; i++)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(i * 50, 200);
        glVertex2f(i * 50 + 25, 200);
        glVertex2f(i * 50 + 25, 195);
        glVertex2f(i * 50, 195);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(i * 50 + 25, 200);
        glVertex2f(i * 50 + 50, 200);
        glVertex2f(i * 50 + 50, 195);
        glVertex2f(i * 50 + 25, 195);
        glEnd();
    }

    //Road divider 2
    for (int i = 0; i < 16; i++)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(i * 50, 86);
        glVertex2f(i * 50 + 25, 86);
        glVertex2f(i * 50 + 25, 81);
        glVertex2f(i * 50, 81);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(i * 50 + 25, 86);
        glVertex2f(i * 50 + 50, 86);
        glVertex2f(i * 50 + 50, 81);
        glVertex2f(i * 50 + 25, 81);
        glEnd();
    }

    //Lamp Post between Bench 1 and 2
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(67, 255);
    glVertex2f(73, 255);
    glVertex2f(73, 295);
    glVertex2f(67, 295);
    glEnd();

    //between Bench 2 and 3
    glBegin(GL_QUADS);
    glVertex2f(137, 255);
    glVertex2f(143, 255);
    glVertex2f(143, 295);
    glVertex2f(137, 295);
    glEnd();

    //between Bench 3 and 4
    glBegin(GL_QUADS);
    glVertex2f(207, 255);
    glVertex2f(213, 255);
    glVertex2f(213, 295);
    glVertex2f(207, 295);
    glEnd();

    //between Bench 4 and 5
    glBegin(GL_QUADS);
    glVertex2f(277, 255);
    glVertex2f(283, 255);
    glVertex2f(283, 295);
    glVertex2f(277, 295);
    glEnd();

    //between Bench 5 and 6
    glBegin(GL_QUADS);
    glVertex2f(347, 255);
    glVertex2f(353, 255);
    glVertex2f(353, 295);
    glVertex2f(347, 295);
    glEnd();

    //between Bench 6 and 7
    glBegin(GL_QUADS);
    glVertex2f(417, 255);
    glVertex2f(423, 255);
    glVertex2f(423, 295);
    glVertex2f(417, 295);
    glEnd();

    //between Bench 7 and 8
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(477, 255);
    glVertex2f(483, 255);
    glVertex2f(483, 295);
    glVertex2f(477, 295);
    glEnd();

    //between Bench 8 and 9
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(547, 255);
    glVertex2f(553, 255);
    glVertex2f(553, 295);
    glVertex2f(547, 295);
    glEnd();

    //between Bench 9 and 10
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(617, 255);
    glVertex2f(623, 255);
    glVertex2f(623, 295);
    glVertex2f(617, 295);
    glEnd();

    //between Bench 10 and 11
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(687, 255);
    glVertex2f(693, 255);
    glVertex2f(693, 295);
    glVertex2f(687, 295);
    glEnd();



    // Buildings
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(50.0f, 300.0f);
    glVertex2f(100.0f, 300.0f);
    glVertex2f(100.0f, 400.0f);
    glVertex2f(50.0f, 400.0f);
    glEnd();

    //1st window pink
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(60.0f, 310.0f);
    glVertex2f(70.0f, 310.0f);
    glVertex2f(70.0f, 320.0f);
    glVertex2f(60.0f, 320.0f);
    glEnd();

    //2nd p
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(80.0f, 310.0f);
    glVertex2f(90.0f, 310.0f);
    glVertex2f(90.0f, 320.0f);
    glVertex2f(80.0f, 320.0f);
    glEnd();

    //3rd p
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(60.0f, 330.0f);
    glVertex2f(70.0f, 330.0f);
    glVertex2f(70.0f, 340.0f);
    glVertex2f(60.0f, 340.0f);
    glEnd();

    //4th p
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(80.0f, 330.0f);
    glVertex2f(90.0f, 330.0f);
    glVertex2f(90.0f, 340.0f);
    glVertex2f(80.0f, 340.0f);
    glEnd();


    //5th window p
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(60.0f, 350.0f);
    glVertex2f(70.0f, 350.0f);
    glVertex2f(70.0f, 360.0f);
    glVertex2f(60.0f, 360.0f);
    glEnd();

    //6th p
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(80.0f, 350.0f);
    glVertex2f(90.0f, 350.0f);
    glVertex2f(90.0f, 360.0f);
    glVertex2f(80.0f, 360.0f);
    glEnd();

    //cyan building
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(120.0f, 300.0f);
    glVertex2f(170.0f, 300.0f);
    glVertex2f(170.0f, 450.0f);
    glVertex2f(120.0f, 450.0f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    //door
    glBegin(GL_QUADS);
    glVertex2f(135.0f, 300.0f);
    glVertex2f(155.0f, 300.0f);
    glVertex2f(155.0f, 320.0f);
    glVertex2f(135.0f, 320.0f);
    glEnd();


    glColor3f(0.8f, 0.8f, 0.8f);
    //1st cyan
    glBegin(GL_QUADS);
    glVertex2f(130.0f, 350.0f);
    glVertex2f(140.0f, 350.0f);
    glVertex2f(140.0f, 360.0f);
    glVertex2f(130.0f, 360.0f);
    glEnd();

    //2nd window cyan
    glBegin(GL_QUADS);
    glVertex2f(150.0f, 350.0f);
    glVertex2f(160.0f, 350.0f);
    glVertex2f(160.0f, 360.0f);
    glVertex2f(150.0f, 360.0f);
    glEnd();

    //3rd window cyan
    glBegin(GL_QUADS);
    glVertex2f(130.0f, 380.0f);
    glVertex2f(140.0f, 380.0f);
    glVertex2f(140.0f, 390.0f);
    glVertex2f(130.0f, 390.0f);
    glEnd();

    //4th window cyan
    glBegin(GL_QUADS);
    glVertex2f(150.0f, 380.0f);
    glVertex2f(160.0f, 380.0f);
    glVertex2f(160.0f, 390.0f);
    glVertex2f(150.0f, 390.0f);
    glEnd();

    //yellow building
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(190.0f, 300.0f);
    glVertex2f(240.0f, 300.0f);
    glVertex2f(240.0f, 430.0f);
    glVertex2f(190.0f, 430.0f);
    glEnd();

// Door yellow
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(205.0f, 300.0f);
    glVertex2f(225.0f, 300.0f);
    glVertex2f(225.0f, 320.0f);
    glVertex2f(205.0f, 320.0f);
    glEnd();


    glColor3f(0.8f, 0.8f, 0.8f);
    //1st window
    glBegin(GL_QUADS);
    glVertex2f(200.0f, 350.0f);
    glVertex2f(210.0f, 350.0f);
    glVertex2f(210.0f, 360.0f);
    glVertex2f(200.0f, 360.0f);
    glEnd();

    //2nd window
    glBegin(GL_QUADS);
    glVertex2f(220.0f, 350.0f);
    glVertex2f(230.0f, 350.0f);
    glVertex2f(230.0f, 360.0f);
    glVertex2f(220.0f, 360.0f);
    glEnd();

    //3rd window
    glBegin(GL_QUADS);
    glVertex2f(200.0f, 370.0f);
    glVertex2f(210.0f, 370.0f);
    glVertex2f(210.0f, 380.0f);
    glVertex2f(200.0f, 380.0f);
    glEnd();

    //4th window
    glBegin(GL_QUADS);
    glVertex2f(220.0f, 370.0f);
    glVertex2f(230.0f, 370.0f);
    glVertex2f(230.0f, 380.0f);
    glVertex2f(220.0f, 380.0f);
    glEnd();


    //red Building
    glColor3f(0.7f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(260.0f, 300.0f);
    glVertex2f(310.0f, 300.0f);
    glVertex2f(310.0f, 380.0f);
    glVertex2f(260.0f, 380.0f);
    glEnd();

    //door
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(280.0f, 300.0f);
    glVertex2f(300.0f, 300.0f);
    glVertex2f(300.0f, 320.0f);
    glVertex2f(280.0f, 320.0f);
    glEnd();


    glColor3f(0.8f, 0.8f, 0.8f);
    //1st window
    glBegin(GL_QUADS);
    glVertex2f(275.0f, 330.0f);
    glVertex2f(285.0f, 330.0f);
    glVertex2f(285.0f, 340.0f);
    glVertex2f(275.0f, 340.0f);
    glEnd();

    // 2nd
    glBegin(GL_QUADS);
    glVertex2f(295.0f, 330.0f);
    glVertex2f(305.0f, 330.0f);
    glVertex2f(305.0f, 340.0f);
    glVertex2f(295.0f, 340.0f);
    glEnd();

    // 3rd w
    glBegin(GL_QUADS);
    glVertex2f(275.0f, 350.0f);
    glVertex2f(285.0f, 350.0f);
    glVertex2f(285.0f, 360.0f);
    glVertex2f(275.0f, 360.0f);
    glEnd();

    // 4th w
    glBegin(GL_QUADS);
    glVertex2f(295.0f, 350.0f);
    glVertex2f(305.0f, 350.0f);
    glVertex2f(305.0f, 360.0f);
    glVertex2f(295.0f, 360.0f);
    glEnd();


    //Blue Building
    glColor3f(0.4f, 0.6f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(330.0f, 300.0f);
    glVertex2f(380.0f, 300.0f);
    glVertex2f(380.0f, 470.0f);
    glVertex2f(330.0f, 470.0f);
    glEnd();

    //door
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(345.0f, 300.0f);
    glVertex2f(370.0f, 300.0f);
    glVertex2f(370.0f, 320.0f);
    glVertex2f(345.0f, 320.0f);
    glEnd();

    glColor3f(0.8f, 0.8f, 0.8f);
    //1st w
    glBegin(GL_QUADS);
    glVertex2f(340.0f, 330.0f);
    glVertex2f(350.0f, 330.0f);
    glVertex2f(350.0f, 340.0f);
    glVertex2f(340.0f, 340.0f);
    glEnd();

    // 2nd w
    glBegin(GL_QUADS);
    glVertex2f(360.0f, 330.0f);
    glVertex2f(370.0f, 330.0f);
    glVertex2f(370.0f, 340.0f);
    glVertex2f(360.0f, 340.0f);
    glEnd();

    // 3rd w
    glBegin(GL_QUADS);
    glVertex2f(340.0f, 360.0f);
    glVertex2f(350.0f, 360.0f);
    glVertex2f(350.0f, 370.0f);
    glVertex2f(340.0f, 370.0f);
    glEnd();

    //4th w
    glBegin(GL_QUADS);
    glVertex2f(360.0f, 360.0f);
    glVertex2f(370.0f, 360.0f);
    glVertex2f(370.0f, 370.0f);
    glVertex2f(360.0f, 370.0f);
    glEnd();

    //5th w
    glBegin(GL_QUADS);
    glVertex2f(340.0f, 380.0f);
    glVertex2f(350.0f, 380.0f);
    glVertex2f(350.0f, 390.0f);
    glVertex2f(340.0f, 390.0f);
    glEnd();

    //6th w
    glBegin(GL_QUADS);
    glVertex2f(360.0f, 380.0f);
    glVertex2f(370.0f, 380.0f);
    glVertex2f(370.0f, 390.0f);
    glVertex2f(360.0f, 390.0f);
    glEnd();





    // Trees
    for (int i = 0; i < 16; i++)
    {
        float x = i * 50 + 25;
        glColor3f(0.54f, 0.27f, 0.07f);
        glBegin(GL_QUADS);
        glVertex2f(x - 5, 200);
        glVertex2f(x + 5, 200);
        glVertex2f(x + 5, 220);
        glVertex2f(x - 5, 220);
        glEnd();

        // Leaves
        glColor3f(0.0f, 0.5f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 15, 220);
        glVertex2f(x + 15, 220);
        glVertex2f(x, 240);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(x - 10, 230);
        glVertex2f(x + 10, 230);
        glVertex2f(x, 250);
        glEnd();
    }


    // Red car
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(100 + position * 800, 130);
    glVertex2f(160 + position * 800, 130);
    glVertex2f(160 + position * 800, 150);
    glVertex2f(100 + position * 800, 150);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(110 + position * 800, 150);
    glVertex2f(150 + position * 800, 150);
    glVertex2f(140 + position * 800, 170);
    glVertex2f(120 + position * 800, 170);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(115 + position * 800, 120);
    glVertex2f(125 + position * 800, 120);
    glVertex2f(125 + position * 800, 130);
    glVertex2f(115 + position * 800, 130);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(135 + position * 800, 120);
    glVertex2f(145 + position * 800, 120);
    glVertex2f(145 + position * 800, 130);
    glVertex2f(135 + position * 800, 130);
    glEnd();

    // Draw the Sun
    drawSunDay();
    drawBenches();
    glPopMatrix();

    drawBus1();
    glFlush();
}

void drawNight()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glScalef(1.5,1.35,1.);
    // Sky
    glColor3f(0.0, 0.0, 0.1);
    glBegin(GL_QUADS);
    glVertex2f(0, 600);
    glVertex2f(800, 600);
    glColor3f(0.63, 0.86, 0.98);
    glVertex2f(800, 300);
    glVertex2f(0, 300);
    glEnd();

    // Green hill 1
    glColor3f(0.1, 0.26, 0.09);
    glBegin(GL_POLYGON);
    glVertex2f(510.3506290118808, 405.3397492062188);
    glVertex2f(466.3406936760255, 428.2020532767929);
    glVertex2f(423.473873543699, 453.3505877544245);
    glVertex2f(387.4657446325447, 435.0607444979652);
    glVertex2f(343.4558092966896, 409.9122100203336);
    glVertex2f(315.4494868102362, 392.765481967403);
    glVertex2f(254.2928234214504, 360.1866986668348);
    glVertex2f(203.9957544661873, 327.6079153662666);
    glVertex2f(136.9233583757059, 290.2514402512625);
    glVertex2f(496.1425713386362, 275.5561088118699);
    glVertex2f(590.3686932588903, 371.0462931003575);
    glEnd();

    // Green hill 2
    glColor3f(0.1, 0.35, 0.09);
    glBegin(GL_POLYGON);
    glVertex2f(485.1471331652317, 325.5892345736768);
    glVertex2f(450.8281643952328, 311.4578944919123);
    glVertex2f(422.2290237535669, 302.7099220603438);
    glVertex2f(401.3684741090577, 292.9525681943637);
    glVertex2f(724.7069935989501, 287.9056610223049);
    glVertex2f(771.4750000600271, 354.5248356934802);
    glVertex2f(737.1560312900282, 382.1145949007345);
    glVertex2f(712.5944163860092, 402.975144545244);
    glVertex2f(682.3129733536572, 428.5461408836749);
    glVertex2f(673.9014614002261, 431.9107456650473);
    glVertex2f(666.1628704030695, 431.9107456650473);
    glVertex2f(658.76073988405, 426.5273780148514);
    glVertex2f(638.5731111958154, 411.3866564986752);
    glVertex2f(602.9083005132675, 389.1802649416168);
    glVertex2f(561.5236617023863, 366.3009524282838);
    glVertex2f(508.6993666348388, 339.7205746554412);
    glEnd();

    // Green hill 3
    glColor3f(0.06, 0.38, 0.04);
    glBegin(GL_POLYGON);
    glVertex2f(655.2613949161106, 331.8265529875498);
    glVertex2f(631.1674600767025, 327.1632107605675);
    glVertex2f(612.5140911687736, 323.6657040903308);
    glVertex2f(588.0315444771169, 315.1162433408633);
    glVertex2f(553.0564777747503, 296.4628744329342);
    glVertex2f(805.654181736287, 294.5198151716916);
    glVertex2f(803.7111224750445, 355.5318759747096);
    glVertex2f(781.560246896879, 362.526889315183);
    glVertex2f(764.4613253979442, 364.8585604286741);
    glVertex2f(738.812943149542, 364.4699485764256);
    glVertex2f(720, 360);
    glVertex2f(700.7289816291872, 348.9254744864848);
    glVertex2f(677.4122704942761, 338.4329544757746);
    glEnd();

    // Green ground
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, 300);
    glVertex2f(800, 300);
    glVertex2f(800, 250);
    glVertex2f(0, 250);
    glEnd();


    // Blue sky area
    glColor3f(0.35f, 0.58f, 0.67f);
    glBegin(GL_QUADS);
    glVertex2f(0, 250);
    glVertex2f(800, 250);
    glVertex2f(800, 200);
    glVertex2f(0, 200);
    glEnd();

    // Road
    glColor3f(0.34f, 0.32f, 0.32f);
    glBegin(GL_QUADS);
    glVertex2f(0, 200);
    glVertex2f(800, 200);
    glVertex2f(800, 80);
    glVertex2f(0, 80);
    glEnd();

    //road middle
    glColor3f(0.72, 0.67, 0.11);
    glBegin(GL_QUADS);
    glVertex2f(0, 140);
    glVertex2f(90, 140);
    glVertex2f(90, 148);
    glVertex2f(0, 148);
    glEnd();

    glColor3f(0.72, 0.67, 0.11);
    glBegin(GL_QUADS);
    glVertex2f(210, 140);
    glVertex2f(300, 140);
    glVertex2f(300, 148);
    glVertex2f(210, 148);
    glEnd();

    glColor3f(0.72, 0.67, 0.11);
    glBegin(GL_QUADS);
    glVertex2f(410, 140);
    glVertex2f(500, 140);
    glVertex2f(500, 148);
    glVertex2f(410, 148);
    glEnd();

    glColor3f(0.72, 0.67, 0.11);
    glBegin(GL_QUADS);
    glVertex2f(610, 140);
    glVertex2f(700, 140);
    glVertex2f(700, 148);
    glVertex2f(610, 148);
    glEnd();

    glColor3f(0.72, 0.67, 0.11);
    glBegin(GL_QUADS);
    glVertex2f(790, 140);
    glVertex2f(900, 140);
    glVertex2f(900, 148);
    glVertex2f(790, 148);
    glEnd();

    //Road divider 1
    for (int i = 0; i < 16; i++)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(i * 50, 200);
        glVertex2f(i * 50 + 25, 200);
        glVertex2f(i * 50 + 25, 195);
        glVertex2f(i * 50, 195);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(i * 50 + 25, 200);
        glVertex2f(i * 50 + 50, 200);
        glVertex2f(i * 50 + 50, 195);
        glVertex2f(i * 50 + 25, 195);
        glEnd();
    }

    //Road divider 2
    for (int i = 0; i < 16; i++)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(i * 50, 86);
        glVertex2f(i * 50 + 25, 86);
        glVertex2f(i * 50 + 25, 81);
        glVertex2f(i * 50, 81);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(i * 50 + 25, 86);
        glVertex2f(i * 50 + 50, 86);
        glVertex2f(i * 50 + 50, 81);
        glVertex2f(i * 50 + 25, 81);
        glEnd();
    }

    //Lamp Post between Bench 1 and 2
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(67, 255);
    glVertex2f(73, 255);
    glVertex2f(73, 295);
    glVertex2f(67, 295);
    glEnd();

    // light
    //drawCircle(-970.0f, 0.0f, 10.0f, 1.0f, 1.0f, 0.0f);

    //between Bench 2 and 3
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(137, 255);
    glVertex2f(143, 255);
    glVertex2f(143, 295);
    glVertex2f(137, 295);
    glEnd();

    //between Bench 3 and 4
    glBegin(GL_QUADS);
    glVertex2f(207, 255);
    glVertex2f(213, 255);
    glVertex2f(213, 295);
    glVertex2f(207, 295);
    glEnd();

    //between Bench 4 and 5
    glBegin(GL_QUADS);
    glVertex2f(277, 255);
    glVertex2f(283, 255);
    glVertex2f(283, 295);
    glVertex2f(277, 295);
    glEnd();

    //between Bench 5 and 6
    glBegin(GL_QUADS);
    glVertex2f(347, 255);
    glVertex2f(353, 255);
    glVertex2f(353, 295);
    glVertex2f(347, 295);
    glEnd();

    //between Bench 6 and 7
    glBegin(GL_QUADS);
    glVertex2f(417, 255);
    glVertex2f(423, 255);
    glVertex2f(423, 295);
    glVertex2f(417, 295);
    glEnd();

    //between Bench 7 and 8
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(477, 255);
    glVertex2f(483, 255);
    glVertex2f(483, 295);
    glVertex2f(477, 295);
    glEnd();

    //between Bench 8 and 9
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(547, 255);
    glVertex2f(553, 255);
    glVertex2f(553, 295);
    glVertex2f(547, 295);
    glEnd();

    //between Bench 9 and 10
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(617, 255);
    glVertex2f(623, 255);
    glVertex2f(623, 295);
    glVertex2f(617, 295);
    glEnd();

    //between Bench 10 and 11
    glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(687, 255);
    glVertex2f(693, 255);
    glVertex2f(693, 295);
    glVertex2f(687, 295);
    glEnd();



    // Buildings
    glColor3f(0.55f, 0.05f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(50.0f, 300.0f);
    glVertex2f(100.0f, 300.0f);
    glVertex2f(100.0f, 400.0f);
    glVertex2f(50.0f, 400.0f);
    glEnd();

    //1st window pink
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(60.0f, 310.0f);
    glVertex2f(70.0f, 310.0f);
    glVertex2f(70.0f, 320.0f);
    glVertex2f(60.0f, 320.0f);
    glEnd();

    //2nd p
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(80.0f, 310.0f);
    glVertex2f(90.0f, 310.0f);
    glVertex2f(90.0f, 320.0f);
    glVertex2f(80.0f, 320.0f);
    glEnd();

    //3rd p
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(60.0f, 330.0f);
    glVertex2f(70.0f, 330.0f);
    glVertex2f(70.0f, 340.0f);
    glVertex2f(60.0f, 340.0f);
    glEnd();

    //4th p
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(80.0f, 330.0f);
    glVertex2f(90.0f, 330.0f);
    glVertex2f(90.0f, 340.0f);
    glVertex2f(80.0f, 340.0f);
    glEnd();


    //5th window p
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(60.0f, 350.0f);
    glVertex2f(70.0f, 350.0f);
    glVertex2f(70.0f, 360.0f);
    glVertex2f(60.0f, 360.0f);
    glEnd();

    //6th p
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(80.0f, 350.0f);
    glVertex2f(90.0f, 350.0f);
    glVertex2f(90.0f, 360.0f);
    glVertex2f(80.0f, 360.0f);
    glEnd();

    //cyan building
    glColor3f(0.08f, 0.4f, 0.39f);
    glBegin(GL_QUADS);
    glVertex2f(120.0f, 300.0f);
    glVertex2f(170.0f, 300.0f);
    glVertex2f(170.0f, 450.0f);
    glVertex2f(120.0f, 450.0f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    //door
    glBegin(GL_QUADS);
    glVertex2f(135.0f, 300.0f);
    glVertex2f(155.0f, 300.0f);
    glVertex2f(155.0f, 320.0f);
    glVertex2f(135.0f, 320.0f);
    glEnd();


    glColor3f(0.8f, 0.8f, 0.8f);
    //1st cyan
    glBegin(GL_QUADS);
    glVertex2f(130.0f, 350.0f);
    glVertex2f(140.0f, 350.0f);
    glVertex2f(140.0f, 360.0f);
    glVertex2f(130.0f, 360.0f);
    glEnd();

    //2nd window cyan
    glBegin(GL_QUADS);
    glVertex2f(150.0f, 350.0f);
    glVertex2f(160.0f, 350.0f);
    glVertex2f(160.0f, 360.0f);
    glVertex2f(150.0f, 360.0f);
    glEnd();

    //3rd window cyan
    glBegin(GL_QUADS);
    glVertex2f(130.0f, 380.0f);
    glVertex2f(140.0f, 380.0f);
    glVertex2f(140.0f, 390.0f);
    glVertex2f(130.0f, 390.0f);
    glEnd();

    //4th window cyan
    glBegin(GL_QUADS);
    glVertex2f(150.0f, 380.0f);
    glVertex2f(160.0f, 380.0f);
    glVertex2f(160.0f, 390.0f);
    glVertex2f(150.0f, 390.0f);
    glEnd();

    //yellow building
    glColor3f(0.54f, 0.56f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f(190.0f, 300.0f);
    glVertex2f(240.0f, 300.0f);
    glVertex2f(240.0f, 430.0f);
    glVertex2f(190.0f, 430.0f);
    glEnd();

// Door yellow
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(205.0f, 300.0f);
    glVertex2f(225.0f, 300.0f);
    glVertex2f(225.0f, 320.0f);
    glVertex2f(205.0f, 320.0f);
    glEnd();


    glColor3f(0.8f, 0.8f, 0.8f);
    //1st window
    glBegin(GL_QUADS);
    glVertex2f(200.0f, 350.0f);
    glVertex2f(210.0f, 350.0f);
    glVertex2f(210.0f, 360.0f);
    glVertex2f(200.0f, 360.0f);
    glEnd();

    //2nd window
    glBegin(GL_QUADS);
    glVertex2f(220.0f, 350.0f);
    glVertex2f(230.0f, 350.0f);
    glVertex2f(230.0f, 360.0f);
    glVertex2f(220.0f, 360.0f);
    glEnd();

    //3rd window
    glBegin(GL_QUADS);
    glVertex2f(200.0f, 370.0f);
    glVertex2f(210.0f, 370.0f);
    glVertex2f(210.0f, 380.0f);
    glVertex2f(200.0f, 380.0f);
    glEnd();

    //4th window
    glBegin(GL_QUADS);
    glVertex2f(220.0f, 370.0f);
    glVertex2f(230.0f, 370.0f);
    glVertex2f(230.0f, 380.0f);
    glVertex2f(220.0f, 380.0f);
    glEnd();


    //red Building
    glColor3f(0.22f, 0.04f, 0.04f);
    glBegin(GL_QUADS);
    glVertex2f(260.0f, 300.0f);
    glVertex2f(310.0f, 300.0f);
    glVertex2f(310.0f, 380.0f);
    glVertex2f(260.0f, 380.0f);
    glEnd();

    //door
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(280.0f, 300.0f);
    glVertex2f(300.0f, 300.0f);
    glVertex2f(300.0f, 320.0f);
    glVertex2f(280.0f, 320.0f);
    glEnd();


    glColor3f(0.8f, 0.8f, 0.8f);
    //1st window
    glBegin(GL_QUADS);
    glVertex2f(275.0f, 330.0f);
    glVertex2f(285.0f, 330.0f);
    glVertex2f(285.0f, 340.0f);
    glVertex2f(275.0f, 340.0f);
    glEnd();

    // 2nd
    glBegin(GL_QUADS);
    glVertex2f(295.0f, 330.0f);
    glVertex2f(305.0f, 330.0f);
    glVertex2f(305.0f, 340.0f);
    glVertex2f(295.0f, 340.0f);
    glEnd();

    // 3rd w
    glBegin(GL_QUADS);
    glVertex2f(275.0f, 350.0f);
    glVertex2f(285.0f, 350.0f);
    glVertex2f(285.0f, 360.0f);
    glVertex2f(275.0f, 360.0f);
    glEnd();

    // 4th w
    glBegin(GL_QUADS);
    glVertex2f(295.0f, 350.0f);
    glVertex2f(305.0f, 350.0f);
    glVertex2f(305.0f, 360.0f);
    glVertex2f(295.0f, 360.0f);
    glEnd();


    //Blue Building
    glColor3f(0.04f, 0.08f, 0.17f);
    glBegin(GL_QUADS);
    glVertex2f(330.0f, 300.0f);
    glVertex2f(380.0f, 300.0f);
    glVertex2f(380.0f, 470.0f);
    glVertex2f(330.0f, 470.0f);
    glEnd();

    //door
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(345.0f, 300.0f);
    glVertex2f(370.0f, 300.0f);
    glVertex2f(370.0f, 320.0f);
    glVertex2f(345.0f, 320.0f);
    glEnd();

    glColor3f(0.8f, 0.8f, 0.8f);
    //1st w
    glBegin(GL_QUADS);
    glVertex2f(340.0f, 330.0f);
    glVertex2f(350.0f, 330.0f);
    glVertex2f(350.0f, 340.0f);
    glVertex2f(340.0f, 340.0f);
    glEnd();

    // 2nd w
    glBegin(GL_QUADS);
    glVertex2f(360.0f, 330.0f);
    glVertex2f(370.0f, 330.0f);
    glVertex2f(370.0f, 340.0f);
    glVertex2f(360.0f, 340.0f);
    glEnd();

    // 3rd w
    glBegin(GL_QUADS);
    glVertex2f(340.0f, 360.0f);
    glVertex2f(350.0f, 360.0f);
    glVertex2f(350.0f, 370.0f);
    glVertex2f(340.0f, 370.0f);
    glEnd();

    //4th w
    glBegin(GL_QUADS);
    glVertex2f(360.0f, 360.0f);
    glVertex2f(370.0f, 360.0f);
    glVertex2f(370.0f, 370.0f);
    glVertex2f(360.0f, 370.0f);
    glEnd();

    //5th w
    glBegin(GL_QUADS);
    glVertex2f(340.0f, 380.0f);
    glVertex2f(350.0f, 380.0f);
    glVertex2f(350.0f, 390.0f);
    glVertex2f(340.0f, 390.0f);
    glEnd();

    //6th w
    glBegin(GL_QUADS);
    glVertex2f(360.0f, 380.0f);
    glVertex2f(370.0f, 380.0f);
    glVertex2f(370.0f, 390.0f);
    glVertex2f(360.0f, 390.0f);
    glEnd();





    // Tree trunk
    for (int i = 0; i < 16; i++)
    {
        float x = i * 50 + 25;
        glColor3f(0.54f, 0.27f, 0.07f);
        glBegin(GL_QUADS);
        glVertex2f(x - 5, 200);
        glVertex2f(x + 5, 200);
        glVertex2f(x + 5, 220);
        glVertex2f(x - 5, 220);
        glEnd();

        // Leaves
        glColor3f(0.0f, 0.5f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 15, 220);
        glVertex2f(x + 15, 220);
        glVertex2f(x, 240);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(x - 10, 230);
        glVertex2f(x + 10, 230);
        glVertex2f(x, 250);
        glEnd();
    }


    // Red car
    glColor3f(0.67f, 0.01f, 0.01f);
    glBegin(GL_QUADS);
    glVertex2f(100 + position * 800, 130);
    glVertex2f(160 + position * 800, 130);
    glVertex2f(160 + position * 800, 150);
    glVertex2f(100 + position * 800, 150);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(110 + position * 800, 150);
    glVertex2f(150 + position * 800, 150);
    glVertex2f(140 + position * 800, 170);
    glVertex2f(120 + position * 800, 170);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(115 + position * 800, 120);
    glVertex2f(125 + position * 800, 120);
    glVertex2f(125 + position * 800, 130);
    glVertex2f(115 + position * 800, 130);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(135 + position * 800, 120);
    glVertex2f(145 + position * 800, 120);
    glVertex2f(145 + position * 800, 130);
    glVertex2f(135 + position * 800, 130);
    glEnd();

    //blue car
    glColor3f(0.04f, 0.3f, 0.44f);
    glBegin(GL_QUADS);
    glVertex2f(300 + posCar2 * 800, 130);
    glVertex2f(360 + posCar2 * 800, 130);
    glVertex2f(360 + posCar2 * 800, 150);
    glVertex2f(300 + posCar2 * 800, 150);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(310 + posCar2 * 800, 150);
    glVertex2f(350 + posCar2 * 800, 150);
    glVertex2f(340 + posCar2 * 800, 170);
    glVertex2f(320 + posCar2 * 800, 170);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(315 + posCar2 * 800, 120);
    glVertex2f(325 + posCar2 * 800, 120);
    glVertex2f(325 + posCar2 * 800, 130);
    glVertex2f(315 + posCar2 * 800, 130);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(335 + posCar2 * 800, 120);
    glVertex2f(345 + posCar2 * 800, 120);
    glVertex2f(345 + posCar2 * 800, 130);
    glVertex2f(335 + posCar2 * 800, 130);
    glEnd();



    drawMoonNight();
    drawCircle(77.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(62.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(132.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(147.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(202.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(217.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(272.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(287.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(342.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(357.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(412.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(427.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(472.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(487.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(542.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(557.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(612.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(627.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(682.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawCircle(697.0f, 290.0f, 5.0f, 1.0f, 1.0f, 0.0f);
    drawBenches();
    glPopMatrix();
    glFlush();
}

/////////////////////////////////////////////////////////////////////////////////////////
void initScreen5()
{
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 1200, 0, 800);
}

void display5()
{
    if(nightScenary)
      drawNight();
    else
        drawDay3();

}


// Function to draw objects for different screens
    void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if(currentScreen == 1){
    display1();
    }
    else if (currentScreen == 2) {
        display2();
    } else if (currentScreen == 3) {
        //initScreen3();
        display3();
    } else if (currentScreen == 4) {
        display4();
    } else if (currentScreen == 5) {
        //initScreen5();
        display5();
    }
    drawBus1();
    if(currentScreen==1)
        drawCityLights(200);
    glutSwapBuffers();
}

/////////////////////////////////////////////////For 5 ////////////////////////////////////////////////////////////////////////////////
void SpecialInput2(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        speedCar1 += 0.001f;  //increase speedCar1
        break;
    case GLUT_KEY_DOWN:
        speedCar1 -= 0.001f;  //decrease speedCar1
        if (speedCar1 < 0.0f) speedCar1 = 0.0f;  // Ensure speedCar1 doesn't go negative
        break;
    case GLUT_KEY_LEFT:
        position = -1.0f;  // Reset position to left side when left arrow is pressed
        break;
    case GLUT_KEY_RIGHT:
        position = 1.0f;  // Reset position to right side when right arrow is pressed
        break;
    }
    glutPostRedisplay();
}


void mouse2(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) // Only register clicks when the button is pressed
    {
        if (button == GLUT_LEFT_BUTTON)
        {if(currentScreen == 5)
            speedCar12 += 0.001f;
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {if(currentScreen == 5)
            speedCar12 -= 0.001f;
              if (speedCar12 < 0.0f) speedCar12 = 0.0f;
        }
    }
    glutPostRedisplay(); // Request display updated
}


/////////////////////////////////// FOR ALL And DISPLAY 1/////////////////////////////////////////////////


void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W': // Start bus1
            isBus1Moving = true;
            break;
        case 'q': case 'Q': // Stop bus1
            isBus1Moving = false;
            break;
        case 'p': case 'P': // Start bus1
            if (currentScreen == 1)
            isBus2Moving = true;
            break;
        case 'o': case 'O': // Stop bus1
            if (currentScreen == 1)
            isBus2Moving = false;
            break;
        case 'k': case 'K': // Stop only at screen 5
            if (currentScreen == 5) {
                isBus1Moving = false;
            }
            break;
        case 'd': case 'D': // Day for All
                isDay = true;
                nightScenary = false;

            break;
        case 'n': case 'N': // Night for All
                isDay = false;
                nightScenary = true;
            break;
        case 'r': case 'R': // start raining only 1
            if (currentScreen == 1 || currentScreen == 3) {
            isRaining = true;}
            break;
        case 'h': case 'H': // start raining only 1
            if (currentScreen == 1 || currentScreen == 3) {
            isRaining = false;}
            break;
        case 'b': case 'B': // Start boat for display3
            if (currentScreen == 3)
            isBoatMoving = true;
            break;
        case 'g': case 'G': // Stop boat for display3
            if (currentScreen == 3)
            isBoatMoving = false;
            break;
         case 'c': case 'C': // Start cloud
             if (currentScreen == 3)
            isCloudMoving = true;
            break;
        case 'v': case 'V': // Stop cloud
            if (currentScreen == 3)
            isCloudMoving = false;
            break;
            case 27:
    exit(0);
    break;
case 'm':
case 'M':
        isMorning =true;
        break;
case 'e':
case 'E':
        isMorning =false;
        break;
        case '+': case '=': // increase speed bus for display 1
            speed += 0.5f;
            break;
        case '-': case '_': // Decrease speed bus for display 1
            speed -= 0.5f;
            if (speed < 0.1f) speed = 0.1f; // Prevent negative speed
            break;
    }
}


// Initialize OpenGL settings
void init() {
    initScreen();
}
///////////////// Software opreation /////////////////

int main(int argc, char** argv) {
    srand(time(0));  // Seed for random number generator
    for (int i = 0; i < 100; i++) {
        rainDropX[i] = rand() % 1200;  // Random X position
        rainDropY[i] = rand() % 800;  // Random Y position
    }
    cout<<"//////////////// FOR SCREEN 1 /////////////////\n"<<endl;
    cout<<"START BUS (W)  ||  STOP BUS (Q)"<<endl;
    cout<<"START BUS2 (P)  ||  STOP BUS2 (O)"<<endl;
    cout<<"START RAIN(R) ||  STOP RAIN (H)"<<endl;
    cout<<"START DAY (D)  ||  START NIGHT (N)"<<endl;
    cout<<"Mouse Left START PLANE  ||  Mouse RIGHT STOP PLANE \n"<<endl;

    cout<<"//////////////// FOR SCREEN 2 /////////////////\n"<<endl;
    cout<< "Press M/m key to see Morning Scenario. "<< endl;
    cout<< "Press E/e key to see Evening view. "<< endl;
    cout<< "Press UP key to increase the speed of the  Train. "<< endl;
    cout<< "Press LOW key to decrease the speed of the  Train. "<< endl;
    cout<< "Click Mouse or TouchPad to STOP the  Train. "<< endl;
    cout<<"\n"<<endl;

    cout<<"//////////////// FOR SCREEN 3 /////////////////\n"<<endl;
    cout<<"START BUS (W)  ||  STOP BUS (Q)"<<endl;
    cout<<"START CloudMoving (C)  ||  STOP CloudMoving (V)"<<endl;
    cout<<"START RAIN(R) ||  STOP RAIN (H)"<<endl;
    cout<<"START BoatMoving (B)  ||  BoatMoving (G)"<<endl;
    cout<<"\n"<<endl;

    cout<<"//////////////// FOR SCREEN 4 /////////////////\n"<<endl;
    cout<<"START BUS (W)  ||  STOP BUS (Q)"<<endl;
    cout<<"START RAIN(R) ||  STOP RAIN (H)"<<endl;
    cout<<"START DAY (D)  ||  START NIGHT (N)"<<endl;
    cout<<"\n"<<endl;

    cout<<"//////////////// FOR SCREEN 5 /////////////////\n"<<endl;
    cout<<"START BUS (W)  ||  STOP BUS (Q)"<<endl;
    cout<<"START RAIN(R) ||  STOP RAIN (H)"<<endl;
    cout<<"START DAY (D)  ||  START NIGHT (N)"<<endl;
    cout<<"Mouse Left START PLANE  ||  Mouse RIGHT STOP PLANE \n"<<endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Bus Moving Through 5 Screens");

    init();
    glutDisplayFunc(display);
    //////////////////////////////////// FOR DISLAY 1 /////////////////////////////////
    glutMouseFunc(mouse1);
    glutKeyboardFunc(keyboardFunc);
    glutTimerFunc(20, MovingPart, 0);
    glutTimerFunc(20, Bus1Move, 0);
    glutTimerFunc(20, Bus2Move, 0);
    glutTimerFunc(20, PlaneMove, 0);
    glutTimerFunc(2000, toggleLED, 0);

    //////////////////////////////////// FOR DISPLAY 2 /////////////////////////////////
    glutSpecialFunc(specialKeysM);
    glutMouseFunc(mouseM);
    glutTimerFunc(20, updateM, 0);  // Register the timer function
     glutTimerFunc(20, updateV, 0);
     glutTimerFunc(20, updateS, 0);

    //////////////////////////////////// FOR DISPLAY 3 /////////////////////////////////
    glutTimerFunc(10, update2_11, 0);


    //////////////////////////////////// FOR DISPLAY 4 /////////////////////////////////
    glutTimerFunc(20, update_44, 0);
    glutTimerFunc(50, updateBoat, 0);

    //////////////////////////////////// FOR DISPLAY 5 /////////////////////////////////
    glutTimerFunc(100, updatedD5, 0);
    //glutSpecialFunc(SpecialInput2);
    glutMouseFunc(mouse2);
    glutMainLoop();
    return 0;
}
