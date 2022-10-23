#pragma once
#include "graphics/GraphicFramework.hpp"
namespace cuvel
{
	class OpenGLFramework final : public GraphicFramework
	{
	public:
		OpenGLFramework();
		~OpenGLFramework();

		void update() override;
		void event() override;
		void render() override;
	};
}

