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

#include "Commun/VECommun.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

char tabstr[999][255];
int curstr;

pthread_mutex_t infolock;

void * thread_server(void * ptr) {
  char * sendbuf= new char[255];
  char * buf= new char[255];
  unsigned int i;
  NetworkServer * sup_server = new NetworkServer();

  sup_server->init_server(9999);

  while (1) {
    fflush(NULL);
    sup_server->wait_connection();
    int done=0;
    int posy=0;
    while (done==0) {
      strcpy(buf,sup_server->read_line());
      if (strcmp(buf,"PING")==0) {
        strcpy(sendbuf,"SUP");
        sup_server->send_line(sendbuf);
      }
      if (strcmp(buf,"DISP_INFO")==0) {
         pthread_mutex_lock(&infolock);
         strcpy(buf,sup_server->read_line());
         strcpy(tabstr[curstr],buf);
         curstr++;
         pthread_mutex_unlock(&infolock);
      }
      if (strcmp(buf,"DISC")==0) {
        done=1;
      }
    }
    sup_server->close_connection();
  }
  delete buf;
  delete sendbuf;
  return NULL;
}



int main(int argc,char ** argv) {
  Display * dis = XOpenDisplay((char *) 0);
  int screen = DefaultScreen(dis);
  unsigned long backpx = BlackPixel(dis,screen);
  unsigned long frontpx = WhitePixel(dis,screen);
  Window win = XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0,640,350,5,frontpx,backpx);
 XSetStandardProperties(dis,win,"Virtual Exec Supervision Screen (c) O.Moulin","VirtualExecGS",None,NULL,0,NULL);
  XSelectInput(dis,win,ExposureMask | ButtonPressMask);
  GC gc=XCreateGC(dis,win,0,0);

  XSetBackground(dis,gc,backpx);
  XSetForeground(dis,gc,frontpx);
  XMapRaised(dis,win);
  XClearWindow(dis,win);

  curstr=0;

  pthread_t serv;
  pthread_create(&serv, NULL, thread_server, NULL);

  XEvent event;
  int ydisp=0;
  while (1) {
    XNextEvent(dis,&event);
    if(event.type==ButtonPress) {
      if (event.xbutton.x>630 && event.xbutton.y<10) {
        if (ydisp>0) {
            ydisp=ydisp-1;
        }
      }
      if (event.xbutton.x>630 && event.xbutton.y>320 && event.xbutton.y<330) {
        if (ydisp<curstr) {
           ydisp=ydisp+1;
        }
      }
      if (event.xbutton.x>530 && event.xbutton.y>330 && event.xbutton.x<580) {
        ydisp=0;
        curstr=0;
      }
      if (event.xbutton.x>580 && event.xbutton.y>330 && event.xbutton.x<630) {
        exit(0);
      }
    }
    pthread_mutex_lock(&infolock);
    XClearWindow(dis,win);
    XSetForeground(dis,gc,0x7F7F7F);
    XDrawLine(dis,win,gc,630,0,630,350);
    XDrawLine(dis,win,gc,640,0,640,350);
    XDrawLine(dis,win,gc,630,10,640,10);
    XDrawLine(dis,win,gc,630,320,640,320);
    XDrawLine(dis,win,gc,630,330,640,330);

    XDrawLine(dis,win,gc,0,330,630,330);
    XDrawLine(dis,win,gc,0,330,0,350);
    XDrawLine(dis,win,gc,0,349,630,349);
    XDrawLine(dis,win,gc,580,330,580,350);
    XDrawString(dis,win,gc,595,346,"Quit",4);

    XDrawLine(dis,win,gc,530,330,530,350);
    XDrawString(dis,win,gc,545,346,"Clear",5);

    for (int i=ydisp;i<ydisp+27 && i<curstr;i++) {
      if(strcmp(tabstr[i],"--")==0) {
        XSetForeground(dis,gc,0xF5F5F5);
        XDrawLine(dis,win,gc,100,(i-ydisp+1)*12-6,540,(i-ydisp+1)*12-6);
      }
      else {
        if (strncmp(tabstr[i],"ENGINE",6)==0) {
          XSetForeground(dis,gc,0xFF0101);
        }
        else {
          XSetForeground(dis,gc,0xFFFFFF);
        }
        XDrawString(dis,win,gc,0,(i-ydisp+1)*12,tabstr[i],strlen(tabstr[i]));
      }
    }
    pthread_mutex_unlock(&infolock);
  }

  return 0;
}
