#pragma once

/*
Espacios de trabajo
***/
using namespace std;

/*
Librerias
***/
#include "matplotpp.h"
#include <iostream>
#include <fstream>
#include <vector>

/*
Constantes
***/
#define LENGTH 3000

/*
Variables globales
***/
vector<double> sig1(LENGTH),
               sig2(LENGTH),
               sig3(LENGTH),
               mag1(LENGTH);

/*
Funciones
***/
class MP :public MatPlot{ 
    void DISPLAY(){
    vector<double> x(LENGTH); 
    for(int i=0;i<LENGTH;++i)  x[i]=i;

    subplot(2,1,1);
    plot(x,sig1);set("r");
    plot(x,sig2);set("g");
    plot(x,sig3);set("b");
    grid(1);

    subplot(2,1,2);
    plot(x,mag1);
    ylabel("Magnitud");
    //axis(0,600,-0,1000);
    grid(1);
  }
}mp;

void display(){ mp.display(); }
void reshape(int w,int h){ mp.reshape(w,h); }
void mouse(int button, int state, int x, int y ){ mp.mouse(button,state,x,y); }
void motion(int x, int y ){mp.motion(x,y); }
