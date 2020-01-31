cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
project(appbgservice)
add_subdirectory(notificationutil)
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/appbgservice/main.c)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall")
link_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME}
  SceLibKernel_stub
  SceIofilemgr_stub
  SceSysmodule_stub
  SceAppMgr_stub
  #ScePaf_stub
  SceNotificationUtil_stub_weak
)
vita_create_self(eboot2.bin ${PROJECT_NAME} UNSAFE)

add_custom_target(${PROJECT_NAME}.vpk ALL
	COMMAND vita-mksfoex 
	-s APP_VER=01.00
	-d ATTRIBUTE=17305600
	-s CATEGORY=gdd
	-s INSTALL_DIR_ADDCONT=BGTEST010
	-s INSTALL_DIR_SAVEDATA=BGTEST010
	-s TITLE_ID=BGTEST011
	-s STITLE=BGtestservice
	-s TITLE=BGtestservice	
	"${PROJECT_NAME}" param2.sfo
)

project(appmain)
add_subdirectory(bgapputil)
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/appmain/main.c)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall")
link_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME}
  SceLibKernel_stub
  SceDisplay_stub
  SceIofilemgr_stub
  SceCtrl_stub
  SceSysmodule_stub
  SceAppMgr_stub
  #ScePaf_stub 
  SceBgAppUtil_stub_weak
)
vita_create_self(eboot.bin ${PROJECT_NAME} UNSAFE)

set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d ATTRIBUTE=16809984 -s CATEGORY=gdc -s PUBTOOLINFO=has_bg=1")

vita_create_vpk(${PROJECT_NAME}.vpk BGTEST010 eboot.bin
  VERSION 01.00
  NAME "BG Application Sample"
  FILE ${CMAKE_CURRENT_BINARY_DIR}/eboot2.bin eboot2.bin
  FILE ${CMAKE_CURRENT_BINARY_DIR}/param2.sfo sce_sys/param2.sfo
  FILE common_data/sce_sys/icon0.png sce_sys/icon0.png
  FILE common_data/dummy.dat dummy.dat
  FILE common_data/msgdat.dat msgdat.dat
)