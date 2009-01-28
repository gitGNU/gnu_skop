// Copyright 2007 Marc Betoule

// This file is part of SkOP.

// SkOP is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// any later version.
  
// SkOP is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
  
// You should have received a copy of the GNU General Public License
// along with SkOP; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


/**
 * \file shader.cpp
 * \brief class implementation for opengl initialisation and shader code manipulation.
 */    

#include <iostream>
#include <fstream>
#include "shader.h"

using namespace std;
bool GlslContext::useGLSL = false;
bool GlslContext::initdone = false;
int GlslContext::CheckGLError(char *file, int line){
  GLenum glErr;
  int retCode = 0;

  glErr = glGetError();
  while(glErr != GL_NO_ERROR)
    {
      cout << "GL Error #" << glErr << "(" << gluErrorString(glErr) << ") " << " in File " << file << " at line: " << line << endl;
      retCode = 1;
      glErr = glGetError();
    }
  return retCode;
}

bool GlslContext::initGLExtensions()
{
  if(initdone) return true;
  initdone = true;

  GLenum err = glewInit();
 if(GLEW_OK != err){
    cout << "Error :" << glewGetErrorString(err) << "\n";
    initdone = false;
    return false;
  }

  cout << "OpenGL Vendor : " << (char*) glGetString(GL_VENDOR) << "\n";
  cout << "OpenGL Renderer : " << (char*) glGetString(GL_RENDERER) << "\n";
  cout << "OpenGL Version : " << (char*) glGetString(GL_VERSION) << "\n\n";
  cout << "OpenGL Version : " << (char*) glGetString(GL_EXTENSIONS) << "\n\n";

  if(!GlslContext::checkGLSL()){
    initdone = false;
    return false;
  }

  return true;
}

bool GlslContext::checkGLSL()
{
    if (useGLSL) return true;  // already initialized and GLSL is available
    useGLSL = true;

    if (!initdone) initGLExtensions();  // extensions were not yet initialized!!

   if (GLEW_VERSION_2_0)
    {
       cout << "OpenGL 2.0 is available!" << endl;
    }
    else if (GLEW_VERSION_1_5)
    {
       cout << "OpenGL 1.5 core functions are available" << endl;
    }
    else if (GLEW_VERSION_1_4)
    {
       cout << "OpenGL 1.4 core functions are available" << endl;
    }
    else if (GLEW_VERSION_1_3)
    {
       cout << "OpenGL 1.3 core functions are available" << endl;
    }
    else if (GLEW_VERSION_1_2)
    {
       cout << "OpenGL 1.2 core functions are available" << endl;
    }

    if (GL_TRUE != glewGetExtension("GL_ARB_fragment_shader"))
    {
        cout << "GL_ARB_fragment_shader extension is not available!";
        useGLSL = false;
    }
    if (GL_TRUE != glewGetExtension("GL_ARB_vertex_shader"))
    {
        cout << "GL_ARB_vertex_shader extension is not available!";
        useGLSL = false;
    }

    if (GL_TRUE != glewGetExtension("GL_ARB_shader_objects"))
    {
        cout << "GL_ARB_shader_objects extension is not available!";
        useGLSL = false;
    }

    if (GL_TRUE != glewGetExtension("GL_ARB_texture_float"))
    {
        cout << "GL_ARB_texture_float extension is not available!";
        useGLSL = false;
    }
    if (useGLSL)
    {
        cout << "[OK] OpenGL Shading Language is available!\n\n";
    }
    else
    {
        cout << "[FAILED] OpenGL Shading Language is not available...\n\n";
    }

    return useGLSL;
}
SourceCode::SourceCode(const char * filename){
  ifstream entree(filename, ios::in);
  if(!entree){
    cerr << "Cannot read file " << filename << "\n";
  }
  entree.seekg(0,ios::end);
  int count = 0;
  count = entree.tellg();
  char * contentmod = new char[count+1];
  entree.seekg(0,ios::beg);
  entree.read(contentmod, count);
  contentmod[count] = '\0';
  content = contentmod;
  delete contentmod;
  entree.close();
}

SourceCode::SourceCode(const string code){
  content = code;
}

SourceCode::~SourceCode(){
  //delete content;
}

Shader::Shader(const char * filename,GLenum sT){
  shaderSource = new SourceCode(filename);
  shaderType = sT;
  shaderHandler = glCreateShader(shaderType);
  const char * psource = shaderSource->getContent();

  glShaderSource(shaderHandler, 1, &psource,NULL);

  glCompileShader(shaderHandler);
}

Shader::Shader(const string code,GLenum sT){
  shaderSource = new SourceCode(code);
  shaderType = sT;
  shaderHandler = glCreateShader(shaderType);
  const char * psource = shaderSource->getContent();

  glShaderSource(shaderHandler, 1, &psource,NULL);

  glCompileShader(shaderHandler);
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;
  string info;

  glGetShaderiv(shaderHandler, GL_INFO_LOG_LENGTH,&infologLength);
  if (infologLength > 0){
    infoLog = new char[infologLength];
    glGetShaderInfoLog(shaderHandler, infologLength, &charsWritten, infoLog);
    info = infoLog;
    delete infoLog;
  }
  cout << info;
}

Shader::~Shader(){
  delete shaderSource;
}

string Shader::getCompilerLog(){
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;
  string info;

  glGetShaderiv(shaderHandler, GL_INFO_LOG_LENGTH,&infologLength);
  if (infologLength > 0){
    infoLog = new char[infologLength];
    glGetShaderInfoLog(shaderHandler, infologLength, &charsWritten, infoLog);
    info = infoLog;
    delete infoLog;
  }
  return info;
}

Program::Program(const char * vertexFile,const char * fragmentFile){
  vertexShader = new Shader(vertexFile,GL_VERTEX_SHADER);
  fragmentShader = new Shader(fragmentFile,GL_FRAGMENT_SHADER);
  programHandler = glCreateProgram();
  glAttachShader(programHandler,vertexShader->getId());
  glAttachShader(programHandler,fragmentShader->getId());
  glLinkProgram(programHandler);
  internShad = true;
}

Program::Program(const string vertexCode,const string fragmentCode){
  vertexShader = new Shader(vertexCode,GL_VERTEX_SHADER);
  fragmentShader = new Shader(fragmentCode,GL_FRAGMENT_SHADER);
  programHandler = glCreateProgram();
  glAttachShader(programHandler,vertexShader->getId());
  glAttachShader(programHandler,fragmentShader->getId());
  glLinkProgram(programHandler);
  internShad = true;
}

Program::Program(Shader * vShader, Shader * fShader){
  programHandler = glCreateProgram();
  vertexShader = vShader;
  fragmentShader = fShader;
  glAttachShader(programHandler,vertexShader->getId());
  glAttachShader(programHandler,fragmentShader->getId());
  glLinkProgram(programHandler);
  internShad = false;
}
Program::~Program(){
  if(internShad){
    delete vertexShader;
    delete fragmentShader;
  }
}

string Program::getLinkerLog(){
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;
  string info;

  glGetProgramiv(programHandler, GL_INFO_LOG_LENGTH,&infologLength);
  if (infologLength > 0){
    infoLog = new char[infologLength];
    glGetProgramInfoLog(programHandler, infologLength, &charsWritten, infoLog);
    info = infoLog;
    delete infoLog;
  }
  return info;
}

string Program::getLog(){
  string intro = "Vertex Shader Log:\n";
  string info = intro + vertexShader->getCompilerLog();
  info = info + "Fragment Shader Log:\n" + fragmentShader->getCompilerLog();
  info = info + "Linker Log:\n" + getLinkerLog();
  return info;
}

GLint Program::getLoc(const char* name){
  return glGetUniformLocation(programHandler, name);
}
void Program::set1i(const char * name, GLint val){
  GLint loc = getLoc(name);
  glUniform1i(loc, val);
}

void Program::set1f(const char * name, GLfloat val){
  GLint loc = getLoc(name);
  glUniform1f(loc, val);
}

void Program::start(){
  glUseProgram(programHandler);
}

void Program::stop(){
  glUseProgram(0);
}
