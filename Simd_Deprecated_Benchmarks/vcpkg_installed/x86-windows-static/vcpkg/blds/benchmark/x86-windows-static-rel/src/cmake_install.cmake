# Install script for directory: D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/src/v1.8.2-03d9bc3ebb.clean/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/pkgs/benchmark_x86-windows-static")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/x86-windows-static-rel/src/benchmark.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/x86-windows-static-rel/src/benchmark_main.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES
    "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/src/v1.8.2-03d9bc3ebb.clean/include/benchmark"
    "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/x86-windows-static-rel/include/benchmark"
    FILES_MATCHING REGEX "/[^/]*\\.[^/]*h$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark" TYPE FILE FILES
    "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/x86-windows-static-rel/benchmarkConfig.cmake"
    "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/x86-windows-static-rel/benchmarkConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/x86-windows-static-rel/benchmark.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark/benchmarkTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark/benchmarkTargets.cmake"
         "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/x86-windows-static-rel/src/CMakeFiles/Export/d56d27b88fbb159d81f220d5e4f5f598/benchmarkTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark/benchmarkTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark/benchmarkTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark" TYPE FILE FILES "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/x86-windows-static-rel/src/CMakeFiles/Export/d56d27b88fbb159d81f220d5e4f5f598/benchmarkTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark" TYPE FILE FILES "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/x86-windows-static-rel/src/CMakeFiles/Export/d56d27b88fbb159d81f220d5e4f5f598/benchmarkTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/benchmark" TYPE DIRECTORY FILES "D:/GevKeepCoding/XDevCodeSpace/CodeSpace.cpp/Simd_Benchmarks/vcpkg_installed/x86-windows-static/vcpkg/blds/benchmark/src/v1.8.2-03d9bc3ebb.clean/docs/")
endif()

