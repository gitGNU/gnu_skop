// Copyright (C) 2007, 2008, 2009 Marc Betoule

// This file is part of SkOP.

// SkOP is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
  
// SkOP is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SkOP.  If not, see <http://www.gnu.org/licenses/>.


/**
 * \file shader.h
 * \brief class definition for opengl initialisation and shader code manipulation.
 */    

#ifndef SHADER_H
#define SHADER_H


#include <GL/glew.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

class SourceCode{
public :
  SourceCode(const char * filename);
  SourceCode(const string code);
  ~SourceCode();
  const char * getContent(){return content.c_str();}
private :
  string content;
};

class GlslContext{
public :
  static int CheckGLError(char *file, int line);
  static bool initGLExtensions();
  static bool checkGLSL(void);
  static bool useGLSL;
  static bool initdone;
  static int internalFormat;
};

class Shader{
public :
  Shader(const char * filename, GLenum _shaderType);
  Shader(const string code, GLenum _shaderType);
  ~Shader();
  string getCompilerLog();
  GLuint getId(){return shaderHandler;}

private :
  GLenum shaderType;
  GLuint shaderHandler;
  SourceCode * shaderSource;
};

class Program
{
public :
  Program(const char * vertexFile,const char * fragmentFile);
  Program(const string vertexFile,const string fragmentFile);
  Program(Shader * vShader, Shader * fShader);
  ~Program();
  string getLinkerLog();
  string getLog();
  GLint getLoc(const char* name);
  void set1i(const char * name, GLint val);
  void set1f(const char * name, GLfloat val);
  void start();
  void stop();
private :
  Shader * vertexShader;
  Shader * fragmentShader;
  bool internShad;
  GLuint programHandler;
};

#endif //SHADER_H
