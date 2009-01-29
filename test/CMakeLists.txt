ENABLE_TESTING()

SET(healpix_SRCS
  ${SkOP_SOURCE_DIR}/src/shader.h
  ${SkOP_SOURCE_DIR}/src/shader.cpp
  ${SkOP_SOURCE_DIR}/src/healpixfield.cpp
  ${SkOP_SOURCE_DIR}/src/sphericalfield.cpp
  ${SkOP_SOURCE_DIR}/test/healpixtest.h
  ${SkOP_SOURCE_DIR}/test/healpixtest.cpp
  )

SET(QT_USE_QTTEST true)

include(FindQt4)

include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

QT4_AUTOMOC(${healpix_SRCS})

ADD_EXECUTABLE(healpixtest ${healpix_SRCS})

TARGET_LINK_LIBRARIES(healpixtest cfitsio ${QT_LIBRARIES} GLEW)
