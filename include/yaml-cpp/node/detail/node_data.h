#ifndef VALUE_DETAIL_NODE_DATA_H_62B23520_7C8E_11DE_8A39_0800200C9A66
#define VALUE_DETAIL_NODE_DATA_H_62B23520_7C8E_11DE_8A39_0800200C9A66

#if defined(_MSC_VER) || (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ >= 4)) // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif


#include "yaml-cpp/dll.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/ptr.h"
#include "yaml-cpp/node/type.h"
#include <boost/utility.hpp>
#include <list>
#include <utility>
#include <vector>

namespace YAML
{
	namespace detail
	{
		class node_data: private boost::noncopyable
		{
		public:
			node_data();
			
			void mark_defined();
			void set_type(NodeType::value type);
			void set_tag(const std::string& tag);
			void set_null();
			void set_scalar(const std::string& scalar);
			
			bool is_defined() const { return m_isDefined; }
			NodeType::value type() const { return m_isDefined ? m_type : NodeType::Undefined; }
			const std::string& scalar() const { return m_scalar; }
			const std::string& tag() const { return m_tag; }
			
			// size/iterator
			std::size_t size() const;
			
			const_node_iterator begin() const;
			node_iterator begin();
			
			const_node_iterator end() const;
			node_iterator end();

			// sequence
			void append(node& node, shared_memory_holder pMemory);
			void insert(node& key, node& value, shared_memory_holder pMemory);

			// indexing
			template<typename Key> node& get(const Key& key, shared_memory_holder pMemory) const;
			template<typename Key> node& get(const Key& key, shared_memory_holder pMemory);
			template<typename Key> bool remove(const Key& key, shared_memory_holder pMemory);
			
			node& get(node& key, shared_memory_holder pMemory) const;
			node& get(node& key, shared_memory_holder pMemory);
			bool remove(node& key, shared_memory_holder pMemory);
			
		public:
			static std::string empty_scalar;
			
		private:
			void compute_seq_size() const;
			void compute_map_size() const;

			void reset_sequence();
			void reset_map();
			
			void insert_map_pair(node& key, node& value);
			void convert_to_map(shared_memory_holder pMemory);
			void convert_sequence_to_map(shared_memory_holder pMemory);
			
			template<typename T>
			static bool equals(node& node, const T& rhs, shared_memory_holder pMemory);
			
			template<typename T>
			static node& convert_to_node(const T& rhs, shared_memory_holder pMemory);

		private:
			bool m_isDefined;
			NodeType::value m_type;
			std::string m_tag;
			
			// scalar
			std::string m_scalar;
			
			// sequence
			typedef std::vector<node *> node_seq;
			node_seq m_sequence;
			
			mutable std::size_t m_seqSize;
			
			// map
			typedef std::map<node *, node *> node_map;
			node_map m_map;
			
			typedef std::pair<node *, node *> kv_pair;
			typedef std::list<kv_pair> kv_pairs;
			mutable kv_pairs m_undefinedPairs;
		};
	}
}

#endif // VALUE_DETAIL_NODE_DATA_H_62B23520_7C8E_11DE_8A39_0800200C9A66
