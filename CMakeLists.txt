cmake_minimum_required(VERSION 3.16)

project(MySpice VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_QT "Build Qt frontend" OFF)
option(BUILD_SDL "Build SDL frontend" OFF)

if(EMSCRIPTEN)
    add_executable(MySpiceEmscripten
        component.h component.cpp
        grapheditor.h grapheditor.cpp
        resistor.h resistor.cpp
        gmressolver.h gmressolver.cpp
        capacitor.h capacitor.cpp
        inductor.h inductor.cpp
        voltagesource.h voltagesource.cpp
        diode.h diode.cpp

        dcvoltagesource.h dcvoltagesource.cpp
        acvoltagesource.h acvoltagesource.cpp
        circuitsolver.h circuitsolver.cpp
        components.h
        graphicsadapter.h graphicsadapter.cpp
        rendercomponent.h rendercomponent.cpp
        dummygraphics.h dummygraphics.cpp
        scrollablepane.h scrollablepane.cpp
        infocard.h infocard.cpp
        plotcard.h plotcard.cpp
        mouseeventobserver.h mouseeventobserver.cpp
        settingscard.h settingscard.cpp
        unitslider.h unitslider.cpp

        sdl2/main.cpp
        sdl2/graphicssdl.h sdl2/graphicssdl.cpp)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(MySpiceEmscripten PROPERTIES
        SUFFIX ".html"
    )

    target_compile_options(MySpiceEmscripten PRIVATE
        -sUSE_SDL=2
        -sUSE_SDL_TTF=2
        -sUSE_SDL_GFX=2
    )

    target_link_options(MySpiceEmscripten PRIVATE
        -sUSE_SDL=2
        -sUSE_SDL_TTF=2
        -sUSE_SDL_GFX=2
        -sALLOW_MEMORY_GROWTH=1
    )

    target_link_options(MySpiceEmscripten PRIVATE
       --shell-file=${CMAKE_SOURCE_DIR}/web/shell.html
    )

    target_link_options(MySpiceEmscripten PRIVATE
        --preload-file ${CMAKE_SOURCE_DIR}/assets/@
    )
endif()

if(BUILD_SDL)
    find_package(SDL2 REQUIRED)
    find_package(SDL_gfx REQUIRED)
    find_package(SDL_ttf REQUIRED)
    add_executable(MySpiceSDL2
        component.h component.cpp
        grapheditor.h grapheditor.cpp
        resistor.h resistor.cpp
        gmressolver.h gmressolver.cpp
        capacitor.h capacitor.cpp
        inductor.h inductor.cpp
        voltagesource.h voltagesource.cpp
        diode.h diode.cpp

        dcvoltagesource.h dcvoltagesource.cpp
        acvoltagesource.h acvoltagesource.cpp
        circuitsolver.h circuitsolver.cpp
        components.h
        graphicsadapter.h graphicsadapter.cpp
        rendercomponent.h rendercomponent.cpp
        dummygraphics.h dummygraphics.cpp
        scrollablepane.h scrollablepane.cpp
        infocard.h infocard.cpp
        plotcard.h plotcard.cpp
        mouseeventobserver.h mouseeventobserver.cpp
        settingscard.h settingscard.cpp
        unitslider.h unitslider.cpp

        sdl2/main.cpp
        sdl2/graphicssdl.h sdl2/graphicssdl.cpp)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(MySpiceSDL2 PRIVATE SDL2)
    target_link_libraries(MySpiceSDL2 PRIVATE SDL2_gfx)
    target_link_libraries(MySpiceSDL2 PRIVATE SDL2_ttf)
endif()

if(BUILD_QT)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(MySpice
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            component.h component.cpp
            grapheditor.h grapheditor.cpp
            resistor.h resistor.cpp
            gmressolver.h gmressolver.cpp
            capacitor.h capacitor.cpp
            inductor.h inductor.cpp
            voltagesource.h voltagesource.cpp
            diode.h diode.cpp

            dcvoltagesource.h dcvoltagesource.cpp
            acvoltagesource.h acvoltagesource.cpp
            circuitsolver.h circuitsolver.cpp
            components.h
            graphicsadapter.h graphicsadapter.cpp
            rendercomponent.h rendercomponent.cpp
            dummygraphics.h dummygraphics.cpp
            scrollablepane.h scrollablepane.cpp
            infocard.h infocard.cpp
            plotcard.h plotcard.cpp
            mouseeventobserver.h mouseeventobserver.cpp
            settingscard.h settingscard.cpp
            unitslider.h unitslider.cpp

            qt/main.cpp
            qt/mainwindow.cpp
            qt/mainwindow.h
            qt/graphicsqt.h qt/graphicsqt.cpp
            qt/grapheditorwidget.h qt/grapheditorwidget.cpp
            qt/mainwindow.ui
        )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET MySpice APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    else()
        if(ANDROID)
            add_library(MySpice SHARED
                ${PROJECT_SOURCES}
            )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
        else()
            add_executable(MySpice
                ${PROJECT_SOURCES}
            )
        endif()
    endif()

    target_link_libraries(MySpice PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
    target_link_libraries(MySpice PRIVATE Qt${QT_VERSION_MAJOR}::Test)

    # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
    # If you are developing for iOS or macOS you should consider setting an
    # explicit, fixed bundle identifier manually though.
    if(${QT_VERSION} VERSION_LESS 6.1.0)
      set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MySpice)
    endif()
    set_target_properties(MySpice PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )

    include(GNUInstallDirs)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    install(TARGETS MySpice
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(MySpice)
    endif()
    add_subdirectory(MySpiceTests)
endif()
