/*
** B, 2019
** Stream.cpp
*/

#include "Stream.hpp"
#include "FileStream.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

extern Stream stdIn  = FileStream(0, OpenMode::ReadOnly);
extern Stream stdOut = FileStream(1, OpenMode::WriteOnly);
extern Stream stdErr = FileStream(2, OpenMode::WriteOnly);

Stream Stream::Null   = FileStream("/dev/null", OpenMode::ReadWrite);
Stream Stream::Random = FileStream("/dev/urandom", OpenMode::ReadOnly);

////////////////////////////////////////////////////////////////////////////////

bool Stream::open(OpenMode mode)
{
	m_mode = mode;
	return true;
}

void Stream::ignore(int delim, usize limit)
{
	byte c;
	for (size_t i = 0; i < limit; ++i) {
		*this >> c;
		if (c == delim)
			return;
	}
}

bool Stream::canRead(usize /*n*/)
{
	return this->isOpen() && this->mode() & OpenMode::ReadOnly;
}

bool Stream::canWrite(usize /*n*/)
{
	return this->isOpen() && this->mode() & OpenMode::WriteOnly;
}

////////////////////////////////////////////////////////////////////////////////

Stream &Stream::operator >>(bool &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseBoolean();
	}
	return *this;
}

Stream &Stream::operator <<(bool value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else {
			if (value)
				this->write((const byte*)"true", 4);
			else
				this->write((const byte*)"false", 5);
		}
	}
	return *this;
}


Stream &Stream::operator >>(i8 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseSigned();
	}
	return *this;
}

Stream &Stream::operator <<(i8 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printSigned(value);
	}
	return *this;
}


Stream &Stream::operator >>(u8 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseUnsigned();
	}
	return *this;
}

Stream &Stream::operator <<(u8 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printUnsigned(value);
	}
	return *this;
}


Stream &Stream::operator >>(i16 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		value = 0;
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseSigned();
	}
	return *this;
}

Stream &Stream::operator <<(i16 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printSigned(value);
	}
	return *this;
}


Stream &Stream::operator >>(u16 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseSigned();
	}
	return *this;
}

Stream &Stream::operator <<(u16 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printUnsigned(value);
	}
	return *this;
}


Stream &Stream::operator >>(i32 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		value = 0;
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseSigned();
	}
	return *this;
}

Stream &Stream::operator <<(i32 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printSigned(value);
	}
	return *this;
}


Stream &Stream::operator >>(u32 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseSigned();
	}
	return *this;
}

Stream &Stream::operator <<(u32 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printUnsigned(value);
	}
	return *this;
}


Stream &Stream::operator >>(i64 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		value = 0;
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseSigned();
	}
	return *this;
}

Stream &Stream::operator <<(i64 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printSigned(value);
	}
	return *this;
}


Stream &Stream::operator >>(u64 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseSigned();
	}
	return *this;
}

Stream &Stream::operator <<(u64 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printUnsigned(value);
	}
	return *this;
}


Stream &Stream::operator >>(f32 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseDecimal();
	}
	return *this;
}

Stream &Stream::operator <<(f32 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printDecimal(value);
	}
	return *this;
}


Stream &Stream::operator >>(f64 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseDecimal();
	}
	return *this;
}

Stream &Stream::operator <<(f64 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printDecimal(value);
	}
	return *this;
}


Stream &Stream::operator >>(f128 &value)
{
	if (!this->canRead(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->read((byte*)&value, sizeof(value));
		else
			value = parseDecimal();
	}
	return *this;
}

Stream &Stream::operator <<(f128 value)
{
	if (!this->canWrite(sizeof(value)))
		m_state |= State::Fail;
	else {
		if (this->mode() & OpenMode::Binary)
			this->write((const byte*)&value, sizeof(value));
		else
			printDecimal(value);
	}
	return *this;
}


Stream &Stream::operator >>(String &str)
{
	if (this->eof()) {
		m_state |= State::Fail;
		return *this;
	}

	str.clear();

	// Skip any leading whitespace
	for (; !this->eof() && isspace(this->peek()); this->readOne());

	// Read word
	while (this->peek() != EOF && !isspace(this->peek())) {
		str.append(this->readOne());
	}

	// Skip trailing whitespaces
	for (; !this->eof() && isspace(this->peek()); this->readOne());

	return *this;
}

Stream &Stream::operator <<(const String &str)
{
	if (!this->canWrite(str.length()))
		m_state |= State::Fail;
	else
		this->write((const byte*)str.cStr(), str.length());

	return *this;
}

Stream &Stream::operator <<(const char *str)
{
	usize n = strlen(str);
	if (!this->canWrite(n))
		m_state |= State::Fail;
	else
		this->write((const byte*)str, n);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

bool Stream::parseBoolean()
{
}

i64 Stream::parseSigned()
{
}

u64 Stream::parseUnsigned()
{
}

f128 Stream::parseDecimal()
{
}

void Stream::printSigned(i64 value)
{
}

void Stream::printUnsigned(u64 value)
{
}

void Stream::printDecimal(f128 value)
{
}

////////////////////////////////////////////////////////////////////////////////

}