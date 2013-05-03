#include "ShaderProgram.h"
#include "ShaderManager.h"

//----------------------------------------------------------------------------------------------------------------------
ShaderProgram::ShaderProgram(std::string _name)
{
	m_programID = glCreateProgram();
    std::cerr <<"Created program id is "<<m_programID<<"\n";
    m_debugState=true;
    m_programName=_name;
    m_linked=false;
    m_active=false;
}
ShaderProgram::~ShaderProgram()
{
    std::cerr<<"removing shader program "<< m_programName<<"\n";
    glDeleteProgram(m_programID);
}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::use()
{
    //std::cerr<<"Using shader "<<m_programName<<" id "<<m_programID<<"\n";
    glUseProgram(m_programID);
    ceckGLError(__FILE__,__LINE__);
    m_active=true;
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::unbind()
{
    m_active=false;
    glUseProgram(0);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::attatchShader(Shader *_shader)
{
    m_shaders.push_back(_shader);
    glAttachShader(m_programID,_shader->getShaderHandle());
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::bindAttrib(GLuint _index, std::string _attribName)
{
    if(m_linked == true)
    {
        std::cerr<<"Warning binding attribute after link\n";
    }
    m_attribs[_attribName]=_index;
    glBindAttribLocation(m_programID,_index,_attribName.c_str());
    std::cerr<<"bindAttribLoc "<<m_programID<<" index "<<_index<<" name "<<_attribName<<"\n";
    ceckGLError(__FILE__,__LINE__);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::link()
{
    glLinkProgram(m_programID);
	if(m_debugState==true)
    {
        std::cerr <<"linking Shader "<< m_programName.c_str()<<"\n";
    }
    GLint infologLength = 0;

    glGetProgramiv(m_programID,GL_INFO_LOG_LENGTH,&infologLength);
    std::cerr<<"Link Log Length "<<infologLength<<"\n";

    if(infologLength > 0)
	{
		char *infoLog = new char[infologLength];
        GLint charsWritten  = 0;

        glGetProgramInfoLog(m_programID, infologLength, &charsWritten, infoLog);

        std::cerr<<infoLog<<std::endl;
		delete [] infoLog;
        glGetProgramiv(m_programID, GL_LINK_STATUS,&infologLength);
        if( infologLength == GL_FALSE)
        {
            std::cerr<<"Program linl failed exiting \n";
            exit(EXIT_FAILURE);
        }
    }
    m_linked=true;
}


//----------------------------------------------------------------------------------------------------------------------
GLuint ShaderProgram::getUniformLocation(
    const char* _name
    ) const
{
    GLint loc = glGetUniformLocation( m_programID ,_name);
    if (loc == -1)
    {
        std::cerr<<"Uniform \""<<_name<<"\" not found in Program \""<<m_programName<<"\"\n";
    }
    return loc;
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform1f(
    const char* _varname,
    float _v0
    ) const
{
    glUniform1f(getUniformLocation(_varname),_v0);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform2f(
    const char* _varname,
    float _v0,
    float _v1
    ) const
{
    glUniform2f(getUniformLocation(_varname),_v0,_v1);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform3f(
    const char* _varname,
    float _v0,
    float _v1,
    float _v2
    ) const
{
    glUniform3f(getUniformLocation(_varname),_v0,_v1,_v2);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform4f(
    const char* _varname,
    float _v0,
    float _v1,
    float _v2,
    float _v3
    ) const
{
    glUniform4f(getUniformLocation(_varname),_v0,_v1,_v2,_v3);
}


//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform1fv(
    const char* _varname,
    size_t _count,
    const float* _value
    ) const
{
    glUniform1fv(getUniformLocation(_varname),_count,_value);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform2fv(
    const char* _varname,
    size_t _count,
    const float* _value
    ) const
{
    glUniform2fv(getUniformLocation(_varname),_count,_value);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform3fv(
    const char* _varname,
    size_t _count,
    const float* _value
    ) const
{
    glUniform3fv(getUniformLocation(_varname),_count,_value);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform4fv(
    const char* _varname,
    size_t _count,
    const float* _value
    ) const
{
    glUniform4fv(getUniformLocation(_varname),_count,_value);

}


//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform1i(
    const char* _varname,
    GLint _v0
    ) const
{
    glUniform1i(getUniformLocation(_varname),_v0);

}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform2i(
    const char* _varname,
    GLint _v0,
    GLint _v1
    ) const
{
    glUniform2i(getUniformLocation(_varname),_v0,_v1);

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform3i(
    const char* _varname,
    GLint _v0,
    GLint _v1,
    GLint _v2
    ) const
{
    glUniform3i(getUniformLocation(_varname),_v0,_v1,_v2);

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform4i(
    const char* _varname,
    GLint _v0,
    GLint _v1,
    GLint _v2,
    GLint _v3
    ) const
{
    glUniform4i(getUniformLocation(_varname),_v0,_v1,_v2,_v3);

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform1iv(
    const char* _varname,
    size_t _count,
    const GLint* _value
    ) const
{
    glUniform1iv(getUniformLocation(_varname),_count,_value);

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform2iv(
    const char* _varname,
    size_t _count,
    const GLint* _value
    ) const
{
    glUniform2iv(getUniformLocation(_varname),_count,_value);
}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform3iv(
    const char* _varname,
    size_t _count,
    const GLint* _value
    ) const
{
    glUniform3iv(getUniformLocation(_varname),_count,_value);

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniform4iv(
    const char* _varname,
    size_t _count,
    const GLint* _value
    ) const
{
    glUniform4iv(getUniformLocation(_varname),_count,_value);
}


//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniformMatrix2fv(
    const char* _varname,
    size_t _count,
    bool _transpose,
    const float* _value
    ) const
{
    glUniformMatrix2fv(getUniformLocation(_varname),_count,_transpose,_value);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniformMatrix3fv(
    const char* _varname,
    size_t _count,
    bool _transpose,
    const float* _value
    ) const
{
    glUniformMatrix3fv(getUniformLocation(_varname),_count,_transpose,_value);

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniformMatrix4fv(
    const char* _varname,
    size_t _count,
    bool _transpose,
    const float* _value
    ) const
{
    glUniformMatrix4fv(getUniformLocation(_varname),_count,_transpose,_value);

}

//----------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniformMatrix2x3fv(
    const char* _varname,
    size_t _count,
    bool _transpose,
    const float* _value
    ) const
{
    glUniformMatrix2x3fv(getUniformLocation(_varname),_count,_transpose,_value);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniformMatrix2x4fv(
    const char* _varname,
    size_t _count,
    bool _transpose,
    const float* _value
    ) const
{
    glUniformMatrix2x4fv(getUniformLocation(_varname),_count,_transpose,_value);

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniformMatrix3x2fv(
    const char* _varname,
    size_t _count,
    bool _transpose,
    const float* _value
    ) const
{
    glUniformMatrix3x2fv(getUniformLocation(_varname),_count,_transpose,_value);

}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniformMatrix3x4fv(
    const char* _varname,
    size_t _count,
    bool _transpose,
    const float* _value
    ) const
{
    glUniformMatrix3x4fv(getUniformLocation(_varname),_count,_transpose,_value);

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniformMatrix4x2fv(
    const char* _varname,
    size_t _count,
    bool _transpose,
    const float* _value
    ) const
{
    glUniformMatrix4x2fv(getUniformLocation(_varname),_count,_transpose,_value);

}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::setUniformMatrix4x3fv(
    const char* _varname,
    size_t _count,
    bool _transpose,
    const float* _value
    ) const
{
    glUniformMatrix4x3fv(getUniformLocation(_varname),_count,_transpose,_value);

}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::getUniformfv(
    const char* _varname,
    float* o_values
    ) const
{
    glGetUniformfv(m_programID, getUniformLocation(_varname),o_values);
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::getUniformiv(
    const char* _varname,
    int32_t* o_values
    ) const
{
    glGetUniformiv(m_programID,getUniformLocation(_varname),o_values);
}


//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::enableAttribArray(
    const char* _name
    ) const
{

    std::map <std::string, GLuint >::const_iterator index=m_attribs.find(_name);


    if(index!=m_attribs.end())
    {
        std::cerr<<"Enable attrib "<<index->second<<"\n";
        glEnableVertexAttribArray( index->second  );
    }
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::disableAttribArray(
    const char* _name
    ) const
{
    glDisableVertexAttribArray(getUniformLocation(_name));

}

bool ShaderProgram::vertexAttribPointer(
    const char* _name,
    GLint _size,
    GLenum _type,
    GLsizei _stride,
    const GLvoid *_data,
    bool _normalise
    ) const
{

    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttribPointer(attrib->second,_size,_type,_normalise,_stride,_data);
        return  true;
    }
    else
    {
        return false;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib1f(
    const char * _name,
    GLfloat  	_v0
    ) const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib1f(attrib->second, _v0);

    }

}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib2f(
    const char * _name,
    GLfloat  	_v0,
    GLfloat  	_v1
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib2f(attrib->second, _v0,_v1);

    }

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib3f(
    const char * _name,
    GLfloat  	_v0,
    GLfloat  	_v1,
    GLfloat  	_v2
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib3f(attrib->second, _v0,_v1,_v2);

    }

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib4f(
    const char * _name,
    GLfloat  	_v0,
    GLfloat  	_v1,
    GLfloat  	_v2,
    GLfloat  	_v3
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib4f(attrib->second, _v0,_v1,_v2,_v3);

    }

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib1fv(
    const char * _name,
    const GLfloat * 	_v
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib1fv(attrib->second, _v);
    }
}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib2fv(
    const char * _name,
    const GLfloat * _v
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib2fv(attrib->second, _v);
    }

}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib3fv(
    const char * _name,
    const GLfloat * _v
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib3fv(attrib->second, _v);
    }

}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib4fv(
    const char * _name,
    const GLfloat * _v
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib4fv(attrib->second, _v);
    }

}

//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib4iv(
    const char * _name,
    const GLint * _v
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib4iv(attrib->second, _v);
    }

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib4uiv(
    const char * _name,
    const GLuint * _v
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib4uiv(attrib->second, _v);
    }

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib4Niv(
    const char * _name,
    const GLint * _v
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib4Niv(attrib->second, _v);
    }

}
//----------------------------------------------------------------------------------------------------------------------
void ShaderProgram::vertexAttrib4Nuiv(
    const char * _name,
    const GLuint * _v
    )const
{
    std::map <std::string, GLuint >::const_iterator attrib=m_attribs.find(_name);
    // make sure we have a valid  program
	if(attrib!=m_attribs.end() )
    {
        glVertexAttrib4Nuiv(attrib->second, _v);
    }

}
