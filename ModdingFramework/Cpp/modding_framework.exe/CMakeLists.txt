project(ModdingFramework.EXE)

add_executable(${PROJECT_NAME} ModdingFrameworkEXE.cpp ModdingFramework.rc)
target_link_libraries(${PROJECT_NAME} PRIVATE DLL_Injector Logging)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "modding_framework")
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

# Find a way around this disgusting hack
find_path(SIMPLEINI_INCLUDE_DIRS "SimpleIni.h")
target_include_directories(${PROJECT_NAME} INTERFACE include ${SIMPLEINI_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_SOURCE_DIR}/Libraries/Cpp/Serialization/include")

set(
    OUTPUT_FOLDERS
    "${CMAKE_SOURCE_DIR}/Games/Morrowind/Mods/Output/Modding Framework"
    "${CMAKE_SOURCE_DIR}/Games/Fallout1/Mods/Output/Modding Framework"
)

foreach(OUTPUT_FOLDER ${OUTPUT_FOLDERS})
    message(STATUS "Copying ${PROJECT_NAME} to ${OUTPUT_FOLDER}")
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${OUTPUT_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )
endforeach()
