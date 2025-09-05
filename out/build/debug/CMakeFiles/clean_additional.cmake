# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\IronDeal_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\IronDeal_autogen.dir\\ParseCache.txt"
  "IronDeal_autogen"
  )
endif()
