cmake_minimum_required(VERSION 3.9)
project(ITOI)

set(CMAKE_CXX_STANDARD 17)




add_executable(ITOI main.cpp src/Application.cpp src/Application.h src/ImageReaderWriter/ImageReader.cpp src/ImageReaderWriter/ImageReader.h src/ImageReaderWriter/ImageWriter.cpp src/ImageReaderWriter/ImageWriter.h src/Image/Image.cpp src/Image/Image.h src/Kernel/Kernel.cpp src/Kernel/Kernel.h src/Pyramid/Pyramid.cpp src/Pyramid/Pyramid.h src/InterestPoints/InterestPoints.cpp src/InterestPoints/InterestPoints.h src/DescriptorFinder.cpp src/DescriptorFinder.h)



find_package(JPEG REQUIRED)
include_directories(  ${JPEG_INCLUDE_DIRS} )

target_link_libraries( ITOI ${JPEG_LIBRARY} )