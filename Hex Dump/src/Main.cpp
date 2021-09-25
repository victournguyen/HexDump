#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

using namespace std;

int main() {
	ifstream reader("C:\\Users\\Root\\source\\repos\\Hex Dump\\Hex Dump\\src\\ascii.txt", ifstream::in);
	string s_offset;
	int offset = 0;
	cout << "Offset: ";
	cin >> s_offset;
	if (s_offset.rfind("0x", 0) == 0) {
		try {
			offset = stoi(s_offset.substr(2, s_offset.length() - 2), nullptr, 16);
		}
		catch (exception e) {
			printf("Offset is not valid!\n");
			return 1;
		}
	}
	else {
		try {
			offset = stoi(s_offset);
		}
		catch (exception e) {
			printf("Offset is not valid!\n");
			return 1;
		}
	}
	//cin >> offset;
	if (reader) {
		reader.seekg(0, reader.end);
		int length = reader.tellg();
		//cout << length << '\n';
		length -= offset;
		if (length < 0) {
			cout << "Your offset is past the end of the file!\n";
			return 1;
		}
		else if (length == 0) {
			cout << "There is nothing to read.\n";
			return 0;
		}
		reader.seekg(offset);

		char* buffer = new char[16];
		int line_number = offset / 16, start_line_number = line_number;
		while (length > 0) {
			int num_read;
			printf("0x%08x  ", line_number);
			if (line_number == start_line_number)
				for (int i = 0; i < offset % 16; i++)
					printf("   ");
			if (length >= 16) {
				num_read = line_number == start_line_number ? 16 - (offset % 16) : 16;
				reader.read(buffer, num_read);
				for (int i = 0; i < num_read; i++) {
					printf("%02x", (unsigned char)buffer[i]);
					if (i < num_read - 1)
						printf(" ");
				}
				printf(" %c ", '\u007c');
				if (line_number == start_line_number)
					for (int i = 0; i < offset % 16; i++)
						printf(".");
				for (int i = 0; i < num_read; i++) {
					if (buffer[i] >= 32 && buffer[i] <= 126/*isprint(buffer[i])*/)
						printf("%c", (unsigned char)buffer[i]);
					else
						printf(".");
				}
				//reader.seekg(16, reader.cur);
			}
			else {
				num_read = length;
				reader.read(buffer, num_read);
				for (int i = 0; i < num_read; i++)
					printf("%02x ", (unsigned char)buffer[i]);
				for (int i = 0; i < (line_number == start_line_number ? 16 - offset % 16 - num_read : 16 - num_read); i++)
					printf("   ");
				printf("%c ", '\u007c');
				if (line_number == start_line_number)
					for (int i = 0; i < offset % 16; i++)
						printf(".");
				for (int i = 0; i < num_read; i++) {
					if (buffer[i] >= 32 && buffer[i] <= 126/*isprint(buffer[i])*/)
						printf("%c", (unsigned char)buffer[i]);
					else
						printf(".");
				}
				for (int i = 0; i < (line_number == start_line_number ? 16 - offset % 16 - num_read : 16 - num_read); i++)
					printf(".");
			}
			printf("\n");
			length -= num_read;
			line_number++;
		}
		delete[] buffer;
		reader.close();
	}
	else
		printf("Failed to open file.\n");
}
