#ifndef _BSP_MP157_H
#define _BSP_MP157_H
#include <mutex>

using namespace std;

extern int envtemp;
extern std::mutex mtx;

void BspGetTempM4();
void sendTemp();
void getTempSocket();
void BspAlarm(void);

#endif
