#include "BBElementBufferObject.h"
#include "BBUtils.h"
#include <QOpenGLBuffer>


BBElementBufferObject::BBElementBufferObject()
    : BBBufferObject()
{

}

void BBElementBufferObject::setSize(int nIndexCount, GLenum hint)
{
    m_Name = createBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * nIndexCount, hint, nullptr);
}

void BBElementBufferObject::draw(GLenum eDrawPrimitiveType, int nIndexCount, int nDrawStartIndex)
{
    glDrawElements(eDrawPrimitiveType, nIndexCount, GL_UNSIGNED_SHORT, (void*) (sizeof(unsigned short) * nDrawStartIndex));
}
