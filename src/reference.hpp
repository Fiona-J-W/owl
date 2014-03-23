#ifndef REFERENCE_HPP
#define REFERENCE_HPP

#include <cassert>
#include <cstddef>
#include <type_traits>


template<typename T>
class reference {
public:
	static_assert(std::is_same<T, typename std::remove_reference<T>::type>(),
			"T must not be a reference-type");
	
	// there is no reasonable default-value, so:
	reference() = delete;
	
	// these are all just the defaults but it is nice to see them explicitly
	reference(const reference&) = default;
	reference(reference&&) = default;
	reference& operator=(const reference&) = default;
	reference& operator=(reference&&) = default;
	
	//Constructing from pointers is as fine as from references; we can however
	//not allow const references, since the saved value may be modified through
	//this class:
	reference(T& pointee) : ptr{&pointee} {}
	
	// there is no point in having a reference to a temporary object:
	reference(T&&) = delete;
	
	T& operator*() {return *ptr;}
	const T& operator*() const {return *ptr;}
	
	T* operator->() {return ptr;}
	const T* operator->() const {return ptr;}
	
	T& get() {return *ptr;}
	const T& get() const {return *ptr;}
	
	T* get_ptr() {return ptr;}
	const T* get_ptr() const {return ptr;}
	
	template <typename Derived>
	reference(const reference<Derived> other) : ptr{ other.get_ptr() } {
		static_assert(std::is_base_of<T, Derived>::value,
		"Reference<T> can only be constructed from Reference<U> if "
		"T is either equal to U or a base-class of U");
	}
	
	// we need this one for using this template in std::set and the like:
	bool friend operator<(const reference& l, const reference& r) {
		return l.ptr < r.ptr;
	}
	
	// comparing for equality is always usefull:
	bool friend operator==(const reference& l, const reference& r) {
		return l.ptr == r.ptr;
	}
	bool friend operator!=(const reference& l, const reference& r) {
		return l.ptr != r.ptr;
	}
	
private:
	T* ptr;
};

#endif
