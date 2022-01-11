#include "lab_work_5.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/random.hpp"
#include "utils/read_file.hpp"
#include <iostream>
using namespace std;

namespace M3D_ISICG
{
	const std::string LabWork5::_shaderFolder = "src/lab_works/lab_work_5/shaders/";

	LabWork5::~LabWork5()
	{
		_bunny.cleanGL();
		glDeleteProgram( _program );
	}

	bool LabWork5::init()
	{
		std::cout << "Initializing lab work 4..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		glEnable( GL_DEPTH_TEST );

		if ( !_initProgram() )
			return false;

		_initCamera();
		_bunny = TriangleMeshModel();
		
		_bunny.load( "_bunny", "D:\\M1__ISICG_S1\\Moteur3D\\M3D_ISICG_2021-master-TP5\\sponza.obj" );
		_bunny._transformation = glm::scale( _bunny._transformation, Vec3f( 0.01 ) );
		
		
		glUseProgram( _program );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork5::animate( const float p_deltaTime ) {}

	void LabWork5::render()
	{
		// Clear the color buffer.
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		MVPMatrix	 = _camera.getProjectionMatrix() * _camera.getViewMatrix() * _bunny._transformation;
		MVMatrix	 = _camera.getViewMatrix() * _bunny._transformation;
		NormalMatrix = glm::transpose( glm::inverse( MVMatrix ) );

		glProgramUniformMatrix4fv( _program, _uMVPMatrixLoc, 1, false, glm::value_ptr( MVPMatrix ) );
		glProgramUniformMatrix4fv( _program, _uMVMatrixLoc, 1, false, glm::value_ptr( MVMatrix ) );
		glProgramUniformMatrix4fv( _program, _uNormalMatrixLoc, 1, false, glm::value_ptr( NormalMatrix ) );

		_bunny.render( _program );

		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
	}

	void LabWork5::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				break;
			case SDL_SCANCODE_SPACE: // Print camera info
				_camera.print();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
		}
	}

	void LabWork5::displayUI()
	{
		ImGui::Begin( "Settings lab work 4" );

		// Background.
		if ( ImGui::ColorEdit3( "Background", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}

		// Camera.
		if ( ImGui::SliderFloat( "fovy", &_fovy, 10.f, 160.f, "%01.f" ) )
		{
			_camera.setFovy( _fovy );
		}
		if ( ImGui::SliderFloat( "Speed", &_cameraSpeed, 0.1f, 10.f, "%01.1f" ) ) {}

		if ( ImGui::RadioButton( "sponza", _valueDisplay == 0) ){
			if ( _valueDisplay != 0 )
			{
				_bunny.cleanGL();
				_camera.setPosition( Vec3f( 0.f, 2.f, 0.2 ) );
				_bunny.load( "_bunny", "D:\\M1__ISICG_S1\\Moteur3D\\M3D_ISICG_2021-master-TP5\\sponza.obj" );
				_bunny._transformation = glm::scale( _bunny._transformation, Vec3f( 0.01 ) );
				_valueDisplay		   = 0;
			}
		}
		if ( ImGui::RadioButton( "bunny", _valueDisplay == 1) )
		{			
			if ( _valueDisplay != 1 )
			{
				_bunny.cleanGL();
				_camera.setPosition( Vec3f( 0.f, 0.f, 0.2 ) );
				_bunny.load( "_bunny", "D:\\M1__ISICG_S1\\Moteur3D\\M3D_ISICG_2021-master-TP5\\bunny_2.obj" );
				_bunny._transformation = glm::scale( _bunny._transformation, Vec3f( 100 ) );
				_valueDisplay		   = 1;
			}
		}

		ImGui::End();
	}

	void LabWork5::resize( const int p_width, const int p_height )
	{
		BaseLabWork::resize( p_width, p_height );
		_camera.setScreenSize( p_width, p_height );
	}

	bool LabWork5::_initProgram()
	{
		// ====================================================================
		// Shaders.
		// ====================================================================
		// Create shaders.
		const GLuint vertexShader	= glCreateShader( GL_VERTEX_SHADER );
		const GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		// Get sources from files.
		const std::string vertexShaderSrc	= readFile( _shaderFolder + "mesh_texture.vert" );
		const std::string fragmentShaderSrc = readFile( _shaderFolder + "mesh_texture.frag" );

		// Convert to GLchar *
		const GLchar * vSrc = vertexShaderSrc.c_str();
		const GLchar * fSrc = fragmentShaderSrc.c_str();

		// Compile vertex shader.
		glShaderSource( vertexShader, 1, &vSrc, NULL );
		glCompileShader( vertexShader );
		// Check if compilation is ok.
		GLint compiled;
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling vertex shader: " << log << std::endl;
			return false;
		}

		// Compile vertex shader.
		glShaderSource( fragmentShader, 1, &fSrc, NULL );
		glCompileShader( fragmentShader );
		// Check if compilation is ok.
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compiled );
		if ( compiled != GL_TRUE )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fragmentShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling fragment shader: " << log << std::endl;
			return false;
		}
		// ====================================================================

		// ====================================================================
		// Program.
		// ====================================================================
		// Create program.
		_program = glCreateProgram();

		// Attach shaders.
		glAttachShader( _program, vertexShader );
		glAttachShader( _program, fragmentShader );

		// Link program.
		glLinkProgram( _program );
		// Check if link is ok.
		GLint linked;
		glGetProgramiv( _program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _program, sizeof( log ), NULL, log );
			std::cerr << "Error linking program: " << log << std::endl;
			return false;
		}

		// Shaders are now useless.
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		// ====================================================================

		// ====================================================================
		// Get uniform locations.
		// ====================================================================
		_uMVPMatrixLoc	  = glGetUniformLocation( _program, "uMVPMatrix" );
		_uMVMatrixLoc	  = glGetUniformLocation( _program, "uModelViewMatrix" );
		_uNormalMatrixLoc = glGetUniformLocation( _program, "uNormalMatrix" );
		_uPosCameraLoc	  = glGetUniformLocation( _program, "uPosCamera" );
		
		// ====================================================================

		return true;
	}

	void LabWork5::_initCamera()
	{
		_camera.setScreenSize( _windowWidth, _windowHeight );
		_camera.setPosition( Vec3f( 0.f, 2.f, 0.2 ) );
	}
} // namespace M3D_ISICG
