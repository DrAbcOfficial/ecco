#pragma once

#include <fstream>
#include <string>

class ScopedFileWrite {
private:
	std::ofstream m_file;
	std::string m_path;
public:
	ScopedFileWrite(const std::string& path, std::ios::openmode mode = std::ios::binary) 
		: m_path(path), m_file(path, mode) {}

	~ScopedFileWrite() {
		if (m_file.is_open()) {
			m_file.close();
		}
	}

	std::ofstream& get() {
		return m_file;
	}

	bool is_open() const {
		return m_file.is_open();
	}

	bool good() const {
		return m_file.good();
	}

	const std::string& path() const {
		return m_path;
	}

	ScopedFileWrite(const ScopedFileWrite&) = delete;
	ScopedFileWrite& operator=(const ScopedFileWrite&) = delete;
};

class ScopedFileRead {
private:
	std::ifstream m_file;
	std::string m_path;
public:
	ScopedFileRead(const std::string& path, std::ios::openmode mode = std::ios::binary) 
		: m_path(path), m_file(path, mode) {}

	~ScopedFileRead() {
		if (m_file.is_open()) {
			m_file.close();
		}
	}

	std::ifstream& get() {
		return m_file;
	}

	bool is_open() const {
		return m_file.is_open();
	}

	bool good() const {
		return m_file.good();
	}

	std::streamsize gcount() const {
		return m_file.gcount();
	}

	const std::string& path() const {
		return m_path;
	}

	ScopedFileRead(const ScopedFileRead&) = delete;
	ScopedFileRead& operator=(const ScopedFileRead&) = delete;
};
