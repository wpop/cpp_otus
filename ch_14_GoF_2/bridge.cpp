// Мост (Bridge)

#include <iostream>
#include <memory>

namespace shape_example {
	struct IShape {
		virtual void show() = 0;
	};

	struct Square : IShape {
		void show() override {
			std::cout << "Square show!" << std::endl;
		}
	};

	struct Circle : Square {
		void show() override {
			std::cout << "Circle show!" << std::endl;
		}
	};

	struct Triangle : Square {
		void show() override {
			std::cout << "Triangle show!" << std::endl;
		}
	};

#pragma region(VeryBad)
	struct BlackSquare : IShape {
		void show() override {
			std::cout << "Black Square show!" << std::endl;
		}
	};

	struct BlueSquare : IShape {
		void show() override {
			std::cout << "Blue Square show!" << std::endl;
		}
	};

	struct RedSquare : IShape {
		void show() override {
			std::cout << "Red Square show!" << std::endl;
		}
	};
#pragma endregion

#pragma region(NotSoBad)
	struct IColor {
		virtual void color() = 0;
	};

	struct Black : IColor {
		void color() override {
			std::cout << "Black";
		}
	};

	struct Red : IColor {
		void color() override {
			std::cout << "Red";
		}
	};

	struct SquareWithBrige : public IShape {
		SquareWithBrige(std::unique_ptr<IColor> color)
			: m_color{std::move(color)} {
		}

		void show() override {
			m_color->color();
			std::cout << " Square show!" << std::endl;
		}

	private:
		std::unique_ptr<IColor> m_color;
	};

	void testShapes() {

		BlackSquare blackSquare;
		blackSquare.show();

		SquareWithBrige squareWithBrige(std::make_unique<Black>());
		squareWithBrige.show();
	}
} // namespace shape_example

namespace clock_example {
	class ClockSignal {
	public:
		virtual void now() = 0;
	};

	class InternetClockSignal : public ClockSignal {
		void now() override {
			std::cout << "internet clock" << std::endl;
		}
	};

	class LocalClockSignal : public ClockSignal {
		void now() override {
			std::cout << "local clock" << std::endl;
		}
	};

	class Clock {
	public:
		ClockSignal *s = nullptr;
		void refresh() {
			s->now();
			display();
		}
		virtual void display() = 0;
		virtual ~Clock() {
			delete s;
		}
	};

	class DigitalClock : public Clock {
		void display() {
			std::cout << "digital" << std::endl;
		}
	};

	class AnalogClock : public Clock {
		void display() {
			std::cout << "analog" << std::endl;
		}
	};

	void testClock() {
		auto c = std::unique_ptr<Clock>(new DigitalClock);
		c->s = new InternetClockSignal;

		c->refresh();
	}
} // namespace clock_example

int main(int, char *[]) {

	shape_example::testShapes();
	clock_example::testClock();

	return 0;
}
