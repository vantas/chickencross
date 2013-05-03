#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>

//using namespace std;

//----------------------------------------------------------------------------------------------------------------------
void printInfoLog(
    const GLuint &_obj
    )
{
	GLint infologLength = 0;
	GLint charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(_obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,&infologLength);
	std::cerr<<"info log length "<<infologLength<<"\n";
    if(infologLength > 0)
	{
		infoLog = new char[infologLength];
		glGetShaderInfoLog(_obj, infologLength, &charsWritten, infoLog);

        std::cerr<<infoLog<<std::endl;
		delete [] infoLog;
        glGetShaderiv(_obj, GL_COMPILE_STATUS,&infologLength);
        if( infologLength == GL_FALSE)
        {
            std::cerr<<"Shader compile failed exiting \n";
            // exit(EXIT_FAILURE);
        }
    }

}


Shader::Shader(
    std::string _name,
    SHADERTYPE _type
    )
{
    m_name=_name;
    m_shaderType = _type;
    m_debugState = true;
    m_compiled=false;
    switch (_type)
    {
    case VERTEX : { m_shaderHandle = glCreateShader(GL_VERTEX_SHADER_ARB); break; }
    case FRAGMENT : { m_shaderHandle = glCreateShader(GL_FRAGMENT_SHADER_ARB); break; }
    case GEOMETRY : { m_shaderHandle = glCreateShader(GL_GEOMETRY_SHADER_EXT); break; }
    case TESSELATION : { m_shaderHandle = NULL; std::cerr<<"not yet implemented\n"; }
    }
    m_compiled = false;
    m_refCount=0;
    m_source=0;
}
Shader::~Shader()
{
	std::cerr <<"removing shader "<<m_name<<"\n";
    delete m_source;
    glDeleteShader(m_shaderHandle);
}

void Shader::compile()
{
    if (m_source == 0)
    {
        std::cerr<<"Warning no shader source loaded\n";
        return;
    }
	glCompileShader(m_shaderHandle);
	if(m_debugState==true)
	{
        std::cerr <<"Compiling Shader "<<m_name<<"\n";
		printInfoLog(m_shaderHandle);
    }
    m_compiled=true;
}


void Shader::load(
    std::string _name
    )
{
    // see if we already have some source attached
    if(m_source !=0)
    {
        std::cerr<<"deleting existing source code\n";
        delete m_source;
    }
    std::ifstream shaderSource(_name.c_str());
    if (!shaderSource.is_open())
    {
        std::cerr<<"File not found "<<_name.c_str()<<"\n";
        exit(EXIT_FAILURE);
    }
    // now read in the data
    m_source = new std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
    shaderSource.close();
    *m_source+="\0";

    const char* data=m_source->c_str();
    glShaderSource(m_shaderHandle , 1, &data,NULL);
    m_compiled=false;

    if (m_debugState == true)
    {
        std::cerr<<"Shader Loaded and source attached\n";
        printInfoLog(m_shaderHandle);
    }
}
