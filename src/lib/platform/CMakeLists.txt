# SPDX-FileCopyrightText: (C) 2024 Chris Rizzitello <sithlord48@gmail.com>
# SPDX-FileCopyrightText: (C) 2012 - 2024 Symless Ltd
# SPDX-FileCopyrightText: (C) 2009 - 2012 Nick Bolton
# SPDX-License-Identifier: MIT

if(UNIX AND NOT APPLE)
  pkg_check_modules(LIBEI REQUIRED QUIET "libei-1.0 >= ${REQUIRED_LIBEI_VERSION}")
  message(STATUS "libei version: ${LIBEI_VERSION}")

  pkg_check_modules(LIBPORTAL REQUIRED QUIET "libportal >= ${REQUIRED_LIBPORTAL_VERSION}")
  message(STATUS "libportal version: ${LIBPORTAL_VERSION}")

  # Check if we have the restore ability in the IC portal
  include(CMakePushCheckState)
  include(CheckSymbolExists)
  cmake_push_check_state(RESET)
  set(CMAKE_REQUIRED_INCLUDES "${CMAKE_REQUIRED_INCLUDES};${LIBPORTAL_INCLUDE_DIRS};${GLIB2_INCLUDE_DIRS}")
  set(CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES};${LIBPORTAL_LINK_LIBRARIES};${GLIB2_LINK_LIBRARIES}")
  check_symbol_exists(xdp_input_capture_session_get_restore_token "libportal/inputcapture.h" HAVE_IC_RESTORE)
  cmake_pop_check_state()
endif()

if(WIN32)
  set(PLATFORM_SOURCES
    MSWindowsDebugOutputter.cpp
    MSWindowsDebugOutputter.h
    MSWindowsDesks.cpp
    MSWindowsDesks.h
    MSWindowsEventQueueBuffer.cpp
    MSWindowsEventQueueBuffer.h
    MSWindowsHook.cpp
    MSWindowsHook.h
    MSWindowsKeyState.cpp
    MSWindowsKeyState.h
    MSWindowsPowerManager.cpp
    MSWindowsPowerManager.h
    MSWindowsProcess.cpp
    MSWindowsProcess.h
    MSWindowsScreen.cpp
    MSWindowsScreen.h
    MSWindowsScreenSaver.cpp
    MSWindowsScreenSaver.h
    MSWindowsSession.cpp
    MSWindowsSession.h
    MSWindowsWatchdog.cpp
    MSWindowsWatchdog.h
  )
elseif(APPLE)
  set(PLATFORM_SOURCES
    IOSXKeyResource.cpp
    IOSXKeyResource.h
    OSXAutoTypes.h
    OSXCocoaApp.h
    OSXCocoaApp.m
    OSXEventQueueBuffer.cpp
    OSXEventQueueBuffer.h
    OSXKeyState.cpp
    OSXKeyState.h
    OSXMediaKeySupport.h
    OSXMediaKeySupport.m
    OSXPowerManager.cpp
    OSXPowerManager.h
    OSXScreen.h
    OSXScreen.mm
    OSXScreenSaver.cpp
    OSXScreenSaver.h
    OSXScreenSaverControl.h
    OSXScreenSaverUtil.h
    OSXScreenSaverUtil.m
    OSXUchrKeyResource.cpp
    OSXUchrKeyResource.h
  )
elseif(UNIX)
  set(PLATFORM_SOURCES
    XDGPortalRegistry.h
    XDGPowerManager.cpp
    XDGPowerManager.h
    XDGKeyUtil.h
    XDGKeyUtil.cpp
  )
  if (BUILD_X11_SUPPORT)
    configure_file(XWindowsConfig.h.in XWindowsConfig.h @ONLY)
    list(APPEND PLATFORM_SOURCES
      XWindowsEventQueueBuffer.cpp
      XWindowsEventQueueBuffer.h
      XWindowsKeyState.cpp
      XWindowsKeyState.h
      XWindowsScreen.cpp
      XWindowsScreen.h
      XWindowsScreenSaver.cpp
      XWindowsScreenSaver.h
      XWindowsUtil.cpp
      XWindowsUtil.h
      ${CMAKE_CURRENT_BINARY_DIR}/XWindowsConfig.h
    )
  endif()

  if(LIBEI_FOUND)
    list(APPEND PLATFORM_SOURCES
      EiEventQueueBuffer.cpp
      EiEventQueueBuffer.h
      EiKeyState.cpp
      EiKeyState.h
      EiScreen.cpp
      EiScreen.h
    )
    # The Portal sources also require EI.
    if(LIBPORTAL_FOUND)
      list(APPEND PLATFORM_SOURCES
        PortalInputCapture.cpp
        PortalInputCapture.h
        PortalRemoteDesktop.cpp
        PortalRemoteDesktop.h
      )
    endif()
  endif()
endif()

# wayland.h is included to check for wayland support
add_library(platform STATIC ${PLATFORM_SOURCES})

target_link_libraries(platform client ${libs})

if(UNIX)
  target_link_libraries(
    platform
    io
    net
    app
    client
    ${libs})

  if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    target_link_libraries(platform ${COCOA_LIBRARY})
  else()
    if(${LIBXKBCOMMON_VERSION} VERSION_GREATER_EQUAL "1.10")
      target_compile_definitions(platform PRIVATE HAVE_XKB_KEYMAP_MOD_GET_MASK=1)
    endif()
    target_compile_definitions(platform PUBLIC WINAPI_LIBEI WINAPI_LIBPORTAL)
    if (HAVE_IC_RESTORE)
      target_compile_definitions(platform PUBLIC HAVE_LIBPORTAL_INPUTCAPTURE_RESTORE)
    endif()
    target_include_directories(platform PUBLIC ${LIBEI_INCLUDE_DIRS} ${LIBPORTAL_INCLUDE_DIRS})
    target_link_libraries(
      platform
      Qt6::DBus
      ${LIBXKBCOMMON_LINK_LIBRARIES}
      ${GLIB2_LINK_LIBRARIES} ${LIBM_LIBRARIES}
      ${LIBEI_LINK_LIBRARIES} ${LIBPORTAL_LINK_LIBRARIES})
  endif()
endif()
