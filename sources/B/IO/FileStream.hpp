/*
** B, 2019
** FileStream.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class FileStream;
}

////////////////////////////////////////////////////////////////////////////////

#include "Stream.hpp"
#include "B/String.hpp"
#include "B/StringView.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class FileStream final : public Stream
{
public:
	FileStream();
	FileStream(const int fd, OpenMode mode = OpenMode::ReadWrite);
	FileStream(const StringView &filename, OpenMode mode = OpenMode::ReadWrite | OpenMode::Create);
	~FileStream() {}

	bool canRead(usize n = 1) override;
	bool canWrite(usize n = 1) override;
	// usize read(byte *, usize) override;
	// usize write(const byte *, usize) override;

private:
};

}