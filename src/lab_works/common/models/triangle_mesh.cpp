#include "triangle_mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

namespace M3D_ISICG
{
	TriangleMesh::TriangleMesh( const std::string &				  p_name,
								const std::vector<Vertex> &		  p_vertices,
								const std::vector<unsigned int> & p_indices,
								const Material &				  p_material ) :
		_name( p_name ),
		_vertices( p_vertices ), _indices( p_indices ), _material( p_material )
	{
		_vertices.shrink_to_fit();
		_indices.shrink_to_fit();
		_setupGL();
	}

	void TriangleMesh::render( const GLuint p_glProgram ) const
	{
		glProgramUniform3fv(
			p_glProgram, glGetUniformLocation( p_glProgram, "uAmbient" ), 1, glm::value_ptr( _material._ambient ) );

		glProgramUniform3fv(
			p_glProgram, glGetUniformLocation( p_glProgram, "uDiffuse" ), 1, glm::value_ptr( _material._diffuse ) );

		glProgramUniform3fv(
			p_glProgram, glGetUniformLocation( p_glProgram, "uSpecular" ), 1, glm::value_ptr( _material._specular ) );

		glProgramUniform1f( p_glProgram, glGetUniformLocation( p_glProgram, "uShininess" ), _material._shininess );

		glProgramUniform1f( p_glProgram, glGetUniformLocation( p_glProgram, "uHasDiffuseMap" ), _material._hasDiffuseMap );
		glProgramUniform1f( p_glProgram, glGetUniformLocation( p_glProgram, "uHasAmbientMap" ), _material._hasAmbientMap );
		glProgramUniform1f( p_glProgram, glGetUniformLocation( p_glProgram, "uHasSpecularMap" ), _material._hasSpecularMap );
		glProgramUniform1f(p_glProgram, glGetUniformLocation( p_glProgram, "uHasShininessMap" ), _material._hasShininessMap );
		glProgramUniform1f(p_glProgram, glGetUniformLocation( p_glProgram, "uHasNormalMap" ), _material._hasNormalMap );

		if ( _material._hasDiffuseMap )
		{
			glBindTextureUnit( 1, _material._diffuseMap._id );
		}
		if ( _material._hasAmbientMap )
		{
			glBindTextureUnit( 2, _material._ambientMap._id );
		}
		if ( _material._hasSpecularMap )
		{
			glBindTextureUnit( 3, _material._specularMap._id );
		}
		if ( _material._hasShininessMap )
		{
			glBindTextureUnit( 4, _material._shininessMap._id );
		}
		if ( _material._hasNormalMap )
		{
			glBindTextureUnit( 5, _material._normalMap._id );
		}

		glBindVertexArray( _vao );

		glDrawElements( GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0 );

		glBindTextureUnit( 1, 0 );
		glBindTextureUnit( 2, 0 );
		glBindTextureUnit( 3, 0 );
		glBindTextureUnit( 4, 0 );
		glBindTextureUnit( 5, 0 );

		glBindVertexArray( 0 );
	}

	void TriangleMesh::cleanGL()
	{
		
		glDisableVertexArrayAttrib( _vao, 0 );
		glDisableVertexArrayAttrib( _vao, 1 );
		glDisableVertexArrayAttrib( _vao, 2 );
		glDisableVertexArrayAttrib( _vao, 3 );
		glDisableVertexArrayAttrib( _vao, 4 );
		glDeleteVertexArrays( 1, &_vao );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_ebo );
	}

	void TriangleMesh::_setupGL()
	{		
		glCreateBuffers( 1, &_vbo );
		glNamedBufferData( _vbo, _vertices.size() * sizeof( Vertex ), _vertices.data(), GL_STATIC_DRAW );

		glCreateBuffers( 1, &_ebo );
		glNamedBufferData( _ebo, _indices.size() * sizeof( unsigned int ), _indices.data(), GL_STATIC_DRAW );

		// vao init

		glCreateVertexArrays( 1, &_vao );

		// layout 0 vbo position

		glEnableVertexArrayAttrib( _vao, 0 );
		glVertexArrayAttribFormat( _vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _position ) );

		// layout 1 vbo normal

		glEnableVertexArrayAttrib( _vao, 1 );

		glVertexArrayAttribFormat( _vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _normal ) );

		// layout 2 vbo _texCoords

		glEnableVertexArrayAttrib( _vao, 2 );

		glVertexArrayAttribFormat( _vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, _texCoords ) );

		// layout 3 vbo _tangente

		glEnableVertexArrayAttrib( _vao, 3 );

		glVertexArrayAttribFormat( _vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _tangent ) );

		// layout 4 vbo _bitangent

		glEnableVertexArrayAttrib( _vao, 4 );

		glVertexArrayAttribFormat( _vao, 4, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _bitangent ) );

		// link différents vbo

		glVertexArrayVertexBuffer( _vao, 0, _vbo, 0, sizeof( Vertex ) );

		// link ebo

		glVertexArrayElementBuffer( _vao, _ebo );

		// bind vao

		glVertexArrayAttribBinding( _vao, 0, 0 );
		glVertexArrayAttribBinding( _vao, 1, 0 );
		glVertexArrayAttribBinding( _vao, 2, 0 );
		glVertexArrayAttribBinding( _vao, 3, 0 );
		glVertexArrayAttribBinding( _vao, 4, 0 );
	}
} // namespace M3D_ISICG
