// Компоновщик (Composite)

#include <iostream>
#include <vector>

class FileSystemObject {
public:
	virtual int size() = 0;
	virtual void add_object(FileSystemObject *){};
	virtual ~FileSystemObject() = 0 {};
};

class File : public FileSystemObject {
public:
	int size() override {
		return 1024;
	}
};

class Directory : public FileSystemObject {
public:
	~Directory() {
		for (auto p : m_entries)
			delete p;
	}

	int size() override {
		int total = 0;
		for (auto fo : m_entries) {
			total += fo->size();
		}
		return total;
	}

	void add_object(FileSystemObject *fso) override {
		m_entries.push_back(fso);
	}

private:
	std::vector<FileSystemObject *> m_entries;
};

Directory *subdir() {
	Directory *d = new Directory;
	d->add_object(new File);
	d->add_object(new File);
	return d;
}

int main(int, char *[]) {
	Directory *root = new Directory;
	root->add_object(subdir());

	std::cout << root->size() << std::endl;

	delete root;

	return 0;
}
