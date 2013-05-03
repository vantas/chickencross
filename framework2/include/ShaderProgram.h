#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Shader.h"

typedef int int32_t;

//----------------------------------------------------------------------------------------------------------------------
///  @class ShaderProgram  "ShaderProgram.h"
///  @brief This class contains a single ShaderProgram which may have many
///  different shaders attached to it as well as wrappers to call the various
/// OpenGL functions to set paramaters for this Program
///  @author Jonathan Macey
///  @version 1.0
//----------------------------------------------------------------------------------------------------------------------

class ShaderProgram
{
public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create an OpenGL shader program
    /// it is initially empty and just has a name for id
    /// @param _name the name of the Program Object
    //----------------------------------------------------------------------------------------------------------------------
    ShaderProgram(
        std::string _name
        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor
    //----------------------------------------------------------------------------------------------------------------------
    ~ShaderProgram();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief use this Shader object as the current Active shader
    /// once this is set it is active until unbind us called
    //----------------------------------------------------------------------------------------------------------------------
    void use();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to unbind the current Program Object
    /// this will set the shader to be the glUseProgram(0) default value
    /// @note if we don't use this we will lose the m_active state values and we will not
    /// be able to check the current active state etc so use this once shader is finished.
    //----------------------------------------------------------------------------------------------------------------------
    void unbind();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief attach a shader to the ProgramObject
    /// @param _shader the Shader object to attatch
    /// \note we can attach many shaders to the ProgramObject.
    //----------------------------------------------------------------------------------------------------------------------
    void attatchShader(
        Shader *_shader
        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief bind an attribute in the Program object to _index using attribname
    /// @param _index the index number we wish to bind to
    /// @param _attribName the name of the attribute we wish to use
    //----------------------------------------------------------------------------------------------------------------------
    void bindAttrib(
        GLuint index,
        std::string _attribName
        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief link our program object with the attatched shaders
    /// if shader are not attached the program will report the linker
    /// errors
    //----------------------------------------------------------------------------------------------------------------------
    void link();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get the ProgramID for the Program
    //----------------------------------------------------------------------------------------------------------------------
    inline GLuint getID() const {return m_programID;}

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  returns the ID of the uniform attribute called 'name'.
    /// @return the uniform variable id
    //----------------------------------------------------------------------------------------------------------------------
    GLuint getUniformLocation(
        const char* _name
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @name  Set uniform argument values from the argument name
    //----------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a float
    /// @param  _varname - name of the uniform variable
    /// @param  _v0 - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform1f(
        const char* _varname,
        float __v0
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a float2
    /// @param  _varname - name of the uniform variable
    /// @param  _v0 - new value for the variable
    /// @param  _v1 - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform2f(
        const char* _varname,
        float _v0,
        float _v1
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a float3
    /// @param  _varname - name of the uniform variable
    /// @param  _v0 - new value for the variable
    /// @param  _v1 - new value for the variable
    /// @param  _v2 - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform3f(
        const char* _varname,
        float _v0,
        float _v1,
        float _v2
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a float4
    /// @param  _varname - name of the uniform variable
    /// @param  _v0 - new value for the variable
    /// @param  _v1 - new value for the variable
    /// @param  _v2 - new value for the variable
    /// @param  _v3 - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform4f(
        const char* _varname,
        float _v0,
        float _v1,
        float _v2,
        float _v3
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 1D float array
    /// @param  _varname - name of the uniform variable
    /// @param  count - num elements in the array
    /// @param  value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform1fv(
        const char* _varname,
        size_t _count,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 2D float array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _value - new _value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform2fv(
        const char* _varname,
        size_t _count,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 3D float array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _value - new _value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform3fv(
        const char* _varname,
        size_t _count,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 4D float array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _value - new _value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform4fv(
        const char* _varname,
        size_t _count,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @name  Set Program Integer Args
    /// @brief  sets '_varname' as an int
    /// @param  _varname - name of the uniform variable
    /// @param  _v0 - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform1i(
        const char* _varname,
        GLint _v0
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a int2
    /// @param  _varname - name of the uniform variable
    /// @param  _v0 - new value for the variable
    /// @param  _v1 - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform2i(
        const char* _varname,
        GLint _v0,
        GLint _v1
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a int3
    /// @param  _varname - name of the uniform variable
    /// @param  _v0 - new value for the variable
    /// @param  _v1 - new value for the variable
    /// @param  _v2 - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform3i(
        const char* _varname,
        GLint _v0,
        GLint _v1,
        GLint _v2
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a float4
    /// @param  _varname -name of the uniform variable
    /// @param  _v0 - new value for the variable
    /// @param  _v1 - new value for the variable
    /// @param  _v2 - new value for the variable
    /// @param  _v3 - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform4i(
        const char* _varname,
        GLint _v0,
        GLint _v1,
        GLint _v2,
        GLint _v3
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 1D int array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform1iv(
        const char* _varname,
        size_t _count,
        const GLint* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 2D int array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform2iv(
        const char* _varname,
        size_t _count,
        const GLint* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 3D int array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform3iv(
        const char* _varname,
        size_t _count,
        const GLint* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 4D int array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _value - new _value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniform4iv(
        const char* _varname,
        size_t _count,
        const GLint* _value
        ) const;


    //----------------------------------------------------------------------------------------------------------------------
    /// @name  Set Program Matrix Args
    //----------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 2D matrix array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _transpose - if true will transpose the matrix values
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniformMatrix2fv(
        const char* _varname,
        size_t _count,
        bool _transpose,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 3D matrix array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _transpose - if true will transpose the matrix values
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniformMatrix3fv(
        const char* _varname,
        size_t _count,
        bool _transpose,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 4D matrix array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _transpose - if true will transpose the matrix values
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniformMatrix4fv(
        const char* _varname,
        size_t _count,
        bool _transpose,
        const float* _value
        ) const;



    //----------------------------------------------------------------------------------------------------------------------
    /// \name  Set Program Matrix Args (only available in OpenGL 2.1)
    //----------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 2x3 matrix array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _transpose - if true will transpose the matrix values
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniformMatrix2x3fv(
        const char* _varname,
        size_t _count,
        bool _transpose,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 2x4 matrix array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _transpose - if true will transpose the matrix values
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniformMatrix2x4fv(
        const char* _varname,
        size_t _count,
        bool _transpose,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 3x2 matrix array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _transpose - if true will transpose the matrix values
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniformMatrix3x2fv(
        const char* _varname,
        size_t _count,
        bool _transpose,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 3x4 matrix array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _transpose - if true will transpose the matrix values
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniformMatrix3x4fv(
        const char* _varname,
        size_t _count,
        bool _transpose,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 4x2 matrix array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _transpose - if true will transpose the matrix values
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniformMatrix4x2fv(
        const char* _varname,
        size_t _count,
        bool _transpose,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  sets '_varname' as a 4x3 matrix array
    /// @param  _varname - name of the uniform variable
    /// @param  _count - num elements in the array
    /// @param  _transpose - if true will transpose the matrix values
    /// @param  _value - new value for the variable
    //----------------------------------------------------------------------------------------------------------------------
    void setUniformMatrix4x3fv(
        const char* _varname,
        size_t _count,
        bool _transpose,
        const float* _value
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @name  get program arguments
    //----------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  gets the current value of the specified uniform var
    /// @param  _name - name of the uniform variable to retrieve
    /// @param  o_values - the output value
    //----------------------------------------------------------------------------------------------------------------------
    void getUniformfv(
        const char* _name,
        float* o_values
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  gets the current value of the specified uniform var
    /// @param  _name - name of the uniform variable to retrieve
    /// @param  o_values - the output value
    //----------------------------------------------------------------------------------------------------------------------
    void getUniformiv(
        const char* _name,
        int32_t* o_values
        ) const;


    //----------------------------------------------------------------------------------------------------------------------
    /// @name   vertex array functions.
    /// @brief  These functions simplify the binding of per-vertex data as a
    ///         vertex array (or VBO).
    //----------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  enables the specified varying array
    /// @param  _name - the name of the varying attr array to enable
    //----------------------------------------------------------------------------------------------------------------------
    void enableAttribArray(
        const char* _name
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  disables the specified varying array
    /// @param  _name - the name of the varying attr array to enable
    //----------------------------------------------------------------------------------------------------------------------
    void disableAttribArray(
        const char* _name
        ) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  similar to glVertexPointer, but for varying attributes.
    /// @param  _name - the name of the attribute
    /// @param  _size - num elements in each attribute (i.e. 1,2,3,4)
    /// @param _type - the type of data we are attaching
    /// @param  _stride - the size in bytes of each attribute
    /// @param  _data - the data
    /// @param  _normalise - true to normalise fixed point floats
    /// @return true if found and attached else false
    //----------------------------------------------------------------------------------------------------------------------
    bool vertexAttribPointer(
        const char* _name,
        GLint _size,
        GLenum _type,
        GLsizei _stride,
        const GLvoid *_data,
        bool _normalise=GL_FALSE
        ) const;


    void vertexAttrib1f(
        const char * _name,
        GLfloat  	_v0
        ) const;

    void vertexAttrib2f(
        const char * _name,
        GLfloat  	_v0,
        GLfloat  	_v1
        )const;
    void vertexAttrib3f(
        const char * _name,
        GLfloat  	_v0,
        GLfloat  	_v1,
        GLfloat  	_v2
        )const;
    void vertexAttrib4f(
        const char * _name,
        GLfloat  	_v0,
        GLfloat  	_v1,
        GLfloat  	_v2,
        GLfloat  	_v3
        )const;
    void vertexAttrib1fv(
        const char * _name,
        const GLfloat * 	_v
        )const;
    void vertexAttrib2fv(
        const char * _name,
        const GLfloat * _v
        )const;

    void vertexAttrib3fv(
        const char * _name,
        const GLfloat * _v
        )const;

    void vertexAttrib4fv(
        const char * _name,
        const GLfloat * _v
        )const;

    void vertexAttrib4iv(
        const char * _name,
        const GLint * _v
        )const;
    void vertexAttrib4uiv(
        const char * _name,
        const GLuint * _v
        )const;
    void vertexAttrib4Niv(
        const char * _name,
        const GLint * _v
        )const;
    void vertexAttrib4Nuiv(
        const char * _name,
        const GLuint * _v
        )const;

private :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the current Program has been linked
    //----------------------------------------------------------------------------------------------------------------------
    bool m_linked;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a list of the shader objects attached to the program
    //----------------------------------------------------------------------------------------------------------------------
    std::vector <Shader *> m_shaders;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the program id for this program object
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_programID;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief debug mode flag
    //----------------------------------------------------------------------------------------------------------------------
    bool m_debugState;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief text name of the current program used in lookup and debugging
    //----------------------------------------------------------------------------------------------------------------------
    std::string m_programName;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a list of attributes for this object, mapping name to ID number
    //----------------------------------------------------------------------------------------------------------------------
    std::map <std::string, GLuint > m_attribs;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief indicate if this program is the current active program
    //----------------------------------------------------------------------------------------------------------------------
    bool m_active;

};

#endif
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
