#pragma once
#include <vector>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace FanshaweGameEngine
{

	namespace Network
	{
		class Buffer
		{
		public:
			enum class Endianness
			{
				LITTLE_ENDIAN,
				BIG_ENDIAN
			};


		public:
			Buffer(size_t size = 16);
			~Buffer();

			void WriteUInt32(const size_t index, const uint32_t value);
			void WriteUInt32(const uint32_t value);
			void WriteString(const std::string msg);
			void WriteString(const size_t index, const std::string msg);

			uint32_t ReadUInt32(const size_t index);
			uint32_t ReadUInt32();

			void SetEndian(Endianness endian);
			bool IsBigEndian();

			void ClearBuffer();
			void ResizeBuffer(const uint32_t size);
			
			char* Get()
			{ 
				// getting the adress of teh first eleemnt of the vector buffer
				return (char*)(&m_buffer[0]); 
			}


		private:

			std::vector<char> m_buffer;
			uint32_t m_readIndex = 0;
			uint32_t m_writeIndex = 0;

			// Default Big Endian
			Endianness currentEndianness = Endianness::BIG_ENDIAN;

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			// Grey color
			const int Grey = FOREGROUND_INTENSITY;
			const int White = FOREGROUND_INTENSITY;

		};
	}
}
