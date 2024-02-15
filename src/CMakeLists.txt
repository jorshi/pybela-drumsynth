set(EXE_NAME pybela-drumsynth)

file(GLOB SRC_FILES *.cpp)

# exclude Watcher.cpp from the build
get_filename_component(full_path_watcher_cpp ${CMAKE_CURRENT_SOURCE_DIR}/Watcher.cpp ABSOLUTE)
list(REMOVE_ITEM SRC_FILES "${full_path_watcher_cpp}")

add_executable(${EXE_NAME} ${SRC_FILES})

target_include_directories(
    ${EXE_NAME} PRIVATE ${BELA_ROOT} ${BELA_ROOT}/include
)

target_link_libraries(
    ${EXE_NAME}
    PRIVATE
    ${BELA_ROOT}/lib/libbelafull.so
    ${COBALT_LIB}
    ${NEON_LIB}
    ${MATHNEON_LIB}
    dl
    prussdrv
    asound
    atomic
    sndfile
    pthread
)

if (ENABLE_PYTORCH_FRONTEND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
    target_link_libraries(${EXE_NAME} PRIVATE ${TORCH_LIBRARIES})
endif()