# Try to find the BOOST_THREAD library
# BOOST_THREAD_FOUND	    - system has BOOST_THREAD lib
# BOOST_THREAD_LIBRARIES   - libraries needed to use BOOST_THREAD

# TODO: support MacOSX

# BOOST_THREAD needs Boost
INCLUDE(FindPackageHandleStandardArgs)

if(Boost_FOUND AND Boost_LIBRARY_DIR)
  if (BOOST_THREAD_LIBRARIES)
      # Already in cache, be silent
      set(BOOST_THREAD_FIND_QUIETLY TRUE)
  endif()


  if ( AUTO_LINK_ENABLED )
    file ( GLOB BOOST_THREAD_LIBRARIES "${Boost_LIBRARY_DIR}/libboost_thread*" )
  else()
    find_library(BOOST_THREAD_LIBRARIES NAMES "boost_thread" "boost_thread-mt" PATHS ${Boost_LIBRARY_DIR})
  endif()
  
  hide_variable(BOOST_THREAD_LIBRARIES)
  
  find_package_handle_standard_args(BOOST_THREAD "boost_thread not found." BOOST_THREAD_LIBRARIES )
  
endif()

