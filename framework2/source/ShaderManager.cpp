#include "ShaderManager.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include <fstream>

ShaderManager::ShaderManager()
{
    m_debugState=true;
}

ShaderManager::~ShaderManager()
{
    std::cerr<<"Closing down shader manager\n";

    std::map <std::string,ShaderProgram *>::iterator pbegin=m_shaderPrograms.begin();
    std::map <std::string,ShaderProgram *>::iterator pend=m_shaderPrograms.end();
    // delete each of the shader programs first (this will clear the maps in Program)
    // but not delete the shaders
    while(pbegin != pend)
    {
        delete pbegin->second;
        ++pbegin;
    }
    // now we de;ete all of the shaders ready
    std::map <std::string,Shader *>::iterator sbegin=m_shaders.begin();;
    std::map <std::string,Shader *>::iterator send=m_shaders.end();;

    while(sbegin != send)
    {
        delete sbegin->second;
        ++sbegin;
    }

}
void ShaderManager::attachShader(
    std::string _name,
    SHADERTYPE _type
    )
{
    m_shaders[_name]= new Shader(_name,_type);
}

void ShaderManager::compileShader(std::string _name)
{
    // get an iterator to the shaders
    std::map <std::string, Shader * >::const_iterator shader=m_shaders.find(_name);
    // make sure we have a valid shader
	if(shader!=m_shaders.end())
    {
        // grab the pointer to the shader and call compile
        shader->second->compile();
    }
    else {std::cerr<<"Warning shader not know in compile "<<_name.c_str();}

}



void ShaderManager::createShaderProgram(std::string _name)
{
    std::cerr<<"creating empty ShaderProgram "<<_name.c_str()<<"\n";
    m_shaderPrograms[_name]= new ShaderProgram(_name);
}

void ShaderManager::attachShaderToProgram(std::string _program,std::string _shader)

{

    // get an iterator to the shader and program
    std::map <std::string, Shader * >::const_iterator shader=m_shaders.find(_shader);
    std::map <std::string, ShaderProgram * >::const_iterator program=m_shaderPrograms.find(_program);

    // make sure we have a valid shader and program
	if(shader!=m_shaders.end() && program !=m_shaderPrograms.end())
    {
        // now attach the shader to the program
        program->second->attatchShader(shader->second);
        // now increment the shader ref count so we know if how many references
        shader->second->incrementRefCount();

        if (m_debugState == true)
        {
            std::cerr<<_shader.c_str()<<" attached to program "<<_program.c_str()<<"\n";
        }
    }
    else {std::cerr<<"Warning cant attach "<<_shader.c_str() <<" to "<<_program.c_str()<<"\n";}


}

void ShaderManager::loadShaderSource(std::string _shaderName, std::string _sourceFile)
{
    std::map <std::string, Shader * >::const_iterator shader=m_shaders.find(_shaderName);
    // make sure we have a valid shader and program
	if(shader!=m_shaders.end() )
    {
        shader->second->load(_sourceFile);
    }
    else {std::cerr<<"Warning shader not know in loadShaderSource "<<_shaderName.c_str();}

}

void ShaderManager::linkProgramObject(std::string _name)
{

    std::map <std::string, ShaderProgram * >::const_iterator program=m_shaderPrograms.find(_name);
    // make sure we have a valid  program
	if(program!=m_shaderPrograms.end() )
    {
        std::cerr<<"Linking "<<_name.c_str()<<"\n";
        program->second->link();
    }
    else {std::cerr<<"Warning Program not known in link "<<_name.c_str();}

}

void ShaderManager::use(std::string _name)
{
    std::map <std::string, ShaderProgram * >::const_iterator program=m_shaderPrograms.find(_name);
    // make sure we have a valid  program
	if(program!=m_shaderPrograms.end() )
    {
        std::cerr<<"Shader manager Use\n";
        program->second->use();
    }
    else
    {
        std::cerr<<"Warning Program not know in use "<<_name.c_str();
        glUseProgram(0);
    }

}


GLuint ShaderManager::getProgramID(std::string _name)
{
    std::map <std::string, ShaderProgram * >::const_iterator program=m_shaderPrograms.find(_name);
    // make sure we have a valid  program
	if(program!=m_shaderPrograms.end() )
    {
        return program->second->getID();
    }
    else
    {
        std::cerr<<"Warning Program not know in use "<<_name.c_str();
        return -1;
    }
}


void ShaderManager::bindAttribute(std::string _programName,GLuint _index, std::string _attribName)
{
    std::map <std::string, ShaderProgram * >::const_iterator program=m_shaderPrograms.find(_programName);
    // make sure we have a valid  program
	if(program!=m_shaderPrograms.end() )
    {
        program->second->bindAttrib(_index,_attribName);
    }
    else {std::cerr<<"Warning Program not know in bindAttrib "<<_programName.c_str();}


}


void ceckGLError(
    const std::string  &_file,
    const int _line
    )
{
    int err_num = glGetError();

	while (err_num != GL_NO_ERROR)
    {
        const char* str = (const char*)gluErrorString(err_num);
        printf("GL: %s  line: %d, file: %s\n",str,_line,_file.c_str());
        err_num = glGetError();

    }
}


ShaderProgram * ShaderManager::operator[](const std::string &_programName)
{
    std::map <std::string, ShaderProgram * >::const_iterator program=m_shaderPrograms.find(_programName);
    // make sure we have a valid  program
	if(program!=m_shaderPrograms.end() )
    {
        return  program->second;
    }
    else
    {
        std::cerr<<"Warning Program not know in [] "<<_programName.c_str();
        return 0;
    }
}



