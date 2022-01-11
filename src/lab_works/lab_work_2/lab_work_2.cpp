#include "lab_work_2.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "utils/random.hpp"
#include <application.hpp>

namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";
	const int indexVAO = 0;

	

	LabWork2::~LabWork2()
	{ 
		glDeleteProgram( program );
		glDeleteBuffers( 1, &vbo ); 
		glDeleteBuffers( 1, &ebo );
		glDeleteBuffers( 1, &vbo_color );
		glDisableVertexArrayAttrib( vao, 0 );
		glDeleteVertexArrays( 1, &vao );
		
	}

	bool LabWork2::init()
	{
		std::cout << "Initializing lab work 2..." << std::endl;

		luminosite = 1.0;
		_time = 0;
		nb_triangle = 64;
		

		
		std::vector<Vec2f> vertices = circle( nb_triangle );

		
		
		//création vao et vbo
		

		initVbao(vertices);

		//compilation des shaders
		std::string vertexShaderStr   = readFile( _shaderFolder + "lw2.vert" );
		std::string fragmentShaderStr = readFile( _shaderFolder + "lw2.frag" );

		
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
		
		locTrans = glGetUniformLocation( program, "uTranslationX" );
		locLum	 = glGetUniformLocation( program, "uLuminosite" );

		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		glProgramUniform1f( program, locLum, luminosite );

		glUseProgram( program );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork2::initVbao( std::vector<Vec2f>  vertices)

		//init buffers
	{
		glCreateBuffers( 1, &vbo );

		glCreateBuffers( 1, &vbo_color );

		glCreateBuffers( 1, &ebo );

		glNamedBufferData( vbo, vertices.size() * sizeof( Vec2f ), vertices.data(), GL_STATIC_DRAW );

		glNamedBufferData( ebo, indice.size() * sizeof( unsigned int ), indice.data(), GL_STATIC_DRAW );

		glNamedBufferData( vbo_color, color.size() * sizeof( Vec3f ), color.data(), GL_STATIC_DRAW );

		//vao init

		glCreateVertexArrays( 1, &vao );

		//layout 0 vbo

		glEnableVertexArrayAttrib( vao, indexVAO );

		glVertexArrayAttribFormat( vao, indexVAO, 2, GL_FLOAT, GL_FALSE, 0 );

		//layout 1 vbo_color

		glEnableVertexArrayAttrib( vao, 1 );

		glVertexArrayAttribFormat( vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );

		//link différents vbo

		glVertexArrayVertexBuffer( vao, indexVAO, vbo, 0, sizeof( Vec2f ) );

		glVertexArrayVertexBuffer( vao, 1, vbo_color, 0, sizeof( Vec3f ) );

		//link ebo

		glVertexArrayElementBuffer( vao, ebo );

		//bind vao

		glVertexArrayAttribBinding( vao, 0, indexVAO );

		glVertexArrayAttribBinding( vao, 1, 1 );

	}



		

	void LabWork2::animate( const float p_deltaTime ) { 
		_time += p_deltaTime;
		glProgramUniform1f( program, locTrans, glm::sin( _time ) );
	}

	void LabWork2::render() { 
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			
		glBindVertexArray( vao );
		glDrawElements( GL_TRIANGLES, nb_triangle * 3, GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void LabWork2::handleEvents( const SDL_Event & p_event ) {}

	void LabWork2::displayUI()
	{
		ImGui::Begin( "Settings lab work 2" );

		if ( ImGui::SliderFloat( "luminosite", &luminosite, 0.0, 1.0 ) ) {
			glProgramUniform1f( program, locLum, luminosite );
		}

		if ( ImGui::ColorEdit3( "back_color", glm::value_ptr( _bgColor ) ) ) {
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}

		

		if ( ImGui::SliderInt( "nb_triangle", &nb_triangle, 3.0, 128.0 ) ) {
			std::vector<Vec2f> vertices = circle( nb_triangle );
			initVbao( vertices );
		}
		
		ImGui::End();
	}




	std::vector<Vec2f> LabWork2::circle(int nb_triangle) { 


		float			   pi		   = 3.14159265;
		float			   rayon	   = 0.5;
		float			   angle	   = 2 * pi / nb_triangle;
		float			   angle_start = pi * 0.5;
		Vec2f			   centre	   = Vec2f( 0.3, -0.2 );
		std::vector<Vec2f> vertices;
		vertices.push_back( centre );

		int	  current_indice = 1;
		float startx		 = centre.x + rayon * cos( angle_start );
		float starty		 = centre.y + rayon * sin( angle_start );

		vertices.push_back( Vec2f( startx, starty ) );

		for ( int i = 0; i < nb_triangle; i++ )
		{
			indice.push_back( 0 );
			indice.push_back( current_indice );
			current_indice++;
			indice.push_back( current_indice );
			angle_start += angle;
			startx = centre.x + rayon * cos( angle_start );
			starty = centre.y + rayon * sin( angle_start );
			vertices.push_back( Vec2f( startx, starty ) );
			color.push_back( getRandomVec3f() );
		} 

		return vertices;
	}



} // namespace M3D_ISICG
