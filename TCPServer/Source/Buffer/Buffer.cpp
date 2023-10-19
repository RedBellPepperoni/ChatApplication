#include "Buffer.h"
#include <iostream>

namespace FanshaweGameEngine
{

	namespace Network
	{
		Buffer::Buffer(size_t size)
		{
			m_buffer.resize(size, 0);
		}
		Buffer::~Buffer()
		{
			m_buffer.clear();
		}

		void Buffer::WriteUInt32(const size_t index, const uint32_t value)
		{

			ResizeBuffer(index + sizeof(value) + value);

			

			switch (currentEndianness)
			{
			case FanshaweGameEngine::Network::Buffer::Endianness::LITTLE_ENDIAN:

				// Encoding with Little Endian
				m_buffer[index] = value;
				m_buffer[index + 1] = value >> 8;
				m_buffer[index + 2] = value >> 16;
				m_buffer[index + 3] = value >> 24;


				break;
			case FanshaweGameEngine::Network::Buffer::Endianness::BIG_ENDIAN:

				// Encoding with Little Endian
				m_buffer[index] = value >> 24;
				m_buffer[index + 1] = value >> 16;
				m_buffer[index + 2] = value >> 8;
				m_buffer[index + 3] = value;

				break;
			default:
				break;
			}

			


		}
		void Buffer::WriteUInt32(const uint32_t value)
		{

			WriteUInt32(m_writeIndex, value);
			m_writeIndex +=4;

		}

		void Buffer::WriteString(const std::string msg)
		{

			for (int i = 0; i < msg.length(); i++)
			{
				m_buffer[m_writeIndex] = msg[i];
				m_writeIndex++;
			}
		}
		

		uint32_t Buffer::ReadUInt32(const size_t index)
		{
			
			if (currentEndianness == Endianness::LITTLE_ENDIAN)
			{
				return (m_buffer[index + 3] << 24 | m_buffer[index + 2] << 16 | m_buffer[index + 1] << 8 | m_buffer[index]);
			}

			return (m_buffer[index + 3]  | m_buffer[index + 2] << 8 | m_buffer[index + 1] << 16 | m_buffer[index] << 24);
		}
		uint32_t Buffer::ReadUInt32()
		{
			return ReadUInt32(m_readIndex);
		}
		void Buffer::SetEndian(Endianness endian)
		{
			currentEndianness = endian;
		}
		bool Buffer::IsBigEndian()
		{
			return currentEndianness == Endianness::BIG_ENDIAN;
		}

		void Buffer::ClearBuffer()
		{
			// Setting each element of the memeory set to 0
			// Essentially clearing the buffer without resizing 
			m_writeIndex = 0;
			m_readIndex = 0;
			std::fill(m_buffer.begin(), m_buffer.end(), 0);
		}


		void Buffer::ResizeBuffer(const uint32_t size)
		{
			if (size > m_buffer.size())
			{// Growing the buffer




				SetConsoleTextAttribute(hConsole, debugAttrib);
				std::cout << "                [ Growing the Buffer ]          " << std::endl;

				m_buffer.resize(size);
				
			}
		}
	}
}