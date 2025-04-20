#include "AgniException.h"
#include <sstream>
AgniException::AgniException(int line, const char* file)
	:
	line(line),
	file(file)
{

}

const char* AgniException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl;
	oss << GetOriginString() << std::endl;
	whatBuffer = oss.str();
	return whatBuffer.c_str();

}

const char* AgniException::GetType() const noexcept
{
	return "Agni Exception";
}

int AgniException::GetLine() const noexcept
{
	return line;
}

const std::string& AgniException::GetFile() const noexcept
{
	return file;
}

std::string AgniException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}

