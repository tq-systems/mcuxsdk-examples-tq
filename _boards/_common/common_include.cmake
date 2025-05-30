# This file contains common include directives for the ${board} board.
# It also initiates the generation of the version.h file.
# For more information: https://mcuxpresso.nxp.com/mcuxsdk/latest/html/develop/sdk/custom_board_development.html

# The following include directives are common for all boards.
# The conditions must be defined in the boards prj.conf file.

# Include in common folder

if(CONFIG_MCUX_PRJSEG_module.custom_board.pinmux_common_folder)
    mcux_add_source(
        BASE_PATH ${CUSTOM_BOARD_ROOT}/${board}/common/pin_mux/${core_id}
        SOURCES pin_mux.c
                pin_mux.h
    )
    mcux_add_include(BASE_PATH ${CUSTOM_BOARD_ROOT}/${board}/common INCLUDES pin_mux/${core_id})
endif()

if(CONFIG_MCUX_PRJSEG_module.custom_board.clock_common_folder)
    mcux_add_source(
        BASE_PATH ${CUSTOM_BOARD_ROOT}/${board}/common/clock/${core_id}
        SOURCES clock_config.c
                clock_config.h
    )

    mcux_add_include(BASE_PATH ${CUSTOM_BOARD_ROOT}/${board}/common INCLUDES clock/${core_id})
endif()

if(CONFIG_MCUX_PRJSEG_project.custom_board.hw_app_common_folder)
    mcux_add_source(
        BASE_PATH ${CUSTOM_BOARD_ROOT}/${board}/common
        SOURCES hardware_init.c
                app.h
    )
    mcux_add_include(BASE_PATH ${CUSTOM_BOARD_ROOT}/${board}/common INCLUDES .)
endif()

if(CONFIG_MCUX_PRJSEG_module.custom_board.boardfile_common_folder)
    mcux_add_source(
        BASE_PATH ${CUSTOM_BOARD_ROOT}/${board}/common
        SOURCES board.c
                board.h
    )
    mcux_add_include(BASE_PATH ${CUSTOM_BOARD_ROOT}/${board}/common INCLUDES .)
endif()

if(CONFIG_MCUX_PRJSEG_module.custom_board.linker_common_folder)
    include(${CUSTOM_BOARD_ROOT}/${board}/target/linker.cmake)
endif()

# Include in project folder

if(CONFIG_MCUX_PRJSEG_project.custom_board.hw_app_project_folder)
    mcux_add_source(
        BASE_PATH ${CUSTOM_BOARD_ROOT}
        SOURCES ${project_board_port_path}/hardware_init.c
    )
    mcux_add_include(BASE_PATH ${CUSTOM_BOARD_ROOT} INCLUDES ${project_board_port_path})
endif()

if(CONFIG_MCUX_PRJSEG_module.custom_board.pinmux_project_folder)
    mcux_add_source(
        BASE_PATH ${CUSTOM_BOARD_ROOT}/${project_board_port_path}
        SOURCES pin_mux.c
                pin_mux.h
    )
    mcux_add_include(BASE_PATH ${CUSTOM_BOARD_ROOT} INCLUDES ${project_board_port_path})
endif()

if(CONFIG_MCUX_PRJSEG_module.custom_board.clock_project_folder)
    mcux_add_source(
        BASE_PATH ${CUSTOM_BOARD_ROOT}/${project_board_port_path}
        SOURCES clock_config.c
                clock_config.h
    )
    mcux_add_include(BASE_PATH ${CUSTOM_BOARD_ROOT} INCLUDES ${project_board_port_path})
endif()

if(CONFIG_MCUX_PRJSEG_module.custom_board.boardfile_project_folder)
    mcux_add_source(
        BASE_PATH ${CUSTOM_BOARD_ROOT}/${project_board_port_path}
        SOURCES board.c
                board.h
    )
    mcux_add_include(BASE_PATH ${CUSTOM_BOARD_ROOT} INCLUDES ${project_board_port_path})
endif()

if(CONFIG_MCUX_PRJSEG_module.custom_board.linker_project_folder)
    include(${CUSTOM_BOARD_ROOT}/${project_board_port_path}/linker.cmake)
endif()

# Generate version.h file

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    execute_process(
        COMMAND cmd /c "${CUSTOM_BOARD_ROOT}/../scripts/generate_version.bat"
        WORKING_DIRECTORY ${CUSTOM_BOARD_ROOT}/..
        COMMAND_ERROR_IS_FATAL LAST
    )
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux" OR ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    execute_process(
        COMMAND bash -c "${CUSTOM_BOARD_ROOT}/../scripts/generate_version.sh"
        WORKING_DIRECTORY ${CUSTOM_BOARD_ROOT}/..
        COMMAND_ERROR_IS_FATAL LAST
    )
else()
    message(FATAL_ERROR "Host system was not recognized. HOST NAME: ${CMAKE_HOST_SYSTEM_NAME}")
endif()

if(EXISTS "${CUSTOM_BOARD_ROOT}/../version.h")
    mcux_add_armgcc_configuration(CC "-DVERSION_INFORMATION=1" CX "-DVERSION_INFORMATION=1")
    mcux_add_source(
        BASE_PATH ${CUSTOM_BOARD_ROOT}/..
        SOURCES version.h
    )
    mcux_add_include(BASE_PATH ${CUSTOM_BOARD_ROOT}/.. INCLUDES .)
    message("version.h included.")
endif()

log_notice("Common include files for ${board} board are included")
