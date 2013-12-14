PROJECT(sql)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 6)
set(VERSION_PATCH 60)
# KDevplatform dependency version
set( KDEVPLATFORM_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${sql_SOURCE_DIR}/cmake)

find_package(KDE4 REQUIRED)
find_package(KDevPlatform ${KDEVPLATFORM_VERSION} REQUIRED)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
  @ONLY
)


enable_testing()

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${QT4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(connections)

set(kdevsqllanguagesupport_SRCS
    sqllanguagesupport.cpp
    resulttablewidget.cpp
    connections/connectionsmodel.cpp
    queryworker.cpp
)
kde4_add_ui_files( kdevsqllanguagesupport_SRCS results.ui )

kde4_add_plugin(kdevsqllanguagesupport ${kdevsqllanguagesupport_SRCS})

target_link_libraries(kdevsqllanguagesupport
    ${QT_QTSQL_LIBRARY}
    ${KDE4_KUTILS_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
)

install(TARGETS kdevsqllanguagesupport DESTINATION ${PLUGIN_INSTALL_DIR})

configure_file(kdevsqlsupport.desktop.cmake ${CMAKE_CURRENT_BINARY_DIR}/kdevsqlsupport.desktop)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kdevsqlsupport.desktop DESTINATION ${SERVICES_INSTALL_DIR})

install( FILES kdevsqlui.rc DESTINATION ${DATA_INSTALL_DIR}/kdevsqlsupport )
