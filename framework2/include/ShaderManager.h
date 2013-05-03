#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include "Graphics.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include <iostream>
#include <vector>
#include <map>


extern void printInfoLog(
    const GLuint &_obj
    );
extern void ceckGLError(
    const std::string  &_file,
    const int _line
    );
//----------------------------------------------------------------------------------------------------------------------
///  @class ShaderManager  "ShaderManager.h"
///  @brief this class creates a shader manager which as many Program objects
/// and Shader Objects we wish to create. Eventually this will be integrated into
/// NGL as a singleton but should work as a stand alone project at present.
///  @author Jonathan Macey
///  @version 1.0
//----------------------------------------------------------------------------------------------------------------------

class ShaderManager
{
public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor
    //----------------------------------------------------------------------------------------------------------------------
    ShaderManager();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor will remove all attached objects
    //----------------------------------------------------------------------------------------------------------------------
    ~ShaderManager();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief link the ShaderProgram referenced by _name
    /// @param _name the name of the ShaderProgram to link
    //----------------------------------------------------------------------------------------------------------------------
    bool linkShader(
        std::string _name
        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create an empty ShaderProgram for us to attach shaders etc to
    /// @param _name the name of the ShaderProgram to link
    //----------------------------------------------------------------------------------------------------------------------
    void createShaderProgram(
        std::string _name
        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief attatch a Shader to the ShaderProgram referenced by _name
    /// @param _name the name of the ShaderProgram to attach
    //----------------------------------------------------------------------------------------------------------------------
    void attachShader(
        std::string _name,
        SHADERTYPE _type
        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief attatch a Shader to the ShaderProgram referenced by _name
    /// @param _program the name of the ShaderProgram to attach to
    /// @param _shader the name of the Shader to attach to Program
    //----------------------------------------------------------------------------------------------------------------------

    void attachShaderToProgram(
        std::string _program,
        std::string _shader
        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get the Program ID of the GL Program by name
    /// @param _name the name of the ShaderProgram to find
    /// @returns the id of the program found or -1 on error
    //----------------------------------------------------------------------------------------------------------------------
    GLuint getProgramID(
        std::string _name
        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief compile the shader from _name
    /// @param _name the name of the ShaderProgram to compile
    //----------------------------------------------------------------------------------------------------------------------
    void compileShader(
        std::string _name
        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief link the program Object  from _name
    /// @param _name the name of the ShaderProgram to link
    //----------------------------------------------------------------------------------------------------------------------
    void linkProgramObject(
        std::string _name
        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief toggle debug mode
    //----------------------------------------------------------------------------------------------------------------------
    inline void toggleDebug() { m_debugState ^=true;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set active shader to name (if not found sets glProgramObject(0)
    /// @param _name the name of the ShaderProgram to use
    //----------------------------------------------------------------------------------------------------------------------
    void use(
        std::string _name
        );

    void bindAttribute(
        std::string _programName,
        GLuint _index,
        std::string _attribName
        );
    void loadShaderSource(
        std::string _shaderName,
        std::string _sourceFile
        );
    ShaderProgram * operator[](const std::string &_programName);
private :
    std::map <std::string,ShaderProgram *> m_shaderPrograms;
    std::map <std::string,Shader *> m_shaders;
    bool m_debugState;
};



#endif
