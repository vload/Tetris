file(GLOB VOXELS_SOURCES "*.cpp")

add_executable(voxels ${VOXELS_SOURCES})
target_link_libraries(voxels VlEngine)

## Copy assets to the place where the executable is.
# Delete the old assets folder
add_custom_command(TARGET voxels POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory
    $<TARGET_FILE_DIR:voxels>/assets)
# Copy the assets folder
add_custom_command(TARGET voxels POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:voxels>/assets)
