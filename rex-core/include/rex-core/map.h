#pragma once

#include "rex-core/vec.h"

namespace rex::core
{
	template <typename K, typename T>
	struct Node
	{
		K key;
		T value;
	};

	template <typename K, typename T>
	struct Map
	{
		Vec<Node<K, T>> nodes;
	};

	template <typename K, typename T>
	inline static Map<K, T>
	map_new(Allocator* allocator = default_allocator())
	{
		return { vec_new<Node<K,T>>(allocator) };
	}

	template <typename K, typename T>
	inline static Map<K, T>
	map_with_capacity(i64 capacity, Allocator* allocator = default_allocator())
	{
		return { vec_with_capacity<Node<K,T>>(capacity, allocator) };
	}

	template <typename K, typename T>
	inline static Map<K, T>
	map_from(std::initializer_list<Node<K, T>> nodes, Allocator* allocator = default_allocator())
	{
		return { vec_from<Node<K,T>>(nodes, allocator) };
	}

	template <typename K, typename T>
	inline static Map<K, T>
	map_copy(Map<K, T>& self, Allocator* allocator = default_allocator())
	{
		return { vec_copy(self.nodes) };
	}

	template <typename K, typename T>
	inline static void
	map_free(Map<K, T>& self)
	{
		return vec_free(self.nodes);
	}

	template <typename K, typename T>
	inline static void
	map_insert(Map<K, T>& self, K key, T value)
	{
		vec_push(self.nodes, {key, value});
	}

	template <typename K, typename T>
	inline static Node<K, T>*
	map_find(Map<K, T>& self, K key)
	{
		for (auto& node: self.nodes)
			if (node.key == key)
				return &node;
		return nullptr;
	}
}
