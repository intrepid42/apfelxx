add_subdirectory(kernel)
add_subdirectory(interface)
add_subdirectory(evolution)
add_subdirectory(structurefunctions)
add_subdirectory(tmds)
add_subdirectory(gpds)

option(SHARED "Build shared-libray instead of static-libray" ON)
if(SHARED)
  add_library(apfelxx SHARED $<TARGET_OBJECTS:kernel> $<TARGET_OBJECTS:interface>
  $<TARGET_OBJECTS:evolution> $<TARGET_OBJECTS:structurefunctions>
  $<TARGET_OBJECTS:tmds> $<TARGET_OBJECTS:gpds>)
else(SHARED)
  add_library(apfelxx STATIC $<TARGET_OBJECTS:kernel> $<TARGET_OBJECTS:interface>
  $<TARGET_OBJECTS:evolution> $<TARGET_OBJECTS:structurefunctions>
  $<TARGET_OBJECTS:tmds> $<TARGET_OBJECTS:gpds>)
endif(SHARED)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/inc/apfel DESTINATION include)
install(TARGETS apfelxx DESTINATION lib)
