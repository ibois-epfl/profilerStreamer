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

function(download_dependency dependecy_name url)
    set(output_path "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/${dependecy_name}")
    if(NOT EXISTS ${output_path})
        message(STATUS "Downloading ${dependecy_name} from ${url}...")
        file(DOWNLOAD ${url} ${output_path}.zip SHOW_PROGRESS)
        message(STATUS "Extracting ${dependecy_name}...")
        file(MAKE_DIRECTORY ${output_path})
        file(ARCHIVE_EXTRACT INPUT ${output_path}.zip DESTINATION ${output_path})
        if(NOT EXISTS ${output_path})
            message(FATAL_ERROR "Failed to extract ${dependecy_name}, directory not found after extraction.")
        endif()
        file(REMOVE ${output_path}.zip)
    else()
        message(STATUS "${dependecy_name} already exists at ${output_path}, skipping download.")
    endif()
endfunction(download_dependency)