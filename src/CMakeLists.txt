
SET(SkOP_SRCS
  ${SkOP_SOURCE_DIR}/src/catalog.h
  ${SkOP_SOURCE_DIR}/src/catalog.cpp
  ${SkOP_SOURCE_DIR}/src/pages.h
  ${SkOP_SOURCE_DIR}/src/pages.cpp
  ${SkOP_SOURCE_DIR}/src/preferencedialog.h
  ${SkOP_SOURCE_DIR}/src/preferencedialog.cpp
  ${SkOP_SOURCE_DIR}/src/shader.h
  ${SkOP_SOURCE_DIR}/src/shader.cpp
  ${SkOP_SOURCE_DIR}/src/healpixfield.cpp
  ${SkOP_SOURCE_DIR}/src/sphericalfield.cpp
  ${SkOP_SOURCE_DIR}/src/pixeldock.h
  ${SkOP_SOURCE_DIR}/src/pixeldock.cpp
  ${SkOP_SOURCE_DIR}/src/toolbox.h
  ${SkOP_SOURCE_DIR}/src/toolbox.cpp
  ${SkOP_SOURCE_DIR}/src/engspinbox.h
  ${SkOP_SOURCE_DIR}/src/engspinbox.cpp
  ${SkOP_SOURCE_DIR}/src/histodock.h
  ${SkOP_SOURCE_DIR}/src/histodock.cpp
  ${SkOP_SOURCE_DIR}/src/maplistmodel.h
  ${SkOP_SOURCE_DIR}/src/maplistmodel.cpp
  ${SkOP_SOURCE_DIR}/src/glview.h
  ${SkOP_SOURCE_DIR}/src/glview.cpp
  ${SkOP_SOURCE_DIR}/src/skop.cpp
  ${SkOP_SOURCE_DIR}/src/skop.h
  ${SkOP_SOURCE_DIR}/src/main.cpp
  )

SET(QT_USE_QTOPENGL true)

include(FindQt4)

include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

QT4_AUTOMOC(${SkOP_SRCS})

ADD_EXECUTABLE(skop ${SkOP_SRCS})

TARGET_LINK_LIBRARIES(skop ${QT_LIBRARIES} GLEW cfitsio)
