#ifndef __GPS_H
#define __GPS_H
//#define PCT
#ifdef PCT
#define GPSMODEMDEVICE "/dev/ttyUSB0"
#else
#define GPSMODEMDEVICE "/dev/ttyS0"
#endif
#define GPSBAUDRATE  B9600
#define DATEPOS    (7)
void init_GPS(void);
void* GPS_Reader_Task(void*);
#endif
