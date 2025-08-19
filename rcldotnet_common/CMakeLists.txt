cmake_minimum_required(VERSION 3.8)

project(rcldotnet_common C CSharp)

find_package(ament_cmake REQUIRED)
# find_package(dotnet_cmake_module REQUIRED)
# find_package(ament_cmake_export_assemblies REQUIRED)
# find_package(DotNETExtra REQUIRED)

if(NOT CSHARP_TARGET_FRAMEWORK)
 set(CSHARP_TARGET_FRAMEWORK "netstandard2.0")
endif()

set(CS_SOURCES
  DllLoadUtils.cs
  IRosActionDefinition.cs
  IRosActionFeedbackMessage.cs
  IRosActionGetResultRequest.cs
  IRosActionGetResultResponse.cs
  IRosActionSendGoalRequest.cs
  IRosActionSendGoalResponse.cs
  IRosMessage.cs
  IRosServiceDefinition.cs
)

# Use CMake native CSharp support instead of dotnet_cmake_module
add_library(${PROJECT_NAME} SHARED ${CS_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    DOTNET_TARGET_FRAMEWORK ${CSHARP_TARGET_FRAMEWORK}
    # LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/${PROJECT_NAME}/dotnet"
)
# add_dotnet_library(${PROJECT_NAME} ${CS_SOURCES})
install(FILES $<TARGET_FILE:${PROJECT_NAME}> 
        DESTINATION lib/${PROJECT_NAME}/dotnet)
# install_dotnet(${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME}/dotnet)
# ament_export_assemblies_dll("lib/${PROJECT_NAME}/dotnet/${PROJECT_NAME}.dll")

ament_package()
