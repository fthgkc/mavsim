# - Try to find  QUADRACOPTER
# Once done, this will define
#
#  QUADRACOPTER_FOUND - system has quadracopter 
#  QUADRACOPTER_INCLUDE_DIRS - the quadracopter include directories

include(LibFindMacros)

# Include dir
find_path(QUADRACOPTER_INCLUDE_DIR
	NAMES in
	PATHS 
		/usr/include/quadracopter
		/usr/local/include/quadracopter
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(QUADRACOPTER_PROCESS_INCLUDES QUADRACOPTER_INCLUDE_DIR)
libfind_process(QUADRACOPTER)
