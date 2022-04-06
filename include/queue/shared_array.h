#pragma once
#include <memory>
#include <assert.h>
#include "common/s4exceptions.h"

#include <stdlib.h>

namespace S4
{

//用于在智能指针释放时删除new对象
template<typename TYPE>
struct array_deleter {
	void operator ()(TYPE * p)
	{
#ifdef _WIN32
		_aligned_free(p);
#else
		//delete[] p;
		free(p);
#endif
	}
};

//用智能指针管理new出来的指针
template<typename T>
class shared_array
{
public:
	shared_array() noexcept:
		_sp(nullptr),
		_size(0)
	{
	}

	//new出一块新内存，并用智能指针管理，4K对齐
	explicit shared_array(size_t len):
		// _sp(new T[len], array_deleter<T>()),
		_size(len)
	{
		T* p;
		int ret = posix_memalign((void **)&p, 4096, len * sizeof(T));
		if (ret != 0) {
			throw std::runtime_error("Error allocating aligned memory!");
		}
		_sp.reset(p, array_deleter<T>());
	}

	//拷贝构造，智能指针增加引用
	// shared_array(const shared_array& cp) noexcept:
	// 	_sp(cp.sp()),
	// 	_size(cp.size())
	// {
	// }

	//原始指针
	T* get() noexcept{
		return _sp.get();
	};

	const T* get() const noexcept{
		return _sp.get();
	}

	// //
	// std::shared_ptr<T> sp() noexcept{
	// 	return _sp;
	// };
	// //
	// std::shared_ptr<T> sp() const noexcept{
	// 	return _sp;
	// };

	T& operator[](size_t n) noexcept{
		assert(n < _size);
		return _sp.get()[n];
	}

	const T& operator[](size_t n) const noexcept{
		return _sp.get()[n];
	}

	size_t size() const noexcept{
		return _size;
	};

	// shared_array& operator=(const shared_array& b) noexcept{
	// 	this->_size = b.size();
	// 	this->_sp = b.sp();
	// 	return *this;
	// }
private:
	std::shared_ptr<T> _sp;
	size_t _size;

private:
#ifdef _WIN32
	int check_align(size_t align)
	{
		for (size_t i = sizeof(void *); i != 0; i *= 2)
		if (align == i)
			return 0;
		return EINVAL;
	}
	
	int posix_memalign(void **ptr, size_t align, size_t size)
	{
		if (check_align(align))
			return EINVAL;
	
		int saved_errno = errno;
		void *p = _aligned_malloc(size, align);
		if (p == NULL)
		{
			errno = saved_errno;
			return ENOMEM;
		}
	
		*ptr = p;
		return 0;
	}
#endif
};


typedef shared_array<char> char_array_t;

// byte_buffer_t make_byte_buffer(const char * pBuff, unsigned int len);

// struct shared_byte_buffer_t
// {
// 	byte_buffer_t buffer;
// 	size_t curLen;

// 	shared_byte_buffer_t(size_t buffer_size):
// 		buffer(buffer_size),
// 		curLen(0)
// 	{
// 	}

// 	char* get() {
// 		return buffer.sp().get();
// 	}

// 	char& operator[](size_t n) {
// 		assert(n < buffer.size());
// 		return buffer[n];
// 	}

// };

// int to_byte_buffer(char * pBuff, unsigned int len, shared_byte_buffer_t& byte_buffer);


//用于在智能指针释放时删除new对象
template<typename TYPE>
struct class_array_deleter {
	void operator ()(TYPE * p)
	{
		delete[] p;
	}
};

//T0:基类
//T1:派生类
template<typename T0>
class shared_class_array
{
public:
	shared_class_array() noexcept:
		_sp(nullptr),
		_entry_nb(0),
		_entry_size(0)
	{
	}

	//new出一块新内存用于装载派生类，并用智能指针管理
    template<typename T1>
	bool init(size_t len)
		// _sp(new T[len], array_deleter<T>()),
	{
		assert(!_entry_size && !_entry_nb);
		_entry_nb = len;
		_entry_size = sizeof(T1);
		T1* p = new T1[len];
		if (!p) {
			return false;
		}
		_sp.reset(p, class_array_deleter<T1>());
		return true;
	}

	//拷贝构造，智能指针增加引用
	// shared_array(const shared_array& cp) noexcept:
	// 	_sp(cp.sp()),
	// 	_size(cp.size())
	// {
	// }

	size_t size() const noexcept{
		return _entry_nb;
	};

    const T0 * get_base(size_t n) const noexcept{
		assert(_entry_size && _entry_nb);
		assert(n < _entry_nb);
		return (const T0*)((char*)_sp.get() + n * _entry_size);
	}

	T0* get_base(size_t n) {
		assert(_entry_size && _entry_nb);
		assert(n < _entry_nb);
		return (T0*)((char*)_sp.get() + n * _entry_size);
	}
	// shared_array& operator=(const shared_array& b) noexcept{
	// 	this->_entry_nb = b.size();
	// 	this->_sp = b.sp();
	// 	return *this;
	// }
private:
	std::shared_ptr<T0> _sp;
	size_t _entry_nb;
	size_t _entry_size;

};


}



