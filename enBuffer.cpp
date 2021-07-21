#include "enBuffer.h"
namespace En3rN::DX
{
    void enBuffer::create_buffer()
    {
        auto stride = layout_size();
        auto count = m_elements[0].count();

        m_buffer.resize(stride*count);
        for (auto i = 0; i < count; ++i)
        {
            size_t offset = 0;
            for (auto& e : m_elements)
            {
                auto v = memcpy(m_buffer.data() + i * stride + offset, 
                    e.data().data()+i*e.stride(), 
                    e.stride());                
                offset += e.stride();
            }
        }
    }

    size_t enBuffer::size()
    {
        return size_t(sizeof(m_buffer));
    }

    size_t enBuffer::count()
    {
        return size_t(m_buffer.size() / layout_size());
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
    std::vector<D3D11_INPUT_ELEMENT_DESC> enBuffer::input_element_desc()
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
        for (auto e : m_elements)
            ied.push_back(e.get_desc());

        return ied;
    }    
}
