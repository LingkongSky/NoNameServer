# CMakeList.txt: NonameServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()



project ("NonameServer")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/asio2/3rd)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/asio2/include)




set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ASIO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/asio2/include)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)


# 将源代码添加到此项目的可执行文件。

add_executable (NonameServer "NonameServer.cpp"  "NonameServer.h"  "MultiPlayerManager.cpp"  "ServerUtils.cpp" "NonameServerHttp.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NonameServer PROPERTY CXX_STANDARD 17)
endif()

# TODO: 如有需要，请添加测试并安装目标。

# 拷贝文件夹到指定的目录

add_custom_target(copy ALL  
  VERBATIM 
  # COMMAND_EXPAND_LISTS 
  COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/build/utils"
  COMMAND ${CMAKE_COMMAND} -E 
                  copy_directory  
                  "${CMAKE_CURRENT_SOURCE_DIR}/utils" 
                  "${CMAKE_CURRENT_SOURCE_DIR}/build/utils" 
)


target_compile_options(NonameServer 
	PRIVATE 
	$<$<CXX_COMPILER_ID:MSVC>:/bigobj> 
	#$<$<CXX_COMPILER_ID:GNU>:-Wa,-mbig-obj>
	) 


