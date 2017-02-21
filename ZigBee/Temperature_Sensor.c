#include "main.h"
#include "List.h"
#include "802_15_4.h"



int sensor_count;
//double **arr;

double **DEV_POS;
double **s_temperature;
double **s_temperature_noise;
char *pos,*temp,*temp_noise;
double **mat_arr;




double fn_NetSim_Temperature_Sensor_run(int sensor_id)
{
	
	int i=0,j=0;
	double *sensed_temp=NULL;
	sensor_count=NETWORK->nDeviceCount-1;

	for(i=0;i<sensor_count;i++)
	{
		DEV_POS[i][0]=NETWORK->ppstruDeviceList[i]->pstruDevicePosition->X;
		DEV_POS[i][1]=NETWORK->ppstruDeviceList[i]->pstruDevicePosition->Y;
	}

	
	sprintf(pos,"x=[");
	sprintf(temp,"t=[");
	sprintf(temp_noise,"tnoise=[");
	for(i=0;i<sensor_count;i++)
	{
		sprintf(temp,"%s%.2f",temp,s_temperature[i][0]);
		sprintf(temp_noise,"%s%.2f",temp_noise,s_temperature_noise[i][0]);
		sprintf(pos,"%s%.2f %.2f",pos,DEV_POS[i][0],DEV_POS[i][1]);
		if(i!=(sensor_count-1))
		{
			sprintf(temp,"%s;",temp);
			sprintf(temp_noise,"%s;",temp_noise);
			sprintf(pos,"%s;",pos);
		}
	}
	sprintf(pos,"%s]",pos);
	sprintf(temp,"%s]",temp);
	sprintf(temp_noise,"%s]",temp_noise);
	
	

	
	mat_arr=fn_netsim_matlab_run(0, pos, temp, temp_noise, sensor_count, (NETWORK->pstruSimulationParameter->dVal)/1000000,  pstruEventDetails->dEventTime, sensor_id);
	*s_temperature[sensor_id-1]=*mat_arr[0];
	//*s_temperature_noise[sensor_id-1]=*mat_arr[1];
	if(sensor_id==sensor_count)
	{
		fn_NetSim_Temperature_Sensor_WriteLog(sensor_count);
	}
	//return *s_temperature_noise[sensor_id-1];
	return *mat_arr[1];
}


void fn_NetSim_Temperature_Sensor_Init()
{
	int i=0,j=0;
	fopen("Temperature_Sensor_Log.csv","w+");
	sensor_count=NETWORK->nDeviceCount-1;
	
	pos = (char*)calloc(sensor_count*15,sizeof(char));// To form a matrix containing the coordinates of all the sensros.
	temp = (char*)calloc(sensor_count*8,sizeof(char));// To form a matrix containing the sensed temperature of all the sensors.
	temp_noise = (char*)calloc(sensor_count*8,sizeof(char));// To form a matrix containing the sensed temperature with noise of all the sensors.
	mat_arr = (double**)calloc(2,sizeof(*mat_arr));
	DEV_POS=(double**)calloc(sensor_count, sizeof(*DEV_POS));
	s_temperature=(double**)calloc(sensor_count, sizeof(*s_temperature));
	s_temperature_noise=(double**)calloc(sensor_count, sizeof(*s_temperature_noise));
	for(i=0;i<sensor_count;i++)
	{
		DEV_POS[i]=(double*)calloc(2,sizeof*(DEV_POS[i]));
		s_temperature[i]=(double*)calloc(1,sizeof*(s_temperature[i]));
		s_temperature_noise[i]=(double*)calloc(1,sizeof*(s_temperature_noise[i]));
	}
	mat_arr[0]=(double*)calloc(1,sizeof*(mat_arr[0]));//to store the values returned from matlab
	mat_arr[1]=(double*)calloc(1,sizeof*(mat_arr[1]));
	

}

int fn_NetSim_Temperature_Sensor_WriteLog(int scount)
{
	int i=0;
	FILE *fp=fopen("Temperature_Sensor_Log.csv","a+");
	if(!fp)
	{
	fprintf(stderr,"\n***\nUnable to write Temperature_Sensor_Log.csv\n***\n");
	return 0;
	}
	fprintf(fp,"\nSensor_Log at Time: %f\n\n", pstruEventDetails->dEventTime);
	fprintf(fp,"SOURCE ID,ACTUAL TEMPERATURE,TEMPERATURE VALUES RECEIVED AT SINK\n");
	for(i=0;i<scount;i++)
	{
		fprintf(fp,"%d,%f,%f\n",i+1,s_temperature[i][0],s_temperature_noise[i][0]);
	}
	fclose(fp);

	return 1;
}

int fn_NetSim_Temperature_Sensor_Update_Sink_Temp(int sensor_id, double temp)
{
	*s_temperature_noise[sensor_id-1]=temp;
}


int fn_NetSim_Add_DummyPayload(NetSim_PACKET* packet, char* str)
{
	while(packet)
	{
		if(!packet->szPayload && packet->pstruAppData && packet->pstruAppData->dPacketSize)
		{
			unsigned int size=(unsigned int)packet->pstruAppData->dPacketSize;
			packet->szPayload = (PPACKET_INFO)calloc(1,sizeof* packet->szPayload);
			copy_payload(packet->szPayload->packet,packet,&size,str);
			packet->szPayload->packet_len = size;
			packet->pstruAppData->dPacketSize = (double)size;
		}
		packet=packet->pstruNextPacket;
	}
	return 0;
}

void copy_payload(UINT8 real[],NetSim_PACKET* packet,unsigned int* payload, char *str)
{
	u_short i;
	uint32_t key=16;
	if(payload)
	{

		for(i=0;i<*payload;i++)
		{
			
					real[i] = str[i];
			
		}
	
	}
}

void fn_NetSim_Temperature_Sensor_finish()
{
	int i=0;

	for(i=0;i<sensor_count;i++)
	{
		if(DEV_POS[i])
			free(DEV_POS[i]);
		if(s_temperature[i])
			free(s_temperature[i]);
		if(s_temperature_noise[i])
			free(s_temperature_noise[i]);	
		
	}
	
	if(DEV_POS)
		free(DEV_POS);
	if(s_temperature)
		free(s_temperature);
	if(s_temperature_noise)
		free(s_temperature_noise);

	if(pos)
		free(pos);
	if(temp)
		free(temp);
	if(temp_noise)
		free(temp_noise);



}
