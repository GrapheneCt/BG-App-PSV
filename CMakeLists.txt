cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{DOLCESDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{DOLCESDK}/share/dolce.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define DOLCESDK to point to your SDK path!")
  endif()
endif()

include("$ENV{DOLCESDK}/share/dolce.cmake" REQUIRED)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/boot_param.bin
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/boot_param2.bin
  COMMAND dolce-make-bootparam app_memsize 0x1000 ${CMAKE_CURRENT_BINARY_DIR}/boot_param.bin
  COMMAND dolce-make-bootparam app_memsize 0x1000 attribute 0x03 ${CMAKE_CURRENT_BINARY_DIR}/boot_param2.bin
)

project(appbgservice C)
dolce_gen_libs(SceNotificationUtil_stubs
  SceNotificationUtil.yml
  LIB SceNotificationUtil_stub_weak)
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/appbgservice/main.c)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall")
target_link_libraries(${PROJECT_NAME}
  SceLibKernel_stub
  SceIofilemgr_stub
  SceSysmodule_stub
  SceAppMgr_stub
  #ScePaf_stub
  SceNotificationUtil_stub_weak
)
dolce_create_self(eboot2.bin
  ${PROJECT_NAME}
  UNSAFE
  BOOT_PARAM ${CMAKE_CURRENT_BINARY_DIR}/boot_param2.bin
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/param2.sfo
  COMMAND dolce-mksfoex
    -s APP_VER=01.00
    -d ATTRIBUTE=17305600
    -s CATEGORY=gdd
    -s INSTALL_DIR_ADDCONT=BGTEST010
    -s INSTALL_DIR_SAVEDATA=BGTEST010
    -s TITLE_ID=BGTEST011
    -s STITLE=BGtestservice
    -s TITLE=BGtestservice
    "${PROJECT_NAME}" ${CMAKE_CURRENT_BINARY_DIR}/param2.sfo
)

project(appmain C)
dolce_gen_libs(SceBgAppUtil_stubs
  SceBgAppUtil.yml
  LIB SceBgAppUtil_stub_weak)
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/appmain/main.c)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall")
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
dolce_create_self(eboot.bin
  ${PROJECT_NAME}
  UNSAFE
  BOOT_PARAM ${CMAKE_CURRENT_BINARY_DIR}/boot_param.bin
)

set(DOLCE_MKSFOEX_FLAGS "${DOLCE_MKSFOEX_FLAGS} -d ATTRIBUTE=16809984 -s CATEGORY=gdc -s PUBTOOLINFO=has_bg=1")

dolce_create_vpk(${PROJECT_NAME}.vpk BGTEST010 eboot.bin
  VERSION 01.00
  NAME "BG Application Sample"
  FILE ${CMAKE_CURRENT_BINARY_DIR}/eboot2.bin eboot2.bin
  FILE ${CMAKE_CURRENT_BINARY_DIR}/param2.sfo sce_sys/param2.sfo
  FILE common_data/sce_sys/icon0.png sce_sys/icon0.png
  FILE common_data/dummy.dat dummy.dat
  FILE common_data/msgdat.dat msgdat.dat
)
