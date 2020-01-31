#include <psp2/kernel/threadmgr.h>
#include <psp2/appmgr.h> 
#include <psp2/sysmodule.h>
#include <string.h>
#include <psp2/io/fcntl.h> 
#include <psp2/types.h> 

//Init BG Notification
extern int SceNotificationUtilBgApp_CBE814C1(void);

//Send notification request
extern int SceNotificationUtil_DE6F33F4(const char*);

int main(void) {

	sceSysmoduleLoadModule(SCE_SYSMODULE_NOTIFICATION_UTIL);

	//Init BG Notification
	SceNotificationUtilBgApp_CBE814C1();

	//Read notification msg from file, send notification request
	char buf[0x470] = { 0 };
	//UTF-16LE
	SceUID fd = sceIoOpen("app0:msgdat.dat", SCE_O_RDONLY, 0777);
	sceIoRead(fd, &buf, 0x36);
	SceNotificationUtil_DE6F33F4(buf);
	sceIoPread(fd, &buf, 0x46, 0x36);
	SceNotificationUtil_DE6F33F4(buf);
	sceKernelDelayThread(20000000);
	sceIoPread(fd, &buf, 0x36, 0x00);
	sceIoClose(fd);
	SceNotificationUtil_DE6F33F4(buf);

	return sceAppMgrDestroyAppByAppId(-2);
}
