#include "flag.h"

void UserTask_Testdelay(void *data)
{
	MicroOS_OSdelay(1,100);
	MicroOS_OSdelay(2,200);
	MicroOS_OSdelay(3,300);
	MicroOS_OSdelay(4,300);

}

void UserTask_DelayClear(void *data)
{
	if(MicroOS_OSdelayDone(1))
	{
	 printf("100ms OSdelay\n");
	MicroOS_OSdelay_Remove(1);
	}
	if(MicroOS_OSdelayDone(2))
	{
	 printf("200ms OSdelay\n");
		MicroOS_OSdelay_Remove(2);
	}
	if(MicroOS_OSdelayDone(3))
	{
	 printf("300ms OSdelay\n");
		MicroOS_OSdelay_Remove(3);
	}
		if(MicroOS_OSdelayDone(4))
	{
	 printf("400ms OSdelay\n");
		MicroOS_OSdelay_Remove(4);
	}
}
