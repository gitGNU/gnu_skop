ENABLE_TESTING()

SET(test_SRCS
  ${SkOP_SOURCE_DIR}/test/fitstest.h
  ${SkOP_SOURCE_DIR}/test/fitstest.cpp
  )

SET(healpix_SRCS
  ${SkOP_SOURCE_DIR}/src/healpixfield.cpp
  ${SkOP_SOURCE_DIR}/src/sphericalfield.cpp
  ${SkOP_SOURCE_DIR}/test/healpixtest.h
  ${SkOP_SOURCE_DIR}/test/healpixtest.cpp
  )

SET(QT_USE_QTTEST true)

include(FindQt4)

include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

QT4_AUTOMOC(${test_SRCS} ${healpix_SRCS})

ADD_EXECUTABLE(fitstest ${test_SRCS})

ADD_EXECUTABLE(healpixtest ${healpix_SRCS})

TARGET_LINK_LIBRARIES(fitstest fitsio++ ${QT_LIBRARIES})

TARGET_LINK_LIBRARIES(healpixtest cfitsio ${QT_LIBRARIES} GLEW)
