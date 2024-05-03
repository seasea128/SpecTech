cmake_minimum_required(VERSION 3.15)

project(
  SpecTech
  VERSION 0.1
  LANGUAGES CXX)

if(WIN32)
  include(InstallRequiredSystemLibraries)
  set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
  set(CPACK_GENERATOR "NSIS")
else()
  set(CPACK_GENERATOR "ZIP")
endif()

set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/VR.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/VR.ico")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
set(CPACK_NSIS_MODIFY_PATH on)
set(CPACK_CREATE_DESKTOP_LINKS SpecTech)
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\SpecTech.exe")
set(CPACK_PACKAGE_EXECUTABLES SpecTech;SpecTech)

include(CPack)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(APP_ICON_RESOURCE_WINDOWS Icons/VRICON.rc)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Include all of VTK - previous example specified individual components but this
# is just likely to cause problems
find_package(
  VTK REQUIRED
  COMPONENTS CommonCore
             CommonDataModel
             CommonColor
             IOGeometry
             FiltersSources
             GUISupportQt
             InteractionStyle
             RenderingContextOpenGL2
             RenderingOpenVR
             RenderingCore
             RenderingLOD
             RenderingFreeType
             RenderingGL2PSOpenGL2
             RenderingOpenGL2
             GUISupportQt
             RenderingQt)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    Utils.h
    Utils.cpp
    Icons.qrc
    Filters/FilterType.h
    Filters/FilterData.h
    RenderThread/RenderThread.cpp
    RenderThread/RenderThread.h
    RenderThread/Callback.cpp
    RenderThread/Callback.h
    RenderThread/Commands/BaseCommand.cpp
    RenderThread/Commands/BaseCommand.h
    RenderThread/Commands/EndRenderCommand.cpp
    RenderThread/Commands/EndRenderCommand.h
    RenderThread/Commands/RefreshRenderCommand.cpp
    RenderThread/Commands/RefreshRenderCommand.h
    RenderThread/Commands/UpdateColourCommand.cpp
    RenderThread/Commands/UpdateColourCommand.h
    RenderThread/Commands/UpdateVisibilityCommand.cpp
    RenderThread/Commands/UpdateVisibilityCommand.h
    RenderThread/Commands/RemoveActorCommand.cpp
    RenderThread/Commands/RemoveActorCommand.h
    RenderThread/Commands/AddActorCommand.cpp
    RenderThread/Commands/AddActorCommand.h
    RenderThread/Commands/UpdateFilterListCommand.cpp
    RenderThread/Commands/UpdateFilterListCommand.h
    RenderThread/Commands/UpdatePropertyCommand.cpp
    RenderThread/Commands/UpdatePropertyCommand.h
    RenderThread/Commands/UpdateRotationSpeedCommand.h
    RenderThread/Commands/UpdateRotationSpeedCommand.cpp)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(
    SpecTech
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    Icons.qrc
    optiondialog.h
    optiondialog.cpp
    optiondialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/VRICON.rc
    optiondialogwithlist.h
    optiondialogwithlist.cpp
    optiondialogwithlist.ui
    filteroption.h
    filteroption.cpp
    filteroption.ui
    Filters/shrinkfilteroption.h
    Filters/shrinkfilteroption.cpp
    Filters/shrinkfilteroption.ui
    Filters/clipfilteroption.h
    Filters/clipfilteroption.cpp
    Filters/clipfilteroption.ui)

  # Define target properties for Android with Qt 6 as: set_property(TARGET
  # SpecTech APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  # ${CMAKE_CURRENT_SOURCE_DIR}/android) For more information, see
  # https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(SpecTech SHARED ${PROJECT_SOURCES})
    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(SpecTech ${PROJECT_SOURCES})
  endif()
endif()

target_link_libraries(SpecTech PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SpecTech PRIVATE Qt6::Widgets ${VTK_LIBRARIES})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If
# you are developing for iOS or macOS you should consider setting an explicit,
# fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SpecTech)
endif()
set_target_properties(
  SpecTech
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

include(GNUInstallDirs)

# Copy across Open VR bindings that map controllers The program will expect to
# find these in the build dir when it runs
add_custom_target(VRBindings ALL)
add_custom_command(
  TARGET VRBindings
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR})

add_custom_target(Theme ALL)
add_custom_command(
  TARGET Theme
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Combinear.qss
          ${CMAKE_BINARY_DIR})

add_custom_target(Skybox ALL)
add_custom_command(
  TARGET Skybox
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/skybox
          ${CMAKE_BINARY_DIR}/skybox)

install(
  TARGETS SpecTech
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY vrbindings DESTINATION bin)
install(FILES Combinear.qss DESTINATION bin)
install(DIRECTORY skybox DESTINATION bin)
if(WIN32)
  install(FILES ${OpenVR_INCLUDE_DIR}/../bin/win64/openvr_api.dll
          DESTINATION bin)
endif()
qt_generate_deploy_app_script(TARGET SpecTech OUTPUT_SCRIPT deploy_script)
install(SCRIPT ${deploy_script})
install(FILES $<TARGET_RUNTIME_DLLS:SpecTech> TYPE BIN)

# install(TARGETS SpecTech RUNTIME ARCHIVE LIBRARY RUNTIME FRAMEWORK BUNDLE
# PUBLIC_HEADER RESOURCE) install(TARGETS SpecTech COMPONENT SpecTech
# RUNTIME_DEPENDENCIES PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
# POST_EXCLUDE_REGEXES ".*system32/.*\\.dll" DIRECTORIES ${VTK_DIR}/bin)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(SpecTech)
endif()
