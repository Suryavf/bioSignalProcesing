#include "main.h"
#include <ctime>

int main(int argc, char* argv[]){
/*********************************************************
**  Definicion de variables principales
**********************************************************/
	float a, b, c;
	uint  i;

/*********************************************************
**  Cargar datos
**********************************************************/
    DataSignal ekg;
    ekg.loadTXT("limbs.txt",LENGTH,3);
    ekg.normalize();

	float *mag;
	mag = new float[LENGTH];
	mag = ekg.fftMag(0);

    for(i=0;i<LENGTH;i++){
		sig1[i] = ekg.dataC[i][0];
	    sig2[i] = ekg.dataC[i][1];
	    sig3[i] = ekg.dataC[i][2];
	    mag1[i] = mag[i];
	}
	
/*********************************************************
**  Plot
**********************************************************/
    glutInit(&argc, argv);
    glutCreateWindow(100,100,600,400);
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutMotionFunc( motion );
    glutMouseFunc( mouse );
    glutMainLoop();

    system("pause");
    return 0;
}
