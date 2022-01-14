#pragma once
#ifndef __LAB_WORK_2_HPP__
#define __LAB_WORK_2_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	  public:
		LabWork2() : BaseLabWork() {}
		~LabWork2();

		GLuint vbo;
		GLuint vbo_color;
		GLuint vao;
		GLuint ebo;
		GLuint program;
		GLint  locTrans;
		GLint			  locLum;
		int				  nb_triangle;
		float  luminosite;
		float  _time;
		bool   _translation;
		std::vector<int> indice;
		std::vector<Vec3f>color;

		bool init() override;
		void initVbao( std::vector<Vec2f> vertices );
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

		std::vector<Vec2f> circle(int nb_triangle);

	  private:
		// ================ Scene data.
		// ================

		// ================ GL data.
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
