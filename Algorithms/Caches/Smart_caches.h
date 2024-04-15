#pragma once
#include <algorithm>
#include <list>
#include <unordered_map>
#include <type_traits>
//import std;
//using namespace std;

//02. Ещё чуть быстрее делаем свой контейнер - Антон Полухин
// https://www.youtube.com/watch?v=60XhYzkXu1M
template<class T, class Hash = std::hash<T>, class Equal = std::equal_to<T>>
class lru_1
{
	// we assume that lst.front() is the least used one.
public:
	bool Has(const T& key) {
		auto it = map_.find(key);
		if (it == map_.end()) return false;

		auto list_it = it->second;
		lst.erase(list_it);
		it->second = lst.insert(lst.end(), key);
		return true;
	}

	bool Put(const T& key) {
		auto it = map_.find(key);
		if (it != map_.end()) {
			lst.erase(it->second);
			it->second = lst.insert(lst.end(), key);
			return false;
		}
		if (lst.size() == max_size_) {
			T last = lst.front();
			lst.pop_front();
			map_.erase(last);
			map_[key] = lst.insert(lst.end(), key);
		}
		else {
			map_[key] = lst.insert(lst.end(), key);
		}
		return true;
	}

private:
	std::list<int> lst;
	std::unordered_map<T, typename std::list<T>::iterator, Hash, Equal> map_;
	std::size_t max_size_;
};

template<class T, class Hash = std::hash<T>, class Equal = std::equal_to<T> >
class lru_2
{
	bool Has(const T& Key)
	{
		auto it = map_.find(Key);
		if (it == map_.end()) return false;

		auto list_it = it->second;
		lst.splice(lst.end(), lst, list_it);
		return true;
	} // 2x faster than lru_1

	bool Put(const T& key) {
		auto it = map_.find(key);
		if (it != map_.end()) {
			auto list_it = it->second;
			lst.splice(lst.end(), lst, list_it);
			return false;
		}
		if (lst.size() == max_size_) {
			T last = lst.front();
			map_.erase(last);
			lst.front() = key;
			lst.splice(lst.end(), lst, lst.begin());
			map_[key] = --lst.end();
		}
		else {
			map_[key] = lst.insert(lst.end(), key);
		}
		return true;
	} // not faster


private:
	std::list<T> lst;
	std::unordered_map<T, typename std::list<T>::iterator, Hash, Equal> map_;
	std::size_t max_size_;
};

class lru_3
{
public:
	bool Put(const int& key) {
		auto it = m.find(key);
		if (it != m.end()) {
			l.splice(l.end(), l, it->second);
			return false;
		}
		if (l.size() == max_size) {
			int last = l.front();
			auto node = m.extract(last);
			l.front() = key;
			l.splice(l.end(), l, l.begin());
			node.key() = key;
			node.mapped() = --l.end();
			m.insert(std::move(node));
		}
		else {
			m[key] = l.insert(l.end(), key);
		}
		return true;
	} // Put into full list is 30% faster.

private:

	std::list<int> l;
	std::unordered_map<int, std::list<int>::iterator> m;
	int max_size;
};


// Let's make things faster.
// Boost intrusive list
#include <boost/intrusive/link_mode.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>

using LinkMode = boost::intrusive::link_mode<
#ifdef NDEBUG
	boost::intrusive::normal_link
#else
	boost::intrusive::safe_link
#endif
>;
using LruHook = boost::intrusive::list_base_hook<LinkMode>;
class LruNode final :public LruHook {};
class lru_4
{
public:

	bool Has(const int& key) {
		auto it = m.find(key);
		if (it == m.end()) return false;

		l.splice(l.end(), l, l.iterator_to(it->second));
		return true;
	}// 30 percent faster.

	bool Put(const int& key) {
		auto it = m.find(key);
		if (it != m.end()) {
			l.splice(l.end(), l, l.iterator_to(it->second));
			return false;
		}
		// 
		//if (l.size() == max_size) {
		// boost::intrusive::constant_time_size<false> will make size() O(n)
		// 10 - 50 times slower with l.size() 
		// to fix this lest use map.size()
		if (m.size() == max_size) {
			auto node = m.extract(GetLeastRecentKey());
			l.splice(l.end(), l, l.begin());
			node.key() = key;
			m.insert(std::move(node));
		}
		else
		{
			auto [it, ok] = m.emplace(key, LruNode{});
			assert(ok);
			l.insert(l.end(), it->second);
		}
		return true;
	}



private:
	using List = boost::intrusive::list<
		LruNode,
		boost::intrusive::constant_time_size<false>
	>;

	// boost::intrusive::constant_time_size<false> will make size() O(n)

	using Map = std::unordered_map<int, LruNode>;

	Map m;
	List l;
	std::size_t max_size;

	const int& GetLeastRecentKey() {
		using Pair = typename Map::value_type;
		constexpr auto offset = offsetof(Pair, second) - offsetof(Pair, first);
		return *reinterpret_cast<const int*>(
			reinterpret_cast<const char*>(&l.front()) - offset);
	}
};





class LRUCache_copilot1 {
public:
	LRUCache_copilot1(int capacity) : _capacity(capacity) {}

	int get(int key) {
		auto it = _cache.find(key);
		if (it == _cache.end()) return -1;
		touch(it);
		return it->second.first;
	}

	void put(int key, int value) {
		auto it = _cache.find(key);
		if (it != _cache.end()) touch(it);
		else {
			if (_cache.size() == _capacity) {
				_cache.erase(_used.back());
				_used.pop_back();
			}
			_used.push_front(key);
		}
		_cache[key] = {value, _used.begin()};
	}

private:
	typedef std::pair<int, std::list<int>::iterator> pair_t;
	typedef std::unordered_map<int, pair_t> cache_t;

	void touch(cache_t::iterator it) {
		int key = it->first;
		_used.erase(it->second.second);
		_used.push_front(key);
		it->second.second = _used.begin();
	}

	cache_t _cache;
	std::list<int> _used;
	int _capacity;
};


int main() {

}