/*
This program has been created by Olivier Moulin and is distributed under
the GNU public license.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "GraphicDevice/GraphicDevice.h"

typedef GLfloat point2[2];

void GraphicDevice::display(){
  for (int i =0;i<500;i++){
    for (int j=0;j<500;j++){
      if(GraphicDevice::buffer[i][j]>0) {
        glBegin ( GL_POINTS );
        point2 p = { i, j };
        glVertex2fv ( p );
        glEnd ();
        glFlush();
      }
    }
  }
}

void GraphicDevice::set_pixel(int x, int y){
  GraphicDevice::buffer[x][y]=1;
}
void GraphicDevice::LocalInit() {
  strcpy(descriptor,"Graphic Screen (c) O.Moulin");
  deviceID = 10;
  maxinterrupt = 1;
  interrupt = new unsigned char[1];
  interrupt[0]=10;
  memoryStart=1000000;
  memorySpace=24000;
  char *myargv [1];
  int myargc=1;
  myargv [0]=strdup ("Myappname");
  glutInit (&myargc,myargv);
  glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB );
  glutInitWindowSize ( 500, 500 );
  glutInitWindowPosition ( 0, 0 );
  glutCreateWindow ( "Virtual Exec (Graphic Terminal)" );
  glutDisplayFunc ( display );
  glClearColor ( 1.0, 1.0, 1.0, 1.0 );
  glColor3f ( 1.0, 0.0, 0.0 );
  glMatrixMode ( GL_PROJECTION );
  glLoadIdentity ( );
  gluOrtho2D ( 0.0, 500.0, 0.0, 500.0 );
  glMatrixMode ( GL_MODELVIEW );
  for(int i=0;i<500;i++){
    for(int j=0;j<500;j++){
      GraphicDevice::buffer[i][j]=0;
    }
  }
}


// The Text Screen Driver is linked on the 1 interrupt
//
// The register 2 give the choice beetween the functions called for this device
// The register 3,4,5,6,7,8 give the parameters.

// Function 1 :
// Clear the Screen

// Function 2 :
// Display a Pixel at a point
// reg 3hi : x
// reg 3low : y
// reg 4 : color

// Function 3 :
// Display a line beetween two points
// reg 3hi : x
// reg 3low : y
// reg 4hi : x2
// reg 4low : y2
// reg 5 : color

//Function 4 :
// Display a rectangle beetween two points
// reg 3hi : x
// reg 3low : y
// reg 4hi : x2
// reg 4low : y2
// reg 5 : color

//Function 5 :
// Display a character at a point
// reg 3hi : x
// reg 3low : y
// reg 4 : char
// reg 5 : color

//Function 6 :
// Display a String 0 terminated at a point
// reg 3hi : x
// reg 3low : y
// reg 4 : the memory address of the string
// reg 5 : color

void GraphicDevice::ExecInterrupt() {
  if (active) {
    if (linkreg->GetRegister(2)->GetLow()==1) {
      glClear ( GL_COLOR_BUFFER_BIT );
    }
    if (linkreg->GetRegister(2)->GetLow()==2) {
      set_pixel((int)linkreg->GetRegister(3)->GetHigh(),(int)linkreg->GetRegister(3)->GetLow());
    }
    if (linkreg->GetRegister(2)->GetLow()==3) {
    }
    if (linkreg->GetRegister(2)->GetLow()==4) {
    }
    if (linkreg->GetRegister(2)->GetLow()==5) {
    }
    if (linkreg->GetRegister(2)->GetLow()==6) {
    }
  }
}

void GraphicDevice::LocalShutdown() {
}

void GraphicDevice::LocalStart() {
  this->display();
}

void GraphicDevice::LocalStop() {
}
