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
FILE(GLOB ICH_SRCS "assets/FSW/dan/*.i" "assets/FSW/dvm/*.i" "assets/FSW/danpl/*.i" "assets/FSW/danbg/*.i" "assets/FSW/dlg/*.i")
SET(ICH_OUT)
FOREACH(ICH_SRC ${ICH_SRCS})
	get_filename_component(ICH_SRC_DIR ${ICH_SRC} DIRECTORY)
	get_filename_component(ICH_SRC_BASE ${ICH_SRC} NAME_WE)
    SET(OUTPUT_FILE_NAME "${ICH_SRC_DIR}/${ICH_SRC_BASE}.ich")

    ADD_CUSTOM_COMMAND(
      OUTPUT "${OUTPUT_FILE_NAME}"
      COMMAND ichigo ${ICH_SRC} ${OUTPUT_FILE_NAME}
      DEPENDS ichigo "${ICH_SRC}")

    SET(ICH_OUT ${ICH_OUT} "${OUTPUT_FILE_NAME}")
ENDFOREACH(ICH_SRC)
ADD_CUSTOM_TARGET(my_rule ALL DEPENDS ${ICH_OUT})
