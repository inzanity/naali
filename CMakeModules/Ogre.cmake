# Find, include and link Ogre
# Find is already called in the top-level CMakeLists

macro (FIND_OGRE)
endmacro (FIND_OGRE)

macro (INCLUDE_OGRE)
	if (MSVC)
		if (DEFINED ENV{OGRE_HOME})
			include_directories ($ENV{OGRE_HOME}/include)
			link_directories ($ENV{OGRE_HOME}/lib)
		else ()
			include_directories (${PROJECT_SOURCE_DIR}/external_libs/Ogre/include)
			link_directories (${PROJECT_SOURCE_DIR}/external_libs/Ogre/lib)
		endif ()
		
	endif (MSVC)
endmacro (INCLUDE_OGRE)

macro (LINK_OGRE)
	if (MSVC)
		target_link_libraries (${TARGET_NAME}
			debug OgreMain_d
			optimized OgreMain
		)
	endif (MSVC)
endmacro (LINK_OGRE)
