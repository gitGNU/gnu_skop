# Copyright (C) 2007, 2008, 2009 Marc Betoule

# This file is part of SkOP.

# SkOP is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
  
# SkOP is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with SkOP.  If not, see <http://www.gnu.org/licenses/>.
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
INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIR}
  ${QT_QTTEST_INCLUDE_DIR}
  )
include(FindQt4)
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

qt4_automoc(${healpix_SRCS})

ADD_EXECUTABLE(healpixtest ${healpix_SRCS})

TARGET_LINK_LIBRARIES(healpixtest cfitsio ${QT_LIBRARIES} ${QT_QTTEST_LIBRARIES} GLEW)
