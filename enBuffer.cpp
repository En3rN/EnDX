#include "enBuffer.h"
#include "Shader.h"
#include "enMath.h"
#include "enexception.h"

namespace En3rN::DX
{
   
    void enBuffer::create_buffer()
    {
        //TODO : create buffer matching requested semantics
        auto stride = layout_size();
        auto count = m_elements[0].count();

        for (auto& e : m_elements)
            assert(count==e.count(), "element count does not match");

        m_buffer.resize(stride*count);
        for (auto i = 0; i < count; ++i)
        {
            size_t offset = 0;
            for (auto& e : m_elements)
            {
                auto v = memcpy(
                    m_buffer.data() + i * stride + offset, 
                    e.data()+i*e.stride(), 
                    e.stride()
                );                
                offset += e.stride();
            }
        }
    }

    void enBuffer::create_buffer(std::vector<std::string>& semantics)
    {
        auto stride = layout_size();
        auto count = m_elements[0].count();

        for (auto& e : m_elements)
            assert(count == e.count(), "element count does not match");

        m_buffer.resize(stride * count);
        for (auto i = 0; i < count; ++i)
        {
            size_t offset = 0;
            for (auto& e : m_elements)
            {
                if (std::find(begin(semantics), end(semantics), e.semantic()) != end(semantics)) {
                    auto v = memcpy(
                        m_buffer.data() + i * stride + offset,
                        e.data() + i * e.stride(),
                        e.stride()
                    );
                    offset += e.stride();
                }
                else
                {
                    std::string err{ "Semantic:" + e.semantic() + " Not found in buffer" };
                    throw EnExcept(err, std::source_location::current());
                }
            }
        }
    };

    size_t enBuffer::size()
    {
        return size_t(sizeof(m_buffer));
    }

    size_t enBuffer::count()
    {
        return m_elements.at(0).count();
    }

    size_t enBuffer::element_count()
    {
        return size_t(m_elements.size());
    }

    size_t enBuffer::layout_size()
    {
        size_t size(0);
        for (auto& e : m_elements)
            size += e.stride();
        return size;
    }
    uint8_t* enBuffer::data()
    {
        if (m_buffer.empty())
            create_buffer();
         return m_buffer.data();
    }
    enBuffer::Indecies enBuffer::indecies(uint32_t numFaces)
    {
        Indecies indecies{};
        auto verteciesPerFace = count() / numFaces;
        uint64_t triangles = 1u;
        while ((verteciesPerFace % (triangles*3) != 0) && ( verteciesPerFace % (triangles * 3)!=4))
            ++triangles;
        uint64_t indeciesPerFace = triangles * 3;
        uint32_t index = 0u;
        for (auto face = 0u; face < numFaces; ++face)
        {
            uint32_t facefirst = face * verteciesPerFace;
            uint32_t facelast = facefirst + verteciesPerFace - 1; // -1 korrigere for at vi starter på 0
            for (auto triangle = 0u; triangle < triangles; ++triangle)
            {
                auto last = index + 3;
                for (index; index < last; ++index)
                {
                    indecies.push_back(Wrap(index,facefirst,facelast));
                }
                verteciesPerFace < indeciesPerFace ? --index : index;
            }
        }
        return indecies;
    }
}
