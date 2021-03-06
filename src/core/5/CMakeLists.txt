#***************************************************************************
# Project:  NextGIS framework library
# Purpose:  cmake script
# Author:   Dmitry Baryshnikov, bishop.dev@gmail.com
#***************************************************************************
#   Copyright (C) 2015-2018 NextGIS, info@nextgis.ru
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#***************************************************************************

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

find_anyproject(Qt5 REQUIRED COMPONENTS Core Network Svg LinguistTools)
if(WITH_ZLIB_EXTERNAL)
    get_target_property(ZLIB_IMPORTED_LOCATION ${ZLIB_LIBRARIES} IMPORTED_LOCATION_RELEASE)
    get_target_property(RCC_PATH Qt5::lrelease IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR ${RCC_PATH} PATH)
    if(WIN32)
        get_filename_component(PATH_DIR ${ZLIB_IMPORTED_LOCATION} PATH)
        set(ENV{PATH} "$ENV{PATH};${PATH_DIR}")
        if(EXISTS ${ZLIB_IMPORTED_LOCATION})
            file(COPY ${ZLIB_IMPORTED_LOCATION} DESTINATION ${QT_BIN_DIR})
        endif()
    elseif(OSX_FRAMEWORK)
        get_filename_component(PATH_DIR ${ZLIB_IMPORTED_LOCATION} PATH)
        get_filename_component(PATH_DIR ${PATH_DIR} PATH)
        get_filename_component(PATH_DIR ${PATH_DIR} PATH)
        if(EXISTS ${PATH_DIR})
            file(COPY ${PATH_DIR} DESTINATION ${QT_BIN_DIR}/Frameworks)
        endif()
    endif()
endif()

set(PUBLIC_HEADERS ${PUBLIC_HEADERS}
    ${PROJECT_SOURCE_DIR}/application.h
)

set(PROJECT_SOURCES ${PROJECT_SOURCES}
    ${PROJECT_SOURCE_DIR}/application.cpp
)

foreach(TRANSLATION ${TRANSLATIONS})
    if(EXISTS ${TRANSLATION})
        qt5_add_translation(QM_FILES ${TRANSLATION})
    endif()
endforeach()

qt5_wrap_cpp(MOC_SRCS5 ${PUBLIC_HEADERS})
add_ngstdcore_lib(${PROJECT_NAME} 5)

if(NOT SKIP_INSTALL_HEADERS AND NOT SKIP_INSTALL_ALL)
    install(FILES ${PUBLIC_HEADERS} DESTINATION ${INSTALL_INC_DIR}/core)
endif()

if(NOT SKIP_INSTALL_FILES AND NOT SKIP_INSTALL_ALL)
    install(FILES ${QM_FILES} DESTINATION ${INSTALL_TRANSLATIONS_DIR})
endif()
