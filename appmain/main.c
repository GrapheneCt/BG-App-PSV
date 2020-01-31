#include <psp2/io/fcntl.h>
#include <psp2/ctrl.h>
#include <psp2/appmgr.h>
#include <psp2/sysmodule.h>
#include <psp2/types.h> 
#include <psp2/kernel/sysmem.h>
//#include <psp2/paf.h> 

#include "debugScreen.h"

//#define DEBUGMEM

//#define PAF_PRIVATE_HEAP_SIZE 1*1024*1024
#define printf psvDebugScreenPrintf
#define ONPRESS(flag) ((ctrl.buttons & (flag)) && !(ctrl_old.buttons & (flag)))

//Start BG Application
extern int SceBgAppUtil_7C3525B5(int);

int main() {

	//Keep app0: access. Must be a better way to do this
	SceUID fd = sceIoOpen("app0:dummy.dat", SCE_O_RDONLY, 0777);

	/*uint32_t ptr[0x100] = { 0 };
	ptr[0] = 0;
	ptr[1] = (uint32_t)&ptr[0];
	uint32_t scepaf_argp[] = { PAF_PRIVATE_HEAP_SIZE, 0xEA60, 0x40000, 0, 0 };
	sceSysmoduleLoadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, sizeof(scepaf_argp), scepaf_argp, ptr);*/

	sceSysmoduleLoadModule(SCE_SYSMODULE_BG_APP_UTIL);
	psvDebugScreenInit();
	printf("\nBG Application Sample for PS Vita\n");
	printf("Press X button to start BG application\n\n");

	//Reports wrong values?
#ifdef DEBUGMEM
	SceKernelFreeMemorySizeInfo info;
	memset(&info, 0, sizeof(info));
	info.size = sizeof(info);
	sceKernelGetFreeMemorySize(&info);
	printf("Free memory for main application after psvDebugScreenInit():\n");
	printf("LPDDR2: %d MiB\n", info.size_user / 1024 / 1024);
	printf("CDRAM: %d MiB\n", info.size_cdram / 1024 / 1024);
	printf("PHYCONT: %d MiB\n\n", info.size_phycont / 1024 / 1024);
#endif

	SceCtrlData ctrl,ctrl_old={};
	while(1){
		ctrl_old = ctrl;
		sceCtrlReadBufferPositive(0, &ctrl, 1);
		if(ONPRESS(SCE_CTRL_CROSS)){
			//Start BG Application
			int ret = SceBgAppUtil_7C3525B5(0);
			if (ret == 0)
				printf("BG Application started successfully/is already running\n\n");
			else
				printf("BG Application cannot be started. Error code: 0x%x\n\n", ret);
		}
		if(ONPRESS(SCE_CTRL_SQUARE)){
		}
		if(ONPRESS(SCE_CTRL_LEFT)){	
			//printf("0x%x\n", ret);
		}
		if(ONPRESS(SCE_CTRL_RIGHT)){
		}
	}

	sceIoClose(fd);

	return 0;
}
