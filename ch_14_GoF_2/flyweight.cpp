// Приспособленец (Flyweight)

#include <iostream>
#include <map>

class Image {
public:
	void resize(int, int){};

	void crop(int, int){};

	void show(){};
};

class ImageResizer {
	std::map<int, Image> images;

public:
	Image get_box(int w) {
		auto i = images.find(w);
		if (i == images.end()) {
			Image img;
			img.resize(w, w);

			bool b;
			std::tie(i, b) = images.emplace(w, img);
		}
		return i->second;
	}
};

void testImageResizer() {
	ImageResizer rs;

	auto image_100 = rs.get_box(100);
	image_100.show();

	auto image_640 = rs.get_box(640);
	image_640.show();

	auto image_1000 = rs.get_box(1000);
	image_1000.show();
}

class ImageView {
public:
	ImageView(int width, int height, const Image &image)
		: m_image{image}, m_height{height}, m_width{width} {
	}

	void show() const {
		// show only m_width x m_height rect.
	}

private:
	const Image &m_image;
	const int m_width;
	const int m_height;
};

void testImageView() {
	Image veryBigImage;

	auto view1 = ImageView{10, 40, veryBigImage};
	view1.show();
	auto view2 = ImageView{1024, 780, veryBigImage};
	view2.show();
}

int main(int, char *[]) {
	testImageResizer();
	testImageView();
}
