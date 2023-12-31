#include<windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
using namespace std;
#define   PI           3.14159
#define WIN_WIDTH      1300
#define WIN_HEIGHT     1000
#define CYCLE_LENGTH   3.3f
#define ROD_RADIUS     0.05f
#define NUM_SPOKES     20
#define SPOKE_ANGLE    18
#define RADIUS_WHEEL   1.0f
#define TUBE_WIDTH     0.08f
#define RIGHT_ROD      1.6f
#define RIGHT_ANGLE    48.0f
#define MIDDLE_ROD     1.7f
#define MIDDLE_ANGLE   106.0f
#define BACK_CONNECTOR 0.5f
#define LEFT_ANGLE     50.0f
#define WHEEL_OFFSET   0.11f
#define WHEEL_LEN      1.1f
#define TOP_LEN        1.5f
#define CRANK_ROD      0.7f
#define CRANK_RODS     1.12f
#define CRANK_ANGLE    8.0f
#define HANDLE_ROD     1.5f
#define FRONT_INCLINE  70.0f
#define HANDLE_LIMIT   70.0f
#define INC_STEERING   5.0f
#define INC_SPEED      0.05f
GLfloat pedalAngle, speed, steering;
GLfloat camx,camy,camz;
GLfloat anglex,angley,anglez;
int prevx,prevy;
GLenum Mouse;
GLfloat xpos,zpos,direction;
//Methods that will implement along below are
void ZCylinder(GLfloat radius,GLfloat length);
void XCylinder(GLfloat radius,GLfloat length);
void drawFrame(void);
void gear( GLfloat inner_radius, GLfloat outer_radius,
        GLfloat width,GLint teeth, GLfloat tooth_depth );
void drawChain(void);
void drawPedals(void);
void drawTyre(void);
void drawSeat(void);
void help(void);
void init(void);
void reset(void);
void display(void);
void idle(void);
void updateScene(void);
void landmarks(void);
void Camera(int key,int x,int y);
void keyboard(unsigned char key,int x,int y);
void mouse(int button,int state,int x,int y);
void motion(int x,int y);
void reshape(int w,int h);
void glSetupFuncs(void);
GLfloat Absolute(GLfloat);
GLfloat degrees(GLfloat);
GLfloat radians(GLfloat);
GLfloat angleSum(GLfloat, GLfloat);

void ZCylinder(GLfloat radius,GLfloat length)
{
   GLUquadricObj *cylinder; //Quad cylinder
   cylinder=gluNewQuadric();
   glPushMatrix();
      glTranslatef(0.0f,0.0f,0.0f);
      gluCylinder(cylinder,radius,radius,length,15,5);
   glPopMatrix();
}
void XCylinder(GLfloat radius,GLfloat length)
{
   glPushMatrix();
      glRotatef(90.0f,0.0f,1.0f,0.0f);
      ZCylinder(radius,length);
   glPopMatrix();
}
void updateScene()
{
   GLfloat xDelta, zDelta;
   GLfloat rotation;
   GLfloat sin_steering, cos_steering;
   if (-INC_SPEED < speed && speed < INC_SPEED) return;
   if(speed < 0.0f)
         pedalAngle = speed = 0.0f;
   xDelta = speed*cos(radians(direction + steering));
   zDelta = speed*sin(radians(direction + steering));
   xpos += xDelta;
   zpos -= zDelta;
   pedalAngle = degrees(angleSum(radians(pedalAngle), speed/RADIUS_WHEEL));
   sin_steering = sin(radians(steering));
   cos_steering = cos(radians(steering));
   rotation = atan2(speed * sin_steering, CYCLE_LENGTH + speed * cos_steering);
   direction = degrees(angleSum(radians(direction),rotation));
}
GLfloat angleSum(GLfloat a, GLfloat b)
{
  a += b;
  if (a < 0) return a+2*PI;
  else if (a > 2*PI) return a-2*PI;
  else return a;
}
void drawFrame()
{

   glPushMatrix();
      glPushMatrix();
         glColor3f(0.0f,0.0f,1.0f); //Front gear
         glPushMatrix();
            glTranslatef(0.0f,0.0f,0.06f);
            glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
            gear(0.08f,0.3f,0.03f,30,0.03f);
         glPopMatrix();
            glColor3f(0.0f,0.0f,1.0f);
            glTranslatef(0.0f,0.0f,-0.2f);
            ZCylinder(0.08f,0.32f); //V shaped rod
      glPopMatrix();
      glRotatef(RIGHT_ANGLE,0.0f,0.0f,1.0f);
      XCylinder(ROD_RADIUS,RIGHT_ROD);
      glRotatef(MIDDLE_ANGLE-RIGHT_ANGLE,0.0f,0.0f,1.0f);
      XCylinder(ROD_RADIUS,MIDDLE_ROD);


      glTranslatef(MIDDLE_ROD,0.0f,0.0f);
      glRotatef(-MIDDLE_ANGLE,0.0f,0.0f,1.0f);
      glScalef(0.3f,ROD_RADIUS,0.25f);
      drawSeat();


   glPopMatrix();
   glPushMatrix();
   glColor3f(0.0f,0.0f,1.0f);
      glRotatef(-180.0f,0.0f,1.0f,0.0f);
      XCylinder(ROD_RADIUS,BACK_CONNECTOR); //Down rod
      glPushMatrix();
         glTranslatef(0.5f,0.0f,WHEEL_OFFSET);
         XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH);
      glPopMatrix();
      glPushMatrix();
         glTranslatef(0.5f,0.0f,-WHEEL_OFFSET);
         XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH);
      glPopMatrix();
   glPopMatrix();
   glPushMatrix();
      glTranslatef(-(BACK_CONNECTOR+RADIUS_WHEEL+TUBE_WIDTH),0.0f,0.0f);
      glPushMatrix();
         glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
         drawTyre();
         glColor3f(0.0f,0.0f,1.0f);
            gear(0.03f,0.15f,0.03f,20,0.03f);
         glColor3f(1.0f,1.0f,1.0f); //Upper rod
      glPopMatrix();
      glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f); //x cylinder
      glPushMatrix();
         glTranslatef(0.0f,0.0f,-WHEEL_OFFSET);
         XCylinder(ROD_RADIUS,WHEEL_LEN);
      glPopMatrix();
      glPushMatrix();
         glTranslatef(0.0f,0.0f,WHEEL_OFFSET);
         XCylinder(ROD_RADIUS,WHEEL_LEN);
      glPopMatrix();
      glTranslatef(WHEEL_LEN,0.0f,0.0f);
      XCylinder(ROD_RADIUS,CRANK_ROD);
      glTranslatef(CRANK_ROD,0.0f,0.0f);
      glRotatef(-LEFT_ANGLE,0.0f,0.0f,1.0f);
      XCylinder(ROD_RADIUS,TOP_LEN);
      glTranslatef(TOP_LEN,0.0f,0.0f);
      glRotatef(-FRONT_INCLINE,0.0f,0.0f,1.0f);
      glPushMatrix();
         glTranslatef(-0.1f,0.0f,0.0f);
         XCylinder(ROD_RADIUS,0.45f);
      glPopMatrix();
      glPushMatrix();
         glRotatef(-steering,1.0f,0.0f,0.0f); //z cylinder
         glTranslatef(-0.3f,0.0f,0.0f);
         glPushMatrix();
            glRotatef(FRONT_INCLINE,0.0f,0.0f,1.0f);
            glPushMatrix();
               glTranslatef(0.0f,0.0f,-HANDLE_ROD/2);
               ZCylinder(ROD_RADIUS,HANDLE_ROD);
            glPopMatrix();
            glPushMatrix();
               glColor3f(0.0f,0.0f,1.0f);
               glTranslatef(0.0f,0.0f,-HANDLE_ROD/2);
               ZCylinder(0.07f,HANDLE_ROD/4);
               glTranslatef(0.0f,0.0f,HANDLE_ROD*3/4);
               ZCylinder(0.07f,HANDLE_ROD/4);
               glColor3f(0.0f,0.0f,1.0f);
            glPopMatrix();
         glPopMatrix();
         glPushMatrix(); //x cylinder
            XCylinder(ROD_RADIUS,CRANK_ROD);
            glTranslatef(CRANK_ROD,0.0f,0.0f);
            glRotatef(CRANK_ANGLE,0.0f,0.0f,1.0f);
            glPushMatrix();
               glTranslatef(0.0f,0.0f,WHEEL_OFFSET);
               XCylinder(ROD_RADIUS,CRANK_RODS);
            glPopMatrix();
            glPushMatrix();
               glTranslatef(0.0f,0.0f,-WHEEL_OFFSET);
               XCylinder(ROD_RADIUS,CRANK_RODS);
            glPopMatrix();
            glTranslatef(CRANK_RODS,0.0f,0.0f);
            glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
            drawTyre();
         glPopMatrix();
      glPopMatrix();   /*   End of the rotation of the handle effect   */
   glPopMatrix();
}
void gear( GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
        GLint teeth, GLfloat tooth_depth )
{
    GLint i;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;
    const double pi = 3.14159264;
    r0 = inner_radius;
    r1 = outer_radius - tooth_depth/2;
    r2 = outer_radius + tooth_depth/2;
    da = 2.0*pi / teeth / 4.0;

    glNormal3f( 0.0, 0.0, 1.0 ); //Light angle
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
   angle = i * 2.0*pi / teeth;
   glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
   glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 );
   glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    }
    glEnd();
    glBegin( GL_QUADS );
    da = 2.0*pi / teeth / 4.0;
    for (i=0;i<teeth;i++) {
   angle = i * 2.0*pi / teeth;
   glVertex3f( r1*cos(angle),      r1*sin(angle),      width*0.5 );
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),   width*0.5 );
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), width*0.5 );
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    }
    glEnd();
    glNormal3f( 0.0, 0.0, -1.0 );
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
   angle = i * 2.0*pi / teeth;
   glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 );
   glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
   glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
    }
    glEnd();
    glBegin( GL_QUADS );
    da = 2.0*pi / teeth / 4.0;
    for (i=0;i<teeth;i++) {
   angle = i * 2.0*pi / teeth;
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     -width*0.5 );
   glVertex3f( r1*cos(angle),      r1*sin(angle),     -width*0.5 );
    }
    glEnd();
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<teeth;i++) {
   angle = i * 2.0*pi / teeth;
   glVertex3f( r1*cos(angle),      r1*sin(angle),      width*0.5 );
   glVertex3f( r1*cos(angle),      r1*sin(angle),     -width*0.5 );
   u = r2*cos(angle+da) - r1*cos(angle);
   v = r2*sin(angle+da) - r1*sin(angle);
   len = sqrt( u*u + v*v );
   u /= len;
   v /= len;
   glNormal3f( v, -u, 0.0 );
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),      width*0.5 );
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     -width*0.5 );
   glNormal3f( cos(angle), sin(angle), 0.0 );
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da),  width*0.5 );
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
   u = r1*cos(angle+3*da) - r2*cos(angle+2*da);
   v = r1*sin(angle+3*da) - r2*sin(angle+2*da);
   glNormal3f( v, -u, 0.0 );
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da),  width*0.5 );
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
   glNormal3f( cos(angle), sin(angle), 0.0 );
    }
    glVertex3f( r1*cos(0.0), r1*sin(0.0), width*0.5 );
    glVertex3f( r1*cos(0.0), r1*sin(0.0), -width*0.5 );
    glEnd();

    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
   angle = i * 2.0*pi / teeth;
   glNormal3f( -cos(angle), -sin(angle), 0.0 );
   glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
   glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
    }
    glEnd();
}
void drawChain()
{
   GLfloat depth;
   static int mode=0;
   glColor3f(0.0f,0.0f,0.0f);
   glEnable(GL_LINE_STIPPLE);
   mode=(mode+1)%2;
   if(mode==0 && speed>0)
      glLineStipple(1,0x1c47); //dash-dot-dash
   else if(mode==1 && speed>0)
      glLineStipple(1,0x00FF); //dashed
   glBegin(GL_LINES);
   for(depth=0.06f;depth<=0.12f;depth+=0.01f)
   {
      glVertex3f(-1.6f,0.15f,ROD_RADIUS);
      glVertex3f(0.0f,0.3f,depth);
      glVertex3f(-1.6f,-0.15f,ROD_RADIUS);
      glVertex3f(0.0f,-0.3f,depth);
   }
   glEnd();
   glDisable(GL_LINE_STIPPLE);
}
void drawSeat()
{
   glBegin(GL_POLYGON);//upper
   glColor3f(1.0,1.0,1.0);
       glVertex3f(-0.1f, 1.0f, -0.5f);
       glVertex3f( 1.0f, 1.0f, -0.3f);
       glVertex3f( 1.0f, 1.0f,  0.3f);
       glVertex3f(-0.1f, 1.0f,  0.5f);
       glVertex3f(-0.5f, 1.0f,  1.0f);
       glVertex3f(-1.0f, 1.0f,  1.0f);
       glVertex3f(-1.0f, 1.0f, -1.0f);
       glVertex3f(-0.5f, 1.0f, -1.0f);
   glEnd();
   glBegin(GL_POLYGON); //side
    glColor3f(0.0,0.0,1.0);
      glVertex3f(-0.1f, -1.0f, -0.5f);
      glVertex3f( 1.0f, -1.0f, -0.3f);
      glVertex3f( 1.0f, -1.0f,  0.3f);
      glVertex3f(-0.1f, -1.0f,  0.5f);
      glVertex3f(-0.5f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-0.5f, -1.0f, -1.0f);
   glEnd();
   glBegin(GL_QUADS);
      glVertex3f(1.0f,1.0f,-0.3f);
      glVertex3f(1.0f,1.0f,0.3f);
      glVertex3f(1.0f,-1.0f,0.3f);
      glVertex3f(1.0f,-1.0f,-0.3f);
      glVertex3f(1.0f,1.0f,0.3f);
      glVertex3f(-0.1f,1.0f,0.5f);
      glVertex3f(-0.1f,-1.0f,0.5f);
      glVertex3f(1.0f,-1.0f,0.3f);
      glVertex3f(1.0f,1.0f,-0.3f);
      glVertex3f(-0.1f,1.0f,-0.5f);
      glVertex3f(-0.1f,-1.0f,-0.5f);
      glVertex3f(1.0f,-1.0f,-0.3f);
      glVertex3f(-0.1f,1.0f,0.5f);
      glVertex3f(-0.5f,1.0f,1.0f);
      glVertex3f(-0.5f,-1.0f,1.0f);
      glVertex3f(-0.1f,-1.0f,0.5f);
      glVertex3f(-0.1f,1.0f,-0.5f);
      glVertex3f(-0.5f,1.0f,-1.0f);
      glVertex3f(-0.5f,-1.0f,-1.0f);
      glVertex3f(-0.1f,-1.0f,-0.5f);
      glVertex3f(-0.5f,1.0f,1.0f);
      glVertex3f(-1.0f,1.0f,1.0f);
      glVertex3f(-1.0f,-1.0f,1.0f);
      glVertex3f(-0.5f,-1.0f,1.0f);
      glVertex3f(-0.5f,1.0f,-1.0f);
      glVertex3f(-1.0f,1.0f,-1.0f);
      glVertex3f(-1.0f,-1.0f,-1.0f);
      glVertex3f(-0.5f,-1.0f,-1.0f);
      glVertex3f(-1.0f,1.0f,1.0f);
      glVertex3f(-1.0f,1.0f,-1.0f);
      glVertex3f(-1.0f,-1.0f,-1.0f);
      glVertex3f(-1.0f,-1.0f,1.0f);
   glEnd();
}
void drawPedals()
{
 //Right pedal
   glColor3f(0.0f,0.0f,1.0f);

   glPushMatrix();
      glTranslatef(0.0f,0.0f,0.105f);
      glRotatef(-pedalAngle,0.0f,0.0f,1.0f);
      glTranslatef(0.25f,0.0f,0.0f);

      glPushMatrix();
         glScalef(0.5f,0.1f,0.1f);
         glutSolidCube(1.0f);
      glPopMatrix();

   //Right Leg put
   glColor3f(1.0f,1.0f,1.0f);
      glPushMatrix();
         glTranslatef(0.25f,0.0f,0.15f);
         glRotatef(pedalAngle,0.0f,0.0f,1.0f);
         glScalef(0.2f,0.02f,0.3f);
         glutSolidCube(1.0f);
      glPopMatrix();
   glPopMatrix();

//Left pedal
glColor3f(0.0f,0.0f,1.0f);
   glPushMatrix();
      glTranslatef(0.0f,0.0f,-0.105f);
      glRotatef(180.0f-pedalAngle,0.0f,0.0f,1.0f);
      glTranslatef(0.25f,0.0f,0.0f);

      glPushMatrix();
         glScalef(0.5f,0.1f,0.1f);
         glutSolidCube(1.0f);
      glPopMatrix();

      //Left leg put pedal
      glColor3f(1.0f,1.0f,1.0f);
      glPushMatrix();
         glTranslatef(0.25f,0.0f,-0.15f);
         glRotatef(pedalAngle-180.0f,0.0f,0.0f,1.0f);
         glScalef(0.2f,0.02f,0.3f);
         glutSolidCube(1.0f);
      glPopMatrix();

   glPopMatrix();
   glColor3f(1.0f,0.0f,0.0f);
}
void drawTyre(void)
{
   int i;
   glColor3f(1.0f,0.0f,0.0f);
   glutSolidTorus(0.06f, 0.92f, 4, 30); //wheel ring
   glColor3f(1.0f,0.0f,0.0f); //wheel joint
   glPushMatrix();
   glTranslatef(0.0f,0.0f,-0.06f);
      ZCylinder(0.02f,0.12f);
   glPopMatrix();
   glutSolidTorus(0.02f, 0.02f, 3, 20);

   glColor3f(1.0f,1.0f,1.0f); //Spoke
   for(i=0;i<NUM_SPOKES;++i)
   {
      glPushMatrix();
         glRotatef(i*SPOKE_ANGLE,0.0f,0.0f,1.0f);
         glBegin(GL_LINES);
            glVertex3f(0.0f,0.02f,0.0f);
            glVertex3f(0.0f,0.86f,0.0f);
         glEnd();
      glPopMatrix();
   }
   glColor3f(0.0f,0.0f,0.0f); //Tube
   glutSolidTorus(TUBE_WIDTH,RADIUS_WHEEL,10,30);
}
void init()
{
   glClearColor(0, 0, 1, 1); //BG Color
   GLfloat mat_specular[]={1.0,1.0,1.0,1.0}; //Color shade
   GLfloat mat_shininess[]={100.0};
   GLfloat light_directional[]={1.0,1.0,1.0,1.0};
   GLfloat light_positional[]={1.0,1.0,1.0,0.0};
   GLfloat light_diffuse[]={1.0,1.0,1.0};
   reset();
   glShadeModel(GL_SMOOTH);
   glLightfv(GL_LIGHT0,GL_POSITION,light_directional);// To set individual light source parameter
   glLightfv(GL_LIGHT0,GL_AMBIENT,light_diffuse);
   glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
   glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);// Material highlight
   glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
   glColorMaterial(GL_FRONT,GL_DIFFUSE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_DEPTH_TEST);
}
void landmarks(void)
{
      GLfloat i;
      glColor3f(0.0f,1.0f,0.0f);
      glBegin(GL_QUADS);
      for(i=-100.0f ; i<100.0f ; i += 1.0f)
      {
         glVertex3f(-86.0f,-RADIUS_WHEEL,i);
         glVertex3f( 100.0f,-RADIUS_WHEEL,i);
         glVertex3f(i,-RADIUS_WHEEL,-100.0f);
         glVertex3f(i,-RADIUS_WHEEL,100.0f);
      }
      glEnd();
}
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_NORMALIZE); //Graphics back to normal form
   glPushMatrix();
      glRotatef(angley,1.0f,0.0f,0.0f);
      glRotatef(anglex,0.0f,1.0f,0.0f);
      glRotatef(anglez,0.0f,0.0f,1.0f);
     landmarks();
      glPushMatrix();
         glTranslatef(xpos,0.0f,zpos);
         glRotatef(direction,0.0f,1.0f,0.0f);
         drawFrame();
         drawChain();
         drawPedals();
      glPopMatrix();
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(camx,camy,camz,  camx, 0.0, 0.0,  0.0, 1.0, 0.0);
   glutSwapBuffers();
}
GLfloat Absolute(GLfloat a)
{
   if(a < 0.0f)
      return -a;
   else
      return a;
}
GLfloat degrees(GLfloat a)
{
   return a*180.0f/PI;
}
GLfloat radians(GLfloat a)
{
   return a*PI/180.0f;
}
void idle(void)
{
   updateScene();
   glutPostRedisplay();
}
void Camera(int key,int x,int y)
{
   switch(key)
   {
      case GLUT_KEY_UP: //Zoom in
         camz -= 0.1f;
         break;
      case GLUT_KEY_DOWN: //Zoom out
         camz += 0.1f;
         break;
      case GLUT_KEY_LEFT:
         camx -= 0.1f;
         break;
      case GLUT_KEY_RIGHT:
         camx += 0.1f;
         break;
   }
   glutPostRedisplay();
}
void reset()
{
   anglex=angley=anglez=0.0f;
   pedalAngle=steering=0.0f;
   Mouse=GLUT_UP;
   pedalAngle=speed=steering=0.0f;
   camx=camy=0.0f;
   camz=5.0f;
   xpos=zpos=0.0f;
   direction=0.0f;
}
void keyboard(unsigned char key,int x,int y)
{
   GLfloat r=0.0f;

   switch(key)
   {
      case 'r':
      case 'R':
         reset();
         break;
      case 'a':
      case 'A':
         if(steering < HANDLE_LIMIT)
               steering += INC_STEERING;
         break;
      case 'd':
      case 'D':
         if(steering > -HANDLE_LIMIT)
            steering -= INC_STEERING;
         break;
      case 'w':
      case 'W':
         speed += INC_SPEED;
         break;
      case 's':
      case 'S':
         speed -= INC_SPEED;
         break;
      case 27:
         exit(1);
   }
   pedalAngle += speed;
   if(speed < 0.0f)
      speed = 0.0f;
   if(pedalAngle < 0.0f)
      pedalAngle = 0.0f;
   if(pedalAngle >= 360.0f)
      pedalAngle -= 360.0f;
   glutPostRedisplay();
}
void mouse(int button,int state,int x,int y)
{
   switch(button)
   {
      case GLUT_LEFT_BUTTON:
         if(state==GLUT_DOWN)
         {
            Mouse=GLUT_DOWN;
            prevx=x;
            prevy=y;
         }
         if(state==GLUT_UP)
         {
            Mouse=GLUT_UP;
         }
         break;
      case GLUT_RIGHT_BUTTON:
         reset();
         break;
   }
   glutPostRedisplay();
}
void motion(int x,int y)
{
   if(Mouse==GLUT_DOWN)
   {
      int deltax,deltay;
      deltax=prevx-x;
      deltay=prevy-y;
      anglex += 0.5*deltax;
      angley += 0.5*deltay;
      if(deltax!=0 && deltay!=0)
         anglez += 0.5*sqrt(deltax*deltax + deltay*deltay);
      if(anglex < 0)
         anglex+=360.0;
      if(angley < 0)
         angley+=360.0;
      if(anglez < 0)
         anglez += 360.0;
      if(anglex > 360.0)
         anglex-=360.0;
      if(angley > 360.0)
         angley-=360.0;
      if(anglez > 360.0)
         anglez-=360.0;
   }
   else
   {
      Mouse=GLUT_UP;
   }
   prevx=x;
   prevy=y;
   glutPostRedisplay();
}

void reshape(int w,int h)
{
   glViewport(0,0,(GLsizei)w,(GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0,(GLfloat)w/(GLfloat)h,0.1,500.0);
   //Angle,Aspect Ratio,near plane,far plane
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
void glSetupFuncs(void)
{
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutSpecialFunc(Camera);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);

   glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}
void help(void)
{
    cout<<"|===================================================================|\n";
    cout<<"            CSE-3214 Computer Graphics Sessional \n";
    cout<<"                Project Name:-'Two wheeler' \n";
    cout<<" Submitted by 18204021,18204022,18204023,18204025,18204031,18204052\n";
    cout<<"|-------------------------------------------------------------------|\n";

   cout<<"       Use the following shortcuts to move the Two wheeler..."<<endl;
   cout<<"          ________________________________________________________ "<<endl;
   cout<<"         |'w' or 'W'to increase the speed                         |"<<endl;
   cout<<"         |'s' or 'S'to decrease the speed                         |"<<endl;
   cout<<"         |'a' or 'A'to rotate the handle anti-clockwise           |"<<endl;
   cout<<"         |'d' or 'D'to rotate the handle clockwise                |"<<endl;
   cout<<"         |'r' or 'R' or 'Mouse Right button' to reset the scene   |"<<endl;
   cout<<"         |--> Arrow keys to move the camera                       |"<<endl;
   cout<<"         |--> Mouse to move the scene                             |"<<endl;
   cout<<"         |________________________________________________________|"<<endl;
}
int main(int argc,char *argv[])
{
   help();
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition(50,50);
   glutInitWindowSize(WIN_WIDTH,WIN_HEIGHT);
   glutCreateWindow("Two wheeler");
   glSetupFuncs();
   init();
   glutMainLoop();
}
