/*
** B, 2019
** Buffer.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class Buffer;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Types.hpp"
#include "B/Containers/Container.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class Buffer : public Container<byte>
{
public:
	Buffer();
	explicit Buffer(usize size);
	Buffer(const Buffer &other);
	Buffer(Buffer &&other);
	~Buffer();

	static Buffer create(usize size);
	static Buffer adopt(byte *s, usize size);
	static Buffer wrap(const byte *s, usize size);
	static Buffer copy(const byte *s, usize size);

	byte *data() { return m_data; }
	const byte *data() const { return m_data; }
	usize size() const override { return m_size; }
	bool null() const { return data() == nullptr; }
	bool empty() const override { return null() || size() == 0; }

	void clear() override;
	void fill(byte c);
	Buffer slice(usize start = 0, usize length = -1) const;
	usize find(byte c) const;

	const byte &operator [](usize i) const { return m_data[i]; }
	byte &operator [](usize i) { return m_data[i]; }

private:
	byte *m_data = nullptr;
	usize m_size = 0;
	bool m_owner = false;
};

}