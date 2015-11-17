# more libraries with special handling in case we build them ourselves
if(LIBGIT2_FROM_PKGCONFIG)
	pkg_config_library(LIBGIT2 libgit2 REQUIRED)
	set(LIBGIT2_LIBRARIES "")
	if(USE_LIBGIT23_API)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_LIBGIT23_API")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_LIBGIT23_API")
		if(ANDROID)
			# for Android we need to force a static link against ssl and crypto
			# this is a bit hacky, but it seems to work
			set(LIBGIT2_LIBRARIES ${LIBGIT2_LIBRARIES} ${LIBGIT2_LIBRARY_DIRS}/libssl.a ${LIBGIT2_LIBRARY_DIRS}/libcrypto.a)
		endif()
		if(FORCE_LIBSSH)
			pkg_config_library(LIBSSH2 libssh2 REQUIRED)
			set(LIBGIT2_LIBRARIES ${LIBGIT2_LIBRARIES} ${LIBSSH2_LIBRARIES})
		endif()
	endif()
else()
	find_package(LIBGIT2 REQUIRED)
	include_directories(${LIBGIT2_INCLUDE_DIR})
	if(USE_LIBGIT23_API)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_LIBGIT23_API")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_LIBGIT23_API")
		if(FORCE_LIBSSH)
			find_package(Libssh2 QUIET CONFIG)
			if ("${LIBSSH2_VERSION}" STRGREATER "1.6.1")
				set(LIBSSH2_LIBRARIES Libssh2::libssh2)
			endif()
			if(!LIBSSH2_FOUND OR "${LIBSSH2_FOUND}" STREQUAL "")
				pkg_config_library(LIBSSH2 libssh2 REQUIRED)
			endif()
		endif()
		find_package(libcurl QUIET)
		if(!LIBCURL_FOUND OR "${LIBCURL_FOUND}" STREQUAL "")
			pkg_config_library(LIBCURL libcurl REQUIRED)
		endif()
		set(LIBGIT2_LIBRARIES ${LIBGIT2_LIBRARIES} -L${LIBSSH2_LIBRARY_DIRS} ${LIBSSH2_LIBRARIES} ${LIBCURL_LIBRARIES})
	endif()
endif()
