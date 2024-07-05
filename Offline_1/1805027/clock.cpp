#define _USE_MATH_DEFINES

#include <iostream>
#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <time.h>
#include <cmath>

float cx=0, cy=0.4, r=0.37, r1=0.28, r2=0.25, hourRadius=0.12, minuteRadius=0.18, timeNow=0;
float g=9.8;
//float l=(1.0f/(M_PI)^2)*g;
float l=0.37;
float w=M_PI;
float max_theta=40;
float current_theta=max_theta - 90;

float getTheta(int t)
{
    return max_theta*cosf((w*t*M_PI)/180);
}


void glDrawCircles()
{
    float r=0.37;
       for(int i=0;i<4;i++)
       {
        if(i == 0)
        {
            glBegin(GL_POLYGON);  // All vertices form a single loop of single pixel width
                glColor3f(1.0f,0.2f,0.6f);  // Light-blue
                for (float theta = 0; theta < 360; theta += 5) {
                     float x = cx + r * cos(theta/180*M_PI);
                     float y = cy + r * sin(theta/180*M_PI);
                     glVertex2f(x,y);
                }
            glEnd();
        }
        
        else if(i == 1)
        {
            r-=0.03;
            glBegin(GL_POLYGON);  // All vertices form a single loop of single pixel width
                glColor3f(0.7f,0.4f,0.5f);  // Light-blue
                for (float theta = 0; theta < 360; theta += 5) {
                     float x = cx + r * cos(theta/180*M_PI);
                     float y = cy + r * sin(theta/180*M_PI);
                     glVertex2f(x,y);
                }
            glEnd();
        }

        else if(i == 2)
        {
            r-=0.03;
            glBegin(GL_POLYGON);  // All vertices form a single loop of single pixel width
                glColor3f(0.5f, 0.5f, 0.5f);  // Light-blue
                for (float theta = 0; theta < 360; theta += 5) {
                     float x = cx + r * cos(theta/180*M_PI);
                     float y = cy + r * sin(theta/180*M_PI);
                     glVertex2f(x,y);
                }
            glEnd();
        }
        
        else if(i == 3)
        {
            r-=0.03;
            glBegin(GL_POLYGON);  // All vertices form a single loop of single pixel width
                glColor3f(0.4f, 0.4f, 0.6f);  // Light-blue
                for (float theta = 0; theta < 360; theta += 5) {
                     float x = cx + r * cos(theta/180*M_PI);
                     float y = cy + r * sin(theta/180*M_PI);
                     glVertex2f(x,y);
                }
            glEnd();
        }
      }
}

void glDrawKata()
{
    glBegin(GL_LINES);
        glColor3f(0,0,0);
        glLineWidth(2);

        for(float theta = 0; theta < 360 ; theta+=5) {
            if((int)theta % 90 == 0) {
                glColor3f(0,0,0);
                float px1 = cx + r2 * cos(theta/180*M_PI);
                float py1 = cy + r2 * sin(theta/180*M_PI);
                float px2 = cx + (r2-0.05) * cos(theta/180*M_PI);
                float py2 = cy + (r2-0.05) * sin(theta/180*M_PI);
                glVertex2d(px1,py1);
                glVertex2d(px2,py2);
            }

            else if((int)theta % 30 ==0) {
                glColor3f(1,0,0);
                float px1 = cx + r2 * cos(theta/180*M_PI);
                float py1 = cy + r2 * sin(theta/180*M_PI);
                float px2 = cx + (r2-0.02) * cos(theta/180*M_PI);
                float py2 = cy + (r2-0.02) * sin(theta/180*M_PI);
                glVertex2d(px1,py1);
                glVertex2d(px2,py2);
            }
        }

    glEnd();    
}

void glDrawHourHand(){
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    //struct time *t=glGetCurrentTime();
    int hour=local->tm_hour % 12;
    float theta1 = hour * 30 + 0.5f * local->tm_min;
    std :: cout << theta1 << " angle " << std :: endl;
    std :: cout << local->tm_min << " minute " << std :: endl;
    float theta=90 - theta1;
    std :: cout << theta << std :: endl;
    float px1 = cx + hourRadius * cos(theta/180*M_PI);
    float py1 = cy + hourRadius * sin(theta/180*M_PI);
    glBegin(GL_LINES);
       glColor3f(0.0f,0.0f,0.0f);
       glVertex2f(cx,cy);
       glVertex2f(px1,py1);
    glEnd();

    float px2 = cx + hourRadius * cos((theta - 5)/180*M_PI);
    float py2 = cy + hourRadius * sin((theta - 5)/180*M_PI);
    float px3 = cx + hourRadius * cos((theta + 5)/180*M_PI);
    float py3 = cy + hourRadius * sin((theta + 5)/180*M_PI);
    float px4 = cx + (hourRadius + 0.02) * cos(theta/180*M_PI);
    float py4 = cy + (hourRadius + 0.02) * sin(theta/180*M_PI);

    glBegin(GL_TRIANGLES);
       glVertex2f(px2,py2);
       glVertex2f(px3,py3);
       glVertex2f(px4,py4);
    glEnd();
  
}

void glDrawMinuteHand(){
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    //struct time *t=glGetCurrentTime();
    int minute = local->tm_min;
    float theta = 90 - minute * 6;
    float px1 = cx + minuteRadius * cos(theta/180*M_PI);
    float py1 = cy + minuteRadius * sin(theta/180*M_PI);
    glBegin(GL_LINES);
       glVertex2f(cx,cy);
       glVertex2f(px1,py1);
    glEnd();

    float px2 = cx + minuteRadius * cos((theta - 3)/180*M_PI);
    float py2 = cy + minuteRadius * sin((theta - 3)/180*M_PI);
    float px3 = cx + minuteRadius * cos((theta + 3)/180*M_PI);
    float py3 = cy + minuteRadius * sin((theta + 3)/180*M_PI);
    float px4 = cx + (minuteRadius + 0.02) * cos(theta/180*M_PI);
    float py4 = cy + (minuteRadius + 0.02) * sin(theta/180*M_PI);

    glBegin(GL_TRIANGLES);
       glVertex2f(px2,py2);
       glVertex2f(px3,py3);
       glVertex2f(px4,py4);
    glEnd();
}

void glDrawSecondHand()
{
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    int second = local->tm_sec;
    float theta = 90 - second * 6;
    float px1 = cx + minuteRadius * cos(theta/180*M_PI);
    float py1 = cy + minuteRadius * sin(theta/180*M_PI);
    glBegin(GL_LINES);
       glVertex2f(cx,cy);
       glVertex2f(px1,py1);
    glEnd();

}

void drawBob()
{
    float cx1=0,cy1=0.4-r-0.02;
    float cx = cx1,cy = cy1;
    glBegin(GL_POLYGON);  // All vertices form a single loop of single pixel width
        glColor3f(0.4f,0.4f,0.8f);  // Light-blue
        for (float theta = 0; theta < 360; theta += 5) {
            float x = cx + 0.03 * cos(theta/180*M_PI);
            float y = cy + 0.03 * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }
    glEnd();

    float theta = getTheta(timeNow) - 90;
    float cx2 = cx1 + l * cos(theta/180*M_PI);
    float cy2 = cy1 + l * sin(theta/180*M_PI);
    
    cx = cx2;
    cy = cy2;
    glBegin(GL_POLYGON);  // All vertices form a single loop of single pixel width
        glColor3f(0.4f,0.4f,0.8f);  // Light-blue
        for (float theta = 0; theta < 360; theta += 5) {
            float x = cx + 0.05 * cos(theta/180*M_PI);
            float y = cy + 0.05 * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }
    glEnd();

}

void drawPendulumHand()
{
    float cx1=0,cy1=0.4-r-0.03;
    float theta = getTheta(timeNow) - 90;
    float cx2 = cx1 + l * cos(theta/180*M_PI);
    float cy2 = cy1 + l * sin(theta/180*M_PI);

    glLineWidth(5);
    glBegin(GL_LINES);
       glColor3f(0.5f,0.5f,0.5f);   
       glVertex2f(cx1,cy1);
       glVertex2f(cx2,cy2);
    glEnd();

    drawBob();
    timeNow += 0.1;
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

     glPointSize(5); // Make next points look bigger
     glLineWidth(2); // Make next lines wider
     glDrawCircles();
     glDrawKata();
     glLineWidth(3);
     glDrawHourHand();
     glLineWidth(3);
     glDrawMinuteHand();
     glLineWidth(1);
     glDrawSecondHand();
     drawPendulumHand();
     
    glBegin(GL_LINE_LOOP);
       glVertex2d(-.51,0.85);
       glVertex2d(0.51,0.85);
       glVertex2d(0.51,0);
       glVertex2d(0,-0.7);
       glVertex2d(-0.51,0);
    glEnd();
     glFlush();  // Render now
}

void timerListener(int value)
{
    
    glutPostRedisplay();
    glutTimerFunc(1,timerListener,0);
}

// void timerListener(int value)
// {
//     glutPostRedisplay();
//     glutTimerFunc(1000,timerListener,0);
// }

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(640, 640);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
    glutCreateWindow("OpenGL 2D Drawing");  // Create a window with the given title
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    //glutDisplayFunc(drawPendulumHand);
    //glutTimerFunc(0,timerListener,0);
    glutTimerFunc(0,timerListener,0);
    glutMainLoop();                         // Enter the infinitely event-processing loop
    return 0;
}
