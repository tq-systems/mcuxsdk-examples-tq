mcux_add_source(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}/PMIC
    SOURCES PMIC.c
            PMIC.h
)
mcux_add_include(BASE_PATH ${CMAKE_CURRENT_LIST_DIR} INCLUDES PMIC)
