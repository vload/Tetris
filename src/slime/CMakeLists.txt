file(GLOB SLIME_SOURCES "*.cpp")

add_executable(slime ${SLIME_SOURCES})
target_link_libraries(slime VlEngine)

## Copy assets to the place where the executable is.
# Delete the old assets folder
add_custom_command(TARGET slime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory
    $<TARGET_FILE_DIR:slime>/assets)
# Copy the assets folder
add_custom_command(TARGET slime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:slime>/assets)
