project(ModdingFramework)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE ModdingFramework.Types StringFormatting Logging Memory Assembly CodeInjection)
