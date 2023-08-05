BUILD_TYPE_LINUX = Debug
# BUILD_TYPE_LINUX = Release
GENERATOR_LINUX = "Unix Makefiles"

ENABLE_DOXYGEN_LINUX = ON
ENABLE_TESTING_LINUX = ON
ENABLE_COVERAGE_REPORT_LINUX = ON
ENABLE_BUILD_EXAMPLES_LINUX = ON

do_cmake_linux:
	rm -rf build && \
	mkdir build &&	\
	cd build &&	\
	cmake -G $(GENERATOR_LINUX) \
		  -DCMAKE_BUILD_TYPE=$(BUILD_TYPE_LINUX) \
		  -DENABLE_DOXYGEN=$(ENABLE_DOXYGEN_LINUX) \
		  -DENABLE_TESTING=$(ENABLE_TESTING_LINUX) \
		  -DENABLE_COVERAGE_REPORT=$(ENABLE_COVERAGE_REPORT_LINUX) \
		  -DENABLE_BUILD_EXAMPLES=$(ENABLE_BUILD_EXAMPLES_LINUX) \
		  .. && \
	cmake --build . && \
	cmake --build . --target ccov-all && \
	cmake --build . --target docs && \
	ctest --verbose
#	ctest -T Test -T Coverage

# =====================================================================================================

BUILD_TYPE_WINDOWS = Debug
# BUILD_TYPE_WINDOWS = Release
WINDOWS_CTEST_CONFIGURATION = $(BUILD_TYPE_WINDOWS)
GENERATOR_WINDOWS = "Visual Studio 17 2022"

ENABLE_DOXYGEN_WINDOWS = OFF
ENABLE_TESTING_WINDOWS = ON
ENABLE_COVERAGE_REPORT_WINDOWS = OFF
ENABLE_BUILD_EXAMPLES_WINDOWS = ON

do_cmake_windows:
	echo Y | \
	rmdir /S build && \
	mkdir build &&	\
	cd build &&	\
	cmake -G $(GENERATOR_WINDOWS) \
		  -DENABLE_DOXYGEN=$(ENABLE_DOXYGEN_WINDOWS) \
		  -DENABLE_TESTING=$(ENABLE_TESTING_WINDOWS) \
		  -DENABLE_COVERAGE_REPORT=$(ENABLE_COVERAGE_REPORT_WINDOWS) \
		  -DENABLE_BUILD_EXAMPLES=$(ENABLE_BUILD_EXAMPLES_WINDOWS) \
		  .. && \
	cmake --build . --target ALL_BUILD --config $(BUILD_TYPE_WINDOWS) && \
	ctest --verbose -C $(WINDOWS_CTEST_CONFIGURATION)



# # for windows with nmake file generator
# BUILD_TYPE_WINDOWS = Debug
# # BUILD_TYPE_WINDOWS = Release
# GENERATOR_WINDOWS = "NMake Makefiles"

# ENABLE_DOXYGEN_WINDOWS = OFF
# ENABLE_TESTING_WINDOWS = ON
# ENABLE_COVERAGE_REPORT_WINDOWS = OFF
# ENABLE_BUILD_EXAMPLES_WINDOWS = ON

# do_cmake_windows:
# 	echo Y | \
# 	rmdir /S build && \
# 	mkdir build &&	\
# 	cd build &&	\
# 	cmake -G $(GENERATOR_WINDOWS) \
# 		  -DENABLE_DOXYGEN=$(ENABLE_DOXYGEN_WINDOWS) \
# 		  -DENABLE_TESTING=$(ENABLE_TESTING_WINDOWS) \
# 		  -DENABLE_COVERAGE_REPORT=$(ENABLE_COVERAGE_REPORT_WINDOWS) \
# 		  -DENABLE_BUILD_EXAMPLES=$(ENABLE_BUILD_EXAMPLES_WINDOWS) \
# 		  .. && \
# 	cmake --build . && \
# 	ctest --verbose