#include "pch.h"

bool file_exist(const std::string& path)
{
	std::ifstream f(path);
	return f.good();
}

std::pair<std::string, bool> OpenFileDialog()
{
	OPENFILENAME openFileName;
	char file_name[MAX_PATH] = "";
	memset(&openFileName, 0, sizeof(openFileName));

	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.hwndOwner = NULL;
	openFileName.lpstrFilter = NULL;
	openFileName.lpstrFile = file_name;
	openFileName.nMaxFile = MAX_PATH;
	openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	openFileName.lpstrDefExt = "";

	std::string fileStr = std::string();

	if (GetOpenFileNameA(&openFileName))
		fileStr = file_name;

	if (fileStr.empty())
		return std::make_pair(fileStr, false);

	return std::make_pair(fileStr, true);
}

int main(int argc, char* argv[])
{
	std::string filePath = std::string();
	if (argc >= 2)
	{
		filePath = argv[1];
	}
	else
	{
		auto fileDialog = OpenFileDialog();
		if (!fileDialog.second)
		{
			std::cout << "ERROR: file did not get select by file dialog." << std::endl;
			std::system("pause");
			return -1;
		}

		filePath = fileDialog.first;
	}

	if (!file_exist(filePath))
	{
		std::cout << "ERROR: file " << filePath << " does not exist." << std::endl;
		std::system("pause");
		return -1;
	}

	HMODULE mappedFile = LoadLibraryA(filePath.c_str()); // Yes you can load .exe's with load library, we do this so it maps the .exe in it's state when it's ran. Entrypoint won't be called.
	if (mappedFile == 0)
		return -1;

	std::fstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);

	file.seekg(0, std::ios::end);

	// Get filesize.
	auto size = file.tellg();

	file.close();

	// Pattern taken from godot-3.5.1-stable.
	auto keyPtr = CMemory(mappedFile).FindPatternSelf("48 8D 05 ? ? ? ? 0F B6 0C 03", CMemory::Direction::DOWN, size).ResolveRelativeAddressSelf(0x3, 0x7).RCast<std::uint8_t*>();

	for (int i = 0; i < 32; i++)
	{
		std::cout << std::uppercase << std::hex << static_cast<int>(keyPtr[i]);
	}

	std::cout << std::endl;

	FreeLibrary(mappedFile);

	return 0;
}