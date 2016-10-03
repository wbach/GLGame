#include "ShadowFrameBuffer.h"

void CShadowFrameBuffer::Init(glm::vec2 size, glm::vec2 window_size)
{
	m_Size = size;
	m_WindowSize = window_size;
	InitialiseFrameBuffer();
}

void CShadowFrameBuffer::CleanUp()
{
	glDeleteFramebuffers(1, &m_Fbo);
	glDeleteTextures(1, &m_ShadowMap);
}

void CShadowFrameBuffer::BindFBO()
{
	Utils::BindFrameBuffer(m_Fbo, m_Size.x, m_Size.y);
}

void CShadowFrameBuffer::UnbindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);
}

void CShadowFrameBuffer::InitialiseFrameBuffer()
{
	m_Fbo = Utils::CreateFrameBuffer();
	m_ShadowMap = Utils::CreateDepthBufferAttachment(m_Size.x, m_Size.y);
	UnbindFrameBuffer();
}

const GLuint & CShadowFrameBuffer::GetShadowMap() const
{
	return m_ShadowMap;
}
