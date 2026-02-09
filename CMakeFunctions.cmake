function(update_submodules)
    find_package(Git QUIET)
    if (GIT_FOUND)
        message(STATUS "Git found: ${GIT_EXECUTABLE}")
        message(STATUS "Updating git submodules (init and recursive)...")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMODULE_RESULT
            OUTPUT_VARIABLE GIT_SUBMODULE_OUTPUT
            ERROR_VARIABLE GIT_SUBMODULE_ERROR
        )
        message(STATUS "Git submodule update output:\n${GIT_SUBMODULE_OUTPUT}")
        if (GIT_SUBMODULE_ERROR)
            message(WARNING "Git submodule update error:\n${GIT_SUBMODULE_ERROR}")
        endif()
        if (NOT GIT_SUBMODULE_RESULT EQUAL "0")
            message(FATAL_ERROR "Git submodule update failed with exit code ${GIT_SUBMODULE_RESULT}")
        endif()
    else()
        message(WARNING "Git not found, cannot update submodules. Please ensure Git is installed and available in PATH.")
    endif()
    
endfunction(update_submodules)

