cmake_minimum_required (VERSION 3.13)
project (RIDanmaku C CXX)

add_subdirectory(RIEngine)

# Variable definitions
set(RIDanmaku_VERSION_MAJOR 1)
set(RIDanmaku_VERSION_MINOR 0)
set(OUTPUT_NAME thFSW)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (RELEASE)
	add_compile_definitions("RELEASE")
endif()

add_executable(${OUTPUT_NAME} WIN32)
target_link_libraries(${OUTPUT_NAME} riengine)

# Source files
add_subdirectory(src)

# Ichigo
set(ICH_SRC_DIR "${CMAKE_SOURCE_DIR}/scripts")
set(ICH_DST_DIR "${CMAKE_BINARY_DIR}/dat")

set(ICH_SRCS
    "dan/danmaku.i"
    "dan/st01.i"

    "danbg/bg.i"
    "danbg/st02.i"

    "danpl/pl00a.i"
    "danpl/pl00b.i"
    "danpl/player.i"

    "dlg/dialog.i"
    "dlg/en00.i"

    "dvm/ascii.i"
    "dvm/danpl_rin.i"
    "dvm/dan_eff.i"
    "dvm/dan_enemy.i"
    "dvm/dan_ui.i"
    "dvm/dialog.i"
    "dvm/dvm.i"
    "dvm/menu.i"
    "dvm/menu_main.i"
    "dvm/menu_pause.i"
    "dvm/menu_score.i"
    "dvm/menu_settings.i"
    "dvm/st02.i"
)

SET(ICH_OUT)
FOREACH(ICH_SRC ${ICH_SRCS})
    SET(OUTPUT_FILE_NAME "${ICH_DST_DIR}/${ICH_SRC}ch")

    ADD_CUSTOM_COMMAND(
      OUTPUT "${OUTPUT_FILE_NAME}"
      COMMAND ichigo ${ICH_SRC_DIR}/${ICH_SRC} ${OUTPUT_FILE_NAME}
      DEPENDS ichigo "${ICH_SRC_DIR}/${ICH_SRC}")

    SET(ICH_OUT ${ICH_OUT} "${OUTPUT_FILE_NAME}")
ENDFOREACH(ICH_SRC)
ADD_CUSTOM_TARGET(my_rule ALL DEPENDS ${ICH_OUT})