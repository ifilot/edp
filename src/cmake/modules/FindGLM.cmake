# Try to find the gmm library
# Once done this will define
#
# GLM_FOUND         - system has gmm and it can be used
# GLM_INCLUDE_DIRS   - directory where the header file can be found
#

message(STATUS "Looking for GLM")

SET (GLM_FOUND FALSE)

IF(EXISTS GLM_INCLUDE_DIRS)  # already found

    IF( GLM_INCLUDE_DIRS )
        SET(GLM_FOUND TRUE)
    ENDIF( GLM_INCLUDE_DIRS )

ELSE(EXISTS GLM_INCLUDE_DIRS)
    FIND_PATH(GLM_INCLUDE_DIRS glm/glm.hpp
        /usr/include /usr/local/include/GLM3 /opt/local/include/GLM3 ../../../Libraries/glm-0.9.8.4-win-x64
    )

    IF( GLM_INCLUDE_DIRS )
        SET(GLM_FOUND TRUE)
    ENDIF( GLM_INCLUDE_DIRS )

    if(GLM_INCLUDE_DIRS)
      message(STATUS "Found GLM include directory: ${GLM_INCLUDE_DIRS}")
    else()
      message(STATUS "Could not find GLM include dir")
    endif()

ENDIF(EXISTS GLM_INCLUDE_DIRS)
