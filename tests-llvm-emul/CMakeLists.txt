cmake_minimum_required(VERSION 3.10)

project(tests_llvm_emul)

set(CMAKE_CXX_STANDARD 14)

set(LLVM_LINK_COMPONENTS
        BitWriter
        IRReader
        Object
        InstCombine
        ipo
        AsmParser
        BitReader
        MCParser
        CodeGen
        ProfileData   # needed by ScalarOpts
        Support       # has to be after LLVMProfileData
        ScalarOpts
        TransformUtils
        Analysis
        Target
        Core
        MC
        Object
        )

find_package(LLVM
        REQUIRED
        CONFIG
        PATHS ${LLVM_HOME_PATH})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(tests-llvm-emul
        llvmir_emul_tests.cpp
        )

llvm_map_components_to_libnames(llvm_libs ${LLVM_LINK_COMPONENTS})

message(STATUS "root This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "root This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message(STATUS "${llvm_libs} ${LLVM_LINK_COMPONENTS}")
add_library(libllvmir-emul STATIC IMPORTED)
set_target_properties(libllvmir-emul PROPERTIES IMPORTED_LOCATION /home/lichengpeng/toy/UnitTest/cmake-build-debug/libllvmir-emul.a)
#
#target_include_directories(tests-llvm-emul
#        PRIVATE
#        ${TESTS_DIR}
#        )



target_link_libraries(tests-llvm-emul
        PUBLIC
        libllvmir-emul
        ${llvm_libs}
        )

#set_target_properties(tests-llvm-emul
#        PROPERTIES
#        OUTPUT_NAME "tests-llvm-emul"
#        )

install(TARGETS tests-llvm-emul
        ARCHIVE DESTINATION ${INSTALL_TESTS_DIR}
        LIBRARY DESTINATION ${INSTALL_TESTS_DIR}
        RUNTIME DESTINATION ${INSTALL_TESTS_DIR}
        )
#cmake_minimum_required(VERSION 3.10)
#project(test2)
#
#set(CMAKE_CXX_STANDARD 14)
#
#set(LLVM_LINK_COMPONENTS
#        BitWriter
#        IRReader
#        Object
#        InstCombine
#        ipo
#        AsmParser
#        BitReader
#        MCParser
#        CodeGen
#        ProfileData   # needed by ScalarOpts
#        Support       # has to be after LLVMProfileData
#        ScalarOpts
#        TransformUtils
#        Analysis
#        Target
#        Core
#        MC
#        Object
#        )
#
#if (LLVM_HOME_PATH)
#
#    find_package(LLVM
#            REQUIRED
#            CONFIG
#            PATHS ${LLVM_HOME_PATH})
#
#    #include_directories(${LLVM_INCLUDE_DIRS})
#    #add_definitions(${LLVM_DEFINITIONS})
#
#    #add_executable(LLVMEmu main.cpp)
#
#    #llvm_map_components_to_libnames(llvm_libs ${LLVM_LINK_COMPONENTS})
#    ##llvm_map_components_to_libnames(llvm_libs clang)
#
#    #target_link_libraries(LLVMEmu ${llvm_libs})
#
#
#    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
#
#    # Set your project compile flags.
#    # E.g. if using the C++ header files
#    # you will need to enable C++11 support
#    # for your compiler.
#
#    include_directories(${LLVM_INCLUDE_DIRS})
#    add_definitions(${LLVM_DEFINITIONS})
#
#    # Now build our tools
#    add_executable(tests-llvm-emul llvmir_emul_tests.cpp)
#
#    # Find the libraries that correspond to the LLVM components
#    # that we wish to use
#    #llvm_map_components_to_libnames(llvm_libs support core irreader)
#    llvm_map_components_to_libnames(llvm_libs ${LLVM_LINK_COMPONENTS})
#    #message(STATUS "${LLVM_LINK_COMPONENTS}")
#
#    # Link against LLVM libraries
#    target_link_libraries(tests-llvm-emul ${llvm_libs})
#
#else()
#    message(FATAL_ERROR)
#endif()

