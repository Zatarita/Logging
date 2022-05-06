#include "core/wrappers/w_fileout.h"

namespace Log
{
	namespace Wrapper
	{
		FileOut::FileOut(const std::filesystem::path& path)
		{
			open(path);
		}

		FileOut::~FileOut()
		{
			fileout.close();
		}

		void FileOut::open(const std::filesystem::path& path) noexcept
		{
			if (fileout.is_open())
				fileout.close();

			// If the file has a path besides the root folder
			if (path.has_parent_path())
			{
				// If the directory heiarchy doesn't exist
				if (!std::filesystem::exists(path.parent_path()))
				{
					// Try to create the directory heiarchy, open the file if ok
					if (std::filesystem::create_directories(path.parent_path()))
						fileout.open(path);
					else // On failure use the default fileout path.
					{
						fileout.open(DEFAULT_PATH);

						// We'll manually add a log value in here so user can debug file path issues.
						fileout << "Logout Filepath Invalid: " + path.string() + "\n";
					}
				}
			}
			else // If the file is in the root directory nothing fancy is needed.
				fileout.open(path);
		}

		void FileOut::write(std::string_view output) noexcept
		{
			if (!fileout.is_open())
				return;
			fileout << output;
		}

	}
}
