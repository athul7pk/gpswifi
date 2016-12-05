#ifndef __KB_H
#define __KB_H
#define KBMODEMDEVICE "/dev/ttyS1"
void init_Keypad(void);
void* KB_Task(void*);
#endif
