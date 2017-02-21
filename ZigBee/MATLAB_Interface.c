/*
 *
 *	This is a simple program that illustrates how to call the MATLAB
 *	Engine functions from NetSim C Code.
 *
 */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#include "mat.h"
#include "mex.h"



char buf[512];
Engine *ep;
int status;
mxArray *k=NULL, *x=NULL, *t=NULL, *tnoise=NULL, *A=NULL, *B=NULL, *h1=NULL;
mxArray *out;
double *result[2];

double fn_netsim_matlab_init()
{
	/*
	 * Start the MATLAB engine 
	 */
	if (!(ep = engOpen(NULL))) {
		MessageBox ((HWND)NULL, (LPCWSTR)"Can't start MATLAB engine", 
			(LPCWSTR) "MATLAB_Interface.c", MB_OK);
		exit(-1);
	}
	engEvalString(ep,"desktop");
	sprintf(buf,"cd 'F:\\CUSTOM CODES\\Temperature_Sensors_in_NetSim\\Simulation_Temperature_Sensors\\ZigBee'");
	status=engEvalString(ep,buf);

	sprintf(buf,"h1=figure('Name','Actual Sensed Temperature  and  Sensed Temperature with Noise','Position',[100 100 1000 500],'NumberTitle','off')");
	status=engEvalString(ep,buf);


	

	return 0;
}

double **fn_netsim_matlab_run(int plot_flag, char *positions, char *temp, char *temp_noise, int sensor_count, double simulation_time, double event_time, int sensor_id)
{
	
	//write your own implementation here
	
	

	//engPutVariable(ep,"x",x);
	status=engEvalString(ep,positions);

	//engPutVariable(ep,"t",t);
	status=engEvalString(ep,temp);

	//engPutVariable(ep,"tnoise",tnoise);
	status=engEvalString(ep,temp_noise);

	sprintf(buf,"[A,B]=temperature(%d,x,t,tnoise,h1,%d,%f,%f,%d)", plot_flag,sensor_count, event_time, simulation_time, sensor_id);
	status=engEvalString(ep,buf);

	out=engGetVariable(ep,"A");//contains the sensed temperature without noise
	result[0]=mxGetPr(out);

	out=engGetVariable(ep,"B");//contains the sensed temperature with noise
	result[1]=mxGetPr(out);

	
	return result;
}

double fn_netsim_matlab_finish()
{
	status=engEvalString(ep,"exit");
	
	return 0;
}

