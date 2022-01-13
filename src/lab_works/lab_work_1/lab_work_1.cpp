#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";
	const int indexVAO = 0;

	

	LabWork1::~LabWork1() { 
		glDeleteProgram( program );
		glDeleteBuffers( 1, &vbo ); 
		glDisableVertexArrayAttrib( vao, 0 );
		glDeleteVertexArrays( 1, &vao );
	}

	bool LabWork1::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;

		
		std::vector<float> vertices = { -0.5, 0.5 , 0.5, 0.5,  0.5, -0.5 };

		//création vao et vbo
		

		initVbao(vertices);

		//compilation des shaders
		std::string vertexShaderStr   = readFile( _shaderFolder + "lw1.vert" );
		std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );

		
		GLuint	vertex	 = glCreateShader( GL_VERTEX_SHADER );
		GLuint fragment = glCreateShader( GL_FRAGMENT_SHADER );

		const GLchar * vSrc = vertexShaderStr.c_str();
		const GLchar * fSrc = fragmentShaderStr.c_str();

		glShaderSource( vertex, 1, &vSrc, NULL );
		glShaderSource( fragment, 1, &fSrc, NULL );	

		glCompileShader( vertex );
		glCompileShader( fragment );

		
		// Check if compilation is ok.
		GLint compiled;
		glGetShaderiv( vertex, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertex, sizeof( log ), NULL, log );
			glDeleteShader( vertex );
			glDeleteShader( fragment );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;
		}

		
		//création du program
		program = glCreateProgram();
		glAttachShader( program, vertex );
		glAttachShader( program, fragment );

		glLinkProgram( program );
		

		// Check if link is ok.
		GLint linked;
		glGetProgramiv( program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}

		glDeleteShader( vertex );
		glDeleteShader( fragment );
		
		

		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		glUseProgram( program );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork1::initVbao( std::vector<float>  vertices)
	{
		glCreateBuffers( 1, &vbo );

		glNamedBufferData( vbo, vertices.size() * sizeof( float ), vertices.data(), GL_STATIC_DRAW );

		
		//Gl_buffer buf(vbo);
	
		

		glCreateVertexArrays( 1, &vao );

		glEnableVertexArrayAttrib( vao, indexVAO );

		glVertexArrayAttribFormat( vao, indexVAO, 2, GL_FLOAT, GL_FALSE, 0 );

		glVertexArrayVertexBuffer( vao, indexVAO, vbo, 0, sizeof( float ) * 2 );

		glVertexArrayAttribBinding( vao, 0, indexVAO );
	}
		

	void LabWork1::animate( const float p_deltaTime ) { 
	}

	void LabWork1::render() { 
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			
		glBindVertexArray( vao );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		glBindVertexArray( 0 );
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
	}

	void LabWork1::handleEvents( const SDL_Event & p_event ) {}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
