cmake_minimum_required(VERSION 3.8)

project(rcldotnet C CSharp)

# find_package(ament_cmake_export_assemblies REQUIRED)

find_package(ament_cmake REQUIRED)
find_package(rcl REQUIRED)
find_package(rcl_action REQUIRED)
find_package(action_msgs REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(unique_identifier_msgs REQUIRED)
find_package(rmw REQUIRED)
find_package(rmw_implementation REQUIRED)
find_package(rmw_implementation_cmake REQUIRED)
find_package(rosidl_generator_c REQUIRED)

# find_package(dotnet_cmake_module REQUIRED)
# find_package(DotNETExtra REQUIRED)

# Build rcldotnet_common as a subdirectory
add_subdirectory(../rcldotnet_common rcldotnet_common)

if(NOT WIN32)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,error")
  endif()
endif()

if(NOT CSHARP_TARGET_FRAMEWORK)
 set(CSHARP_TARGET_FRAMEWORK "netstandard2.0")
endif()

set(CS_SOURCES
  ActionClient.cs
  ActionClientGoalHandle.cs
  ActionDefinitionStaticMemberCache.cs
  ActionGoalStatus.cs
  ActionServer.cs
  ActionServerGoalHandle.cs
  Client.cs
  Clock.cs
  GuardCondition.cs
  GuidExtensions.cs
  MessageStaticMemberCache.cs
  Node.cs
  Publisher.cs
  QosProfile.cs
  QoSProfileDelegates.cs
  RCLdotnet.cs
  RCLExceptionHelper.cs
  RCLRet.cs
  SafeActionClientHandle.cs
  SafeActionGoalHandle.cs
  SafeActionServerHandle.cs
  SafeClientHandle.cs
  SafeClockHandle.cs
  SafeGuardConditionHandle.cs
  SafeNodeHandle.cs
  SafePublisherHandle.cs
  SafeQosProfileHandle.cs
  SafeRequestIdHandle.cs
  SafeServiceHandle.cs
  SafeSubscriptionHandle.cs
  SafeTimerHandle.cs
  SafeWaitSetHandle.cs
  Service.cs
  ServiceDefinitionStaticMemberCache.cs
  Subscription.cs
  Timer.cs
)

# Use CMake native CSharp support instead of dotnet_cmake_module
add_library(${PROJECT_NAME}_assemblies SHARED ${CS_SOURCES})
set_target_properties(${PROJECT_NAME}_assemblies PROPERTIES
    DOTNET_TARGET_FRAMEWORK ${CSHARP_TARGET_FRAMEWORK}
    # LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/${PROJECT_NAME}/dotnet"
)

# Add dependency on rcldotnet_common target
add_dependencies(${PROJECT_NAME}_assemblies rcldotnet_common)

# set(_assemblies_dep_dlls
#   ${action_msgs_ASSEMBLIES_DLL}
#   ${builtin_interfaces_ASSEMBLIES_DLL}
#   ${rcldotnet_common_ASSEMBLIES_DLL}
#   ${unique_identifier_msgs_ASSEMBLIES_DLL}
# )

# target_link_libraries(${PROJECT_NAME}_assemblies ${_assemblies_dep_dlls})

# add_dotnet_library(${PROJECT_NAME}_assemblies
#   SOURCES
#   ${CS_SOURCES}
#   INCLUDE_DLLS
#   ${_assemblies_dep_dlls}
# )

install(FILES $<TARGET_FILE:${PROJECT_NAME}_assemblies> 
        DESTINATION lib/${PROJECT_NAME}/dotnet)
# install_dotnet(${PROJECT_NAME}_assemblies DESTINATION lib/${PROJECT_NAME}/dotnet)
# ament_export_assemblies_dll("lib/${PROJECT_NAME}/dotnet/${PROJECT_NAME}_assemblies.dll")

add_library(${PROJECT_NAME}_native SHARED
  rcldotnet_action_client.c
  rcldotnet_client.c
  rcldotnet_clock.c
  rcldotnet_guard_condition.c
  rcldotnet_node.c
  rcldotnet_publisher.c
  rcldotnet_timer.c
  rcldotnet_qos_profile.c
  rcldotnet.c
)

ament_target_dependencies(${PROJECT_NAME}_native
  "action_msgs"
  "builtin_interfaces"
  "unique_identifier_msgs"
  "rcl"
  "rcl_action"
  "rosidl_generator_c"
  "rosidl_typesupport_c"
)

ament_export_libraries(${PROJECT_NAME}_native)

install(TARGETS ${PROJECT_NAME}_native
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

ament_export_dependencies(ament_cmake)
ament_export_dependencies(builtin_interfaces)
ament_export_dependencies(rcl)
ament_export_dependencies(rosidl_generator_c)
ament_export_dependencies(rosidl_typesupport_c)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()

  find_package(std_msgs REQUIRED)
  find_package(test_msgs REQUIRED)

  ament_lint_auto_find_test_dependencies()

  set(_test_assemblies_dep_dlls
      ${std_msgs_ASSEMBLIES_DLL}
      ${test_msgs_ASSEMBLIES_DLL}
  )

  # Used by CI to test the different target frameworks in a matrix.
  if(DEFINED ENV{RCLDOTNET_TEST_TARGET_FRAMEWORK})
    set(RCLDOTNET_TEST_TARGET_FRAMEWORK $ENV{RCLDOTNET_TEST_TARGET_FRAMEWORK})
  else()
    set(RCLDOTNET_TEST_TARGET_FRAMEWORK "net6.0")
  endif()

  # Use CMake native CSharp support for tests
  add_executable(test_messages
    ${CS_SOURCES}
    test/test_actions.cs
    test/test_guard_conditions.cs
    test/test_messages.cs
    test/test_services.cs
  )
  
  set_target_properties(test_messages PROPERTIES
      DOTNET_TARGET_FRAMEWORK ${RCLDOTNET_TEST_TARGET_FRAMEWORK}
      EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/${PROJECT_NAME}/dotnet"
  )
  
  # TODO: Add package references for xunit and other test dependencies
  # This would require custom NuGet package reference handling in CMake
  
  # add_dotnet_test(test_messages
  #   TARGET_FRAMEWORK
  #   ${RCLDOTNET_TEST_TARGET_FRAMEWORK}
  #   SOURCES
  #   ${CS_SOURCES}
  #   test/test_actions.cs
  #   test/test_guard_conditions.cs
  #   test/test_messages.cs
  #   test/test_services.cs
  #   INCLUDE_DLLS
  #   ${_assemblies_dep_dlls}
  #   ${_test_assemblies_dep_dlls}
  #   INCLUDE_REFERENCES
  #   "Microsoft.NET.Test.Sdk=15.9.0"
  #   "xunit=2.4.1"
  #   "xunit.runner.visualstudio=2.4.1"
  # )
  add_dependencies(test_messages ${PROJECT_NAME}_assemblies)

  install(FILES $<TARGET_FILE:test_messages> 
          DESTINATION lib/${PROJECT_NAME}/dotnet)
  # install_dotnet(test_messages DESTINATION lib/${PROJECT_NAME}/dotnet )

endif()

ament_package()
