#pragma once
namespace En3rN::DX
{
	class iUIControls
	{
	public:
		virtual ~iUIControls() = default;

		void Open() { m_open = true; }

		virtual bool UIControls() = 0;
	protected:
		bool m_open = false;
	};
}

