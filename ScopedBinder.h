#pragma once
#include <stdint.h>
namespace En3rN::DX {
	template<class T, size_t N>
	class ScopedBinder
	{
	public:
		ScopedBinder( T arr ) :
			m_arr( arr )
		{
		}
		T & GetBindArr() { return m_arr; }
		T   GetUnbindArr() {
			T unbindArr[ N ];
			for( auto i = 0; i < N; ++i ) {
				unbindArr[ i ] = nullptr;
			}
			return unbindArr;
		}
		const size_t size = N;
	private:
		T m_arr[N];
	};
}

