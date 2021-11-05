#pragma once

#include "Job.h"
#include <string>
#include <memory>
#include <vector>

namespace En3rN::DX
{
	class Pass
	{
	public:
		//using handle = std::unique_ptr<Pass>;
		using Container = std::vector<Pass>;
		enum class Name
		{
			Lambertian, Skybox
		};
		Pass(Name name) : m_name(name) {};
		/*Pass(Pass&& other) noexcept = default;
		Pass(Pass& other)  = default;
		Pass& operator = (Pass&& other) noexcept = default;
		Pass& operator = (Pass & other)  = default;
		~Pass() = default;*/
		void AddJob(const Job& job);
		std::string GetPassName();
		Job::Container& GetJobs() { return m_jobs; }
		bool operator < (Pass& other) { return m_name < other.m_name; }		
	protected:
		Name						m_name;
		Job::Container				m_jobs;
	};
}
