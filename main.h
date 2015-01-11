#pragma  once

/*
Librerias
***/
#include "plot.h"
#include <iostream>
#include <fstream>
#include <string>
#include <fftw3.h>
#include <math.h>

/*
Constantes
***/

/*
Espacios de trabajo
***/

/*
Tipo de datos
***/
typedef unsigned int uint;
struct Spectrum{
	float *mag;	// Magnitud
	float *phs;	// Fase
};
struct DataSignal{
	float   *data,	// Datos de 1 canal
	      **dataC;	// Datos multicanal

	char   channel;	// Cantidad de canales
	size_t    size;	// Longitud de los datos

	float   med,	// Media de los datos
	        std,	// Desviacion estandar de los datos
	      	max,	// Maximo valor de los datos
	      	min;	// Minimo valor de los datos

	float *medC,	// Media de los datos por canal
	      *stdC,	// Desviación estandar por canal
	      *maxC,	// Maximo valor de los datos por canal
	      *minC;	// Minimo valor de los datos por canal

/*********************************************************
**  Cargar datos desde texto
**********************************************************/
	void loadTXT(string file,size_t _size, char _channel){
		fstream myfile;
		uint       i,j;

		channel = _channel;
		size    = _size;

		// Un solo canal
		if(_channel == 1){
			float aux = 0;
			float band;

			// Inicializacion de variables
			data = new float [_size];
			
			// Cargar
			myfile.open(file.c_str());
			for(i=0;i<_size;i++){
				myfile >> data[i];
				aux += data[i];
			}
			myfile.close();

			// Parametros de los datos
			med = aux/_size;
			aux = 0;
			min = data[0];
			max = data[0];
			for(i=0;i<_size;i++){
				band = data[i] - med;
				aux += band*band;

				if(min>data[i]) min = data[i];
				if(max<data[i]) max = data[i];
			}
			std = sqrt(aux/_size);
		}

		// Varios canales
		else{
			float *aux;
			float aux2;
			float band;

			// Inicializacion de variables
			dataC = new float*[_size   ];
			aux   = new float [_channel];
			medC  = new float [_channel];
			stdC  = new float [_channel];
			minC  = new float [_channel];
			maxC  = new float [_channel];
			for(i=0;i<_size;   i++) dataC [i] = new float [_channel];
			for(j=0;j<_channel;j++)   aux [j] = 0;	

			// Cargar
			myfile.open(file.c_str());
			for(i=0;i<_size;i++) for(j=0;j<_channel;j++){
				myfile >> dataC[i][j];
				aux[j] += dataC[i][j];
			}
			myfile.close();

			// Parametros de los datos
			for(j=0;j<_channel;j++){
				medC[j] = aux[j]/_size;
				aux2    = 0;
				minC[j] = dataC[0][j];
				maxC[j] = dataC[0][j];

				for(i=0;i<_size;i++){
					band = dataC[i][j] - medC[j];
					aux2 += band*band;

					if(minC[j]>dataC[i][j]) minC[j] = dataC[i][j];
					if(maxC[j]<dataC[i][j]) maxC[j] = dataC[i][j];
				}
				stdC[j] = sqrt(aux2/_size);
			}
		}
	}

	void normalize(){
		uint i,j;

		// Un solo canal
		if(channel == 1){
			for(i=0;i<size;i++) data[i] = (data[i]-med)/std; 
		}

		// Varios canales
		else{
			for(i=0;i<size;i++) for(j=0;j<channel;j++)
				 dataC[i][j] = (dataC[i][j]-medC[j])/stdC[j]; 
		}

	}

	Spectrum fft(){
		Spectrum outFFT;

		if(channel == 1){
			uint      i;
			double  *in;
			double band;

			// Inicializar variables
		  in         = new double [size];
		  outFFT.mag = new float  [size];
		  outFFT.phs = new float  [size];

			// Variables para FFTW
		  fftw_complex *out;
			fftw_plan    plan;

			// Creando plan FFTW
			out  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
			plan = fftw_plan_dft_r2c_1d(size, in, out, FFTW_ESTIMATE);

			// Cargando datos (float2double)
			for(i=0;i<size;i++) in[i] = (double)data[i];

			// Ejecutar FFTW
			fftw_execute(plan);

			// Calculando Magnitud y fase
			for(i=0;i<size;i++){
				band = out[i][0]*out[i][0] + out[i][1]*out[i][1];
				outFFT.mag[i] = sqrt(band);
				outFFT.phs[i] = atan2 (out[i][1],out[i][0]) * 180 / PI;
			}
		}
		else cout << "Error en FFT. Se esperaba 1 canal" << endl;

		return outFFT;
	}

	Spectrum fft(char _channel){
		Spectrum outFFT;

		if(channel > 1){
			uint      i;
			double  *in;
			double band;

		    // Inicializar variables
		    in         = new double [size];
		    outFFT.mag = new float  [size];
		    outFFT.phs = new float  [size];

			// Variables para FFTW
		  fftw_complex *out;
			fftw_plan    plan;

			// Creando plan FFTW
			out  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
			plan = fftw_plan_dft_r2c_1d(size, in, out, FFTW_ESTIMATE);

			// Cargando datos (float2double)
			for(i=0;i<size;i++) in[i] = (double)dataC[i][_channel];

			// Ejecutar FFTW
			fftw_execute(plan);

			// Calculando Magnitud y fase
			for(i=0;i<size;i++){
				band = out[i][0]*out[i][0] + out[i][1]*out[i][1];
				outFFT.mag[i] = sqrt(band);
				outFFT.phs[i] = atan2 (out[i][1],out[i][0]) * 180 / PI;
			}
		}
		else cout << "Error en FFT. Se esperaba mas de 1 canal" << endl;

		return outFFT;
	}

	float *fftMag(){
		float *outFFT;

		if(channel == 1){
			uint      i;
			double  *in;
			double band;

		  // Inicializar variables
		  in     = new double [size];
		  outFFT = new float  [size];

			// Variables para FFTW
		  fftw_complex *out;
			fftw_plan    plan;

			// Creando plan FFTW
			out  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
			plan = fftw_plan_dft_r2c_1d(size, in, out, FFTW_ESTIMATE);

			// Cargando datos (float2double)
			for(i=0;i<size;i++) in[i] = (double)data[i];

			// Ejecutar FFTW
			fftw_execute(plan);

			// Calculando Magnitud y fase
			for(i=0;i<size;i++){
				band = out[i][0]*out[i][0] + out[i][1]*out[i][1];
				outFFT[i] = sqrt(band);
			}
		}
		else cout << "Error en FFT. Se esperaba 1 canal" << endl;

		return outFFT;
	}

	float *fftMag(char _channel){
		float *outFFT;

		if(channel > 1){
			uint      i;
			double  *in;
			double band;

		    // Inicializar variables
		    in     = new double [size];
		    outFFT = new float  [size];

			// Variables para FFTW
		  fftw_complex *out;
			fftw_plan    plan;

			// Creando plan FFTW
			out  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
			plan = fftw_plan_dft_r2c_1d(size, in, out, FFTW_ESTIMATE);

			// Cargando datos (float2double)
			for(i=0;i<size;i++) in[i] = (double)dataC[i][_channel];

			// Ejecutar FFTW
			fftw_execute(plan);

			// Calculando Magnitud y fase
			for(i=0;i<size;i++){
			band = out[i][0]*out[i][0] + out[i][1]*out[i][1];
			outFFT[i] = sqrt(band);
			}
		}
		else cout << "Error en FFT. Se esperaba mas de 1 canal" << endl;

		return outFFT;
	}
};

/*
Funciones
***/
