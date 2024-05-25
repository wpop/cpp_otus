// Декоратор (Decorator)

#include <iostream>

class File {
public:
	virtual void write() = 0;
};

class RawFile : public File {
	void write() override {
		std::cout << "write" << std::endl;
	}
};

class ZipFile : public File {
	File *file;

	void write() override {
		std::cout << "compress" << std::endl;
		file->write();
	}

public:
	ZipFile(File *file_) : file(file_){};
};

int main(int, char *[]) {
	File *c = new RawFile;
	c->write();

	File *t = new ZipFile(new RawFile);
	t->write();

	delete t;
	delete c;
}
