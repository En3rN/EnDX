#pragma once
#include <filesystem>
namespace En3rN::DX
{
	namespace fs = std::filesystem;
	struct Folders
	{
		fs::path MakePath() {};
		fs::path assets = "assets";
		fs::path textures = "textures";
		fs::path modelPath = assets;
		fs::path shaderFolder = "Shaders";
	};
	class Config
	{
	public:
		static Folders& GetFolders() { return Get().folders; }
		
		~Config() = default;
	private:
		Config() = default;
		static Config& Get()
		{
			static Config manager;
			return manager;
		};
		Folders folders;
	};
	

	
}

