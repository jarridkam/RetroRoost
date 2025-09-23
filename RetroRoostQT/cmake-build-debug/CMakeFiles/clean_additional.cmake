# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/RetroRoostQT_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/RetroRoostQT_autogen.dir/ParseCache.txt"
  "RetroRoostQT_autogen"
  )
endif()
