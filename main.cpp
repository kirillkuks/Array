#include <iostream>
#include <vector>

struct TypeBase {
public:
	virtual ~TypeBase() = 0 {}
};

template <typename T>
struct Type : public TypeBase {
public:
	Type(T const& value_) : value{ value_ } {}

	T& get_value() {
		return value;
	}

	~Type() override {}
private:
	T value;
};

struct VarArray {
public:
	VarArray() : length{ 0 }, cur_size{ 0 } {
		data = nullptr;
	}
	VarArray(size_t size) : length{ size } {
		data = new std::shared_ptr<TypeBase>[length];
		for (size_t i = 0; i < length; ++i) {
			data[i] = nullptr;
		}
		cur_size = length;
	}
	VarArray(VarArray const& another) : length{ another.length } {
		data = new std::shared_ptr<TypeBase>[length];
		for (size_t i = 0; i < length; ++i) {
			data[i] = another.data[i];
		}
		cur_size = length;
	}

	template <typename T>
	bool push_back(T&& elem) {
		std::shared_ptr<TypeBase> el(new Type<T>(elem));
		if (length >= cur_size) {
			std::shared_ptr<TypeBase>* new_data = new std::shared_ptr<TypeBase>[length + 1];
			if (new_data == nullptr) {
				return false;
			}

			for (size_t i = 0; i < length; ++i) {
				new_data[i] = data[i];
			}
			new_data[length++] = el;
			++cur_size;

			delete[] data;
			data = new_data;
		}
		else {
			data[length++] = el;
		}
		return true;
	}

	void pop_back() {
		if (length > 0) {
			--length;
		}
	}

	template <typename T>
	bool set_value(size_t index, T&& elem) {
		if (index >= 0 && index < length) {
			std::shared_ptr<TypeBase> el(new Type<T>(elem));
			data[index] = el;
			return true;
		}
		return false;
	}

	template <typename T>
	bool at(size_t index, T* value) {
		if (index >= 0 && index < length) {
			Type<T>* elem = dynamic_cast<Type<T>*>(data[index].get());

			if (elem != nullptr) {
				*value = elem->get_value();
				return true;
			}
		}
		return false;
	}

	VarArray& operator=(VarArray another) {
		swap(*this, another);
		return *this;
	}

	size_t size() {
		return length;
	}

	~VarArray() {
		delete[] data;
	}

private:
	void swap(VarArray& left, VarArray& right) {
		std::swap(left.length, right.length);
		std::swap(left.data, right.data);
	}

private:
	std::shared_ptr<TypeBase>* data;
	size_t length;
	size_t cur_size;
};

int main() {
	VarArray v_arr;
	v_arr.push_back(5);
	v_arr.push_back(std::string("qwerty"));
	v_arr.push_back(3.14);

	int j;
	v_arr.at(0, &j);
	std::string str;
	v_arr.at(1, &str);
	double d;
	v_arr.at(2, &d);

	std::cout << j << ' ' << str << ' ' << d << std::endl;

	VarArray v_arr1;
	v_arr1.push_back(6);
	v_arr1.push_back(4.12);
	v_arr1.push_back(std::pair<std::string, int>("asd", 4));
	v_arr1.push_back(true);
	v_arr1.pop_back();
	v_arr1.pop_back();
	v_arr1.push_back(std::pair<std::string, int>("qwe", 5));
	v_arr1.push_back(true);
	v_arr1.push_back("123");

	v_arr = v_arr1;

	int a;
	v_arr.at(0, &a);
	std::cout << a << std::endl;

	std::pair<std::string, int> p;
	v_arr.at(2, &p);

	std::cout << p.first << " : " << p.second << std::endl;

	v_arr.set_value(2, std::string("str"));

	std::pair<std::string, int> p1;

	v_arr1.at(2, &p1);
	std::string str1;
	v_arr.at(2, &str1);
	std::cout << str1 << std::endl;
	std::cout << p1.first << " : " << p1.second << std::endl;

	return 0;
}