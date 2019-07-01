#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<bits/stdc++.h>
#include <windows.h>

#define pi 3.141529
const int font=(int)GLUT_BITMAP_9_BY_15;
double cubeLen=35,sphereRad=7;
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
int debug;
int mapFlag=0;
double playerRad=0;
int playerRadFlag=0;
int start = clock();
struct Moon
{

    double x,y,z,rad;
    Moon(double a,double b,double c,double r)
    {
        x=a;
        y=b;
        z=c;
        rad=r;
    }
};
struct point
{
    double x,y,z;
};
class vec
{

public:
    double x,y,z;
    vec(double a,double b,double c)
    {
        x=a;
        y=b;
        z=c;
    }


};
vec pos(475,475,25);
vec l(-sqrt(0.5),-sqrt(0.5),0);
vec r(-sqrt(0.5),sqrt(0.5),0);
vec u(0,0,1);
vec temp_pos(1000,1000,1000);
vec temp_l(0,0,0);
Moon moon(-200,-200,200,20);
void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
    }
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }
    glEnd();
}



void drawSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0; j<slices; j++)
        {

            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }
            glEnd();
        }
    }
}

void drawWallGeneric(double ax,double ay,double bx,double by,double height,double width)
{


    double dis = sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
    double x1,y1,x2,y2;
    x1=ax+width*(by-ay)/dis;
    y1=ay+width*(ax-bx)/dis;
    x2=bx+width*(by-ay)/dis;
    y2=by+width*(ax-bx)/dis;
    glPushMatrix();
    {
        glBegin(GL_QUADS);
        {

            glVertex3f( ax,ay,0);
            glVertex3f( bx,by,0);
            glVertex3f(bx,by,height);
            glVertex3f(ax,ay,height);
        }
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    {
        //glTranslatef(width,0,0);
        glBegin(GL_QUADS);
        {
            glVertex3f( x1, y1,0);
            glVertex3f( x2,y2,0);
            glVertex3f(x2,y2,height);
            glVertex3f(x1,y1,height);
        }
        glEnd();
    }
    glPopMatrix();



    glPushMatrix();
    {
        glTranslatef(0,0,height);
        glBegin(GL_QUADS);
        {
            glVertex3f( ax, ay,0);
            glVertex3f( bx,by,0);
            glVertex3f(x2,y2,0);
            glVertex3f(x1, y1,0);
        }
        glEnd();
    }
    glPopMatrix();


    glPushMatrix();
    {

        glBegin(GL_QUADS);
        {
            glVertex3f( ax, ay,0);
            glVertex3f( bx,by,0);
            glVertex3f(x2,y2,0);
            glVertex3f(x1, y1,0);
        }
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    {

        glBegin(GL_QUADS);
        {
            glVertex3f( ax, ay,0);
            glVertex3f( ax,ay,height);
            glVertex3f(x1,y1,height);
            glVertex3f(x1, y1,0);
        }
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    {

        glBegin(GL_QUADS);
        {
            glVertex3f( bx, by,0);
            glVertex3f( bx,by,height);
            glVertex3f(x2,y2,height);
            glVertex3f(x2, y2,0);
        }
        glEnd();
    }
    glPopMatrix();






}
void drawOrion()
{

    glPopMatrix();
    glPushMatrix();
    {
        glTranslated(250,300,1000);
        drawSphere(6,50,50);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslated(200,300,1000);
        drawSphere(6,50,50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(150,300,1000);
        drawSphere(6,50,50);
    }
    glPopMatrix();

    glColor3f(1,0.7,0);
    glPushMatrix();
    {
        glTranslated(450,600,1000);
        drawSphere(6,50,50);
    }
    glColor3f(1,1,1);
    glPopMatrix();
    glPushMatrix();
    {

        glTranslated(-250,600,1000);
        drawSphere(6,50,50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(500,-150,1000);
        drawSphere(6,50,50);
    }

    glPopMatrix();
    glPushMatrix();
    {
        glTranslated(-100,-175,1000);
        drawSphere(6,50,50);
    }
    glPopMatrix();

}
void buildTheMaze()
{
    //draw destination
    glColor3f(0.0,0.0,1.0);
    drawWallGeneric(-250,-250,-300,-250,50,50);
    //draw player position
    glPushMatrix();
    {
        glTranslated(temp_pos.x,temp_pos.y,temp_pos.z);
        drawSphere(playerRad,50,50);
        glTranslated(-temp_pos.x,-temp_pos.y,-temp_pos.z);
    }
    glPopMatrix();

    glColor3f(1.0,0.0,0.0);
    //border
    drawWallGeneric(-500,-500,-500,500,50,5);
    drawWallGeneric(-500,-500,500,-500,50,5);
    drawWallGeneric(500,500,-500,500,50,5);
    drawWallGeneric(500,500,500,-500,50,5);
    //main maze
    drawWallGeneric(400,350,400,0,50,5);
    drawWallGeneric(400,400,0,400,50,5);
    drawWallGeneric(300,300,-300,-300,50,5);



}
void forceLookForward()
{

    l.z=0;
    double d;
    d=sqrt(l.x*l.x+l.y*l.y);
    l.x=l.x/d;
    l.y=l.y/d;
    if(r.x*l.y-r.y*l.x>0)
    {
        u.x=r.y*l.z-r.z*l.y;
        u.y=r.z*l.x-r.x*l.z;
        u.z=r.x*l.y-r.y*l.x;
    }
    else
    {
        u.x=-r.y*l.z+r.z*l.y;
        u.y=-r.z*l.x+r.x*l.z;
        u.z=-r.x*l.y+r.y*l.x;

        l.x=-l.x;
        l.y=-l.y;

    }
}
void output(int x, int y,int z, float r, float g, float b, void *font, std::string str)
{
    glViewport(400,400,0,0);
    glColor3f( r, g, b );
    glRasterPos2f(0.0,0.0);
    int len, i;
    len = (int)str.size();
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}
void drawSS()
{





    glViewport(0,0,600,500);
    glColor3f(0.30,0.20,0.10);   //ground
    drawWallGeneric(-500,-500,-500,500,0,1000);


    glColor3f(1,1,1);
    glPushMatrix();
    {
        glTranslated(moon.x,moon.y,moon.z);//moon
        drawSphere(moon.rad,50,50);
    }
    drawOrion();
    buildTheMaze();
    glPushMatrix();
    {
        int n=((clock()-start)/1000);
        int sec,mn=0,hour=0;
        sec=n%60;
        mn=n/60;
        hour=n/3600;
        std::stringstream ss;
        ss << sec;
        std::string strSec = ss.str();
        ss.str( std::string() );
        ss.clear();
        ss << mn;
        std::string strMn = ss.str();
        ss.str( std::string() );
        ss.clear();
        ss<<hour;
        std::string strHour = ss.str();
        std:: string str=strHour+"::"+strMn+"::"+strSec;
        output(0,0,0,0,1,0,(void *)font,str);
    }
    glPopMatrix();

}

void keyboardListener(unsigned char key, int x,int y)
{
    double angle=0.5;

    switch(key)
    {

    case '1':
        angle=0.05;
        r.x=r.x*cos(angle)+l.x*sin(angle);
        r.y=r.y*cos(angle)+l.y*sin(angle);
        r.z=r.z*cos(angle)+l.z*sin(angle);
        //now, l=u*r
        l.x=u.y*r.z-u.z*r.y;
        l.y=u.z*r.x-u.x*r.z;
        l.z=u.x*r.y-u.y*r.x;
        break;

    case '2':
        angle=-0.05;
        r.x=r.x*cos(angle)+l.x*sin(angle);
        r.y=r.y*cos(angle)+l.y*sin(angle);
        r.z=r.z*cos(angle)+l.z*sin(angle);
        //now, l=u*r
        l.x=u.y*r.z-u.z*r.y;
        l.y=u.z*r.x-u.x*r.z;
        l.z=u.x*r.y-u.y*r.x;
        break;
    case '3'://look up
        angle=0.05;
        l.x=l.x*cos(angle)+u.x*sin(angle);
        l.y=l.y*cos(angle)+u.y*sin(angle);
        l.z=l.z*cos(angle)+u.z*sin(angle);
        //now, u=r*l
        u.x=r.y*l.z-r.z*l.y;
        u.y=r.z*l.x-r.x*l.z;
        u.z=r.x*l.y-r.y*l.x;
        break;
    case '4'://look down
        angle=-0.05;
        l.x=l.x*cos(angle)+u.x*sin(angle);
        l.y=l.y*cos(angle)+u.y*sin(angle);
        l.z=l.z*cos(angle)+u.z*sin(angle);
        //now, u=r*l
        u.x=r.y*l.z-r.z*l.y;
        u.y=r.z*l.x-r.x*l.z;
        u.z=r.x*l.y-r.y*l.x;
        break;

    case 'w':
        double unit;

        unit=sqrt(u.x*u.x+u.y*u.y+u.z*u.z);
        pos.x=pos.x+u.x/unit;
        pos.y=pos.y+u.y/unit;
        pos.z=pos.z+u.z/unit;
        break;
    case 's':

        unit=sqrt(u.x*u.x+u.y*u.y+u.z*u.z);
        pos.x=pos.x-u.x/unit;
        pos.y=pos.y-u.y/unit;
        pos.z=pos.z-u.z/unit;
        break;
    case 'x':
        if(debug==0)
            debug=1;
        else
            debug=0;
        break;
    case 'l':
        forceLookForward();
        break;
    case 'm':
        if(mapFlag==0)
        {
            mapFlag=1;
            temp_pos.x=pos.x;
            temp_pos.y=pos.y;
            temp_pos.z=pos.z;
            temp_l.x=l.x;
            temp_l.y=l.y;
            temp_l.z=l.z;
            pos.x=0;
            pos.y=0;
            pos.z=900;
            l.x=0;
            l.y=0;
            l.z=-1;

            angle=0.05;
            l.x=l.x*cos(angle)+u.x*sin(angle);
            l.y=l.y*cos(angle)+u.y*sin(angle);
            l.z=l.z*cos(angle)+u.z*sin(angle);
            //now, u=r*l
            u.x=r.y*l.z-r.z*l.y;
            u.y=r.z*l.x-r.x*l.z;
            u.z=r.x*l.y-r.y*l.x;

            moon.rad=0;



        }
        else
        {
            mapFlag=0;
            pos.x=temp_pos.x;
            pos.y=temp_pos.y;
            pos.z=temp_pos.z;
            l.x=temp_l.x;
            l.y=temp_l.y;
            l.z=temp_l.z;

            angle=0.05;
            l.x=l.x*cos(angle)+u.x*sin(angle);
            l.y=l.y*cos(angle)+u.y*sin(angle);
            l.z=l.z*cos(angle)+u.z*sin(angle);
            //now, u=r*l
            u.x=r.y*l.z-r.z*l.y;
            u.y=r.z*l.x-r.x*l.z;
            u.z=r.x*l.y-r.y*l.x;
            moon.rad=10;

        }

    default:
        break;
    }
}


void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        double unit;
        if(pos.z==25)
        {
            forceLookForward();

            unit=sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
            pos.x=pos.x-l.x/unit;
            pos.y=pos.y-l.y/unit;
            pos.z=pos.z-l.z/unit;

        }
        break;
    case GLUT_KEY_UP:		// up arrow key
        if(pos.z==25)
        {
            forceLookForward();
            unit=sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
            pos.x=pos.x+l.x/unit;
            pos.y=pos.y+l.y/unit;
            pos.z=pos.z+l.z/unit;
        }

        break;

    case GLUT_KEY_RIGHT:

        unit=sqrt(r.x*r.x+r.y*r.y+r.z*r.z);
        pos.x=pos.x+r.x/unit;
        pos.y=pos.y+r.y/unit;
        pos.z=pos.z+r.z/unit;
        break;
    case GLUT_KEY_LEFT:

        unit=sqrt(r.x*r.x+r.y*r.y+r.z*r.z);
        pos.x=pos.x-r.x/unit;
        pos.y=pos.y-r.y/unit;
        pos.z=pos.z-r.z/unit;

        break;

    case GLUT_KEY_PAGE_UP:

        unit=sqrt(u.x*u.x+u.y*u.y+u.z*u.z);
        pos.x=pos.x+u.x/unit;
        pos.y=pos.y+u.y/unit;
        pos.z=pos.z+u.z/unit;

        break;
    case GLUT_KEY_PAGE_DOWN:

        unit=sqrt(u.x*u.x+u.y*u.y+u.z*u.z);
        pos.x=pos.x-u.x/unit;
        pos.y=pos.y-u.y/unit;
        pos.z=pos.z-u.z/unit;
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:


        break;
    case GLUT_KEY_END:

        break;

    default:
        break;
    }
}


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}



void display()
{

    ///clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	///color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    ///load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    ///initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    gluLookAt(pos.x,pos.y,pos.z,pos.x+l.x,pos.y+l.y,pos.z+l.z,u.x,u.y,u.z);
    //gluLookAt(0,0,200,	0,0,0,	0,1,0);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    //drawAxes();
    //drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

    //drawSphere(30,24,20);




    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    if(debug==1)
    {
        std::cout<<l.x<<" "<<l.y<<" "<<l.z<<"\n";
        std::cout<<r.x<<" "<<r.y<<" "<<r.z<<"\n";
        std::cout<<u.x<<" "<<u.y<<" "<<u.z<<"\n";
    }
    if(mapFlag==1&&playerRadFlag==0&&playerRad<=20)
    {

        playerRad++;
        if(playerRad==20)
        {
            playerRadFlag=1;
        }
    }
    else if(mapFlag==1&&playerRadFlag==1&&playerRad>0)
    {
        playerRad--;
        if(playerRad==1)
        {
            playerRadFlag=0;
        }
    }
    else if(mapFlag==0)
    {
        playerRad=0;
        playerRadFlag=0;
    }
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{

    //codes for initialization
    drawgrid=1;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;
    debug=0;
    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();
}
